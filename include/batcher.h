
#include <torch/script.h>
#include <mutex>
#include <condition_variable>
#include <torch/torch.h>
#include <cstdint>

#pragma once

using TensorDict = std::unordered_map<std::string, torch::Tensor>;
using TorchTensorDict = torch::Dict<std::string, torch::Tensor>;

inline TensorDict iValueToTensorDict(const torch::IValue& value, torch::DeviceType device, bool detach) {
    std::unordered_map<std::string, torch::Tensor> map;
    auto dict = value.toGenericDict();
    for (auto& name2tensor : dict) {
        auto name = name2tensor.key().toString();
        torch::Tensor tensor = name2tensor.value().toTensor();
        if (detach) {
            tensor = tensor.detach();
        }
        tensor = tensor.to(device);
        map.insert({name->string(), tensor});
    }
    return map;
}

// TODO: this may be simplified with constructor in the future version
inline TorchTensorDict tensorDictToTorchDict(
    const TensorDict& tensorDict, const torch::Device& device) {
  TorchTensorDict dict;
  for (const auto& name2tensor : tensorDict) {
    dict.insert(name2tensor.first, name2tensor.second.to(device));
  }
  return dict;
}

class FutureReply {
    public:
        FutureReply() : ready_(false) {}

        TensorDict get(int slot) {
            std::unique_lock<std::mutex> lk(m_ready_);
            cv_ready_.wait(lk, [this] { return ready_; });
            lk.unlock();

            TensorDict e;
            for (const auto& kv : data_) {
                assert(slot >= 0 && slot < kv.second.size(0));
                e[kv.first] = kv.second[slot];
            }
            return e;
            // return data_[slot];
        }

        void set(TensorDict&& t) {
            // assert(t.device().is_cpu());
            {
                std::lock_guard<std::mutex> lk(m_ready_);
                ready_ = true;
                data_ = std::move(t);
            }
            cv_ready_.notify_all();
        }

    private:
        // no need for protection, only set() can set it
        // torch::Tensor data_;
        TensorDict data_;

        std::mutex m_ready_;
        bool ready_;
        std::condition_variable cv_ready_;
};

class ExitThread: public std::exception {};

class Batcher {
    public:
        Batcher(int batch_size) : batch_size_(batch_size), next_slot_(0), num_active_write_(0), current_reply_(nullptr), next_reply_(std::make_shared<FutureReply>())
        {     
        }

        // send data into batcher
        std::shared_ptr<FutureReply> send(const TensorDict& t, int* slot) {
            std::unique_lock<std::mutex> lk(m_next_slot_);

            // init buffer
            if (buffer_.empty()) {
                for (const auto& kv : t) {
                    auto t = kv.second.sizes();
                    std::vector<std::int64_t> sizes;
                    sizes.push_back(batch_size_);
                    sizes.insert(sizes.end(), t.begin(), t.end());

                    buffer_[kv.first] = torch::zeros(sizes, kv.second.dtype());
                }
            }

            assert(next_slot_ <= batch_size_);
            // wait if current batch is full and not extracted
            cv_next_slot_.wait(lk, [this] { return next_slot_ < batch_size_; });

            *slot = next_slot_;
            ++next_slot_;
            ++num_active_write_;
            lk.unlock();

            // this will copy
            for (const auto& kv : t) {
                buffer_[kv.first][*slot] = kv.second;
            }

            // batch has not been extracted yet
            assert(num_active_write_ > 0);
            assert(next_reply_ != nullptr);
            auto reply = next_reply_;
            lk.lock();
            --num_active_write_;
            lk.unlock();
            if (num_active_write_ == 0) {
                cv_get_batch_.notify_one();
            }
            return reply;
        }

        // get batch input from batcher
        TensorDict get() {std::unique_lock<std::mutex> lk(m_next_slot_);
            cv_get_batch_.wait(lk, [this] { return next_slot_ > 0 && num_active_write_ == 0 || exit_; });
            if (exit_) {
                throw ExitThread();
            }
            TensorDict batch;
            for (const auto& kv : buffer_) {
                batch[kv.first] = kv.second.narrow_copy(0, 0, next_slot_).contiguous();
            }

            // assert currentReply has been handled
            assert(current_reply_ == nullptr);
            current_reply_ = std::move(next_reply_);
            next_reply_ = std::make_shared<FutureReply>();

            next_slot_ = 0;
            lk.unlock();

            cv_next_slot_.notify_all();
            return batch;
        }

        // set batch reply for batcher
        void set(TensorDict&& t) {
            for (const auto& kv : t) {
                assert(kv.second.device().is_cpu());
            }
            assert(current_reply_ != nullptr);
            current_reply_->set(std::move(t));
            current_reply_ = nullptr;
        }

  // hack: public so that they can coordinate thread exit
        bool exit_ = false;
        std::condition_variable cv_get_batch_;
        std::mutex m_next_slot_;

    private:
        const int batch_size_;

        int next_slot_;
        int num_active_write_;
        std::condition_variable cv_next_slot_;

        TensorDict buffer_;

        std::shared_ptr<FutureReply> current_reply_;
        std::shared_ptr<FutureReply> next_reply_;
};