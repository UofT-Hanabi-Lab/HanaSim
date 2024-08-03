/*
MIT License

Copyright (c) Facebook, Inc. and its affiliates.
Copyright (c) 2017 Arthur O'Dwyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "batcher.h"
#include <iostream>
#include <vector>
#include <thread>

#include <torch/torch.h>
#include <torch/script.h>

#include <chrono>
using namespace std::chrono;

#pragma once

class AsyncModelWrapper {
    public:
        AsyncModelWrapper(const std::string& path, const std::string& device, int batchsize) : model_(torch::jit::load(path, torch::Device(device))), device_(torch::Device(device)), batcher_(batchsize) {
            forward_thread_ = std::thread(&AsyncModelWrapper::batchForward, this);
        }

    TensorDict forward(TensorDict input) {
        int slot = -1;
        auto reply = batcher_.send(input, &slot);
        auto output = reply->get(slot);
        return output;
    }

    void batchForward() {
        torch::NoGradGuard no_grad;
        int i = 0;
        while (true) {
            i += 1;
            int B = 1000;
            int P = 1000000;
            if (i % B == 0) {
                if (i % P == 0) std::cerr << "avg time (over " << B << " runs): " << std::endl;
                for (auto& kv : timer_) {
                    if (i % P == 0) std::cerr << kv.first << ", " << kv.second / B << std::endl;
                timer_[kv.first] = 0;
                }
                if (i % P == 0) std::cerr << "===================" << std::endl;
            }

            auto start = high_resolution_clock::now();

            TensorDict input;
            try {
                input = batcher_.get();
            } catch (ExitThread &e) {
                break;
            }
            timer_["batch_size"] += input["s"].size(0);

            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start).count();
            timer_["wait_for_batch"] += duration;

            start = high_resolution_clock::now();
            std::vector<torch::jit::IValue> jit_in;
            jit_in.push_back(tensor_dict_to_torch_dict(input, device_));

            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start).count();
            timer_["to_device"] += duration;

            start = high_resolution_clock::now();
            auto jit_out = model_.forward(jit_in);;
            auto output = iValue_to_tensor_dict(jit_out, torch::kCPU, true);

            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start).count();
            timer_["forward"] += duration;

            // stop = high_resolution_clock::now();
            // duration = duration_cast<microseconds>(stop - start).count();
            // timer_["forward"] += duration;

            start = high_resolution_clock::now();
            batcher_.set(std::move(output));
            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start).count();
            timer_["post_process"] += duration;
            }
        }

        ~AsyncModelWrapper() {
            {
                std::unique_lock<std::mutex> lock(batcher_.m_next_slot_);
                batcher_.exit_ = true;
            }
            batcher_.cv_get_batch_.notify_all();
            forward_thread_.join();
        } 


    private:
        torch::jit::script::Module model_;
        torch::Device device_;

        Batcher batcher_;
        std::thread forward_thread_;

        std::unordered_map<std::string, float> timer_;
};