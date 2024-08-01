
#include <torch/script.h>
#include <mutex>
#include <condition_variable>

#pragma once

using TensorDict = std::unordered_map<std::string, torch::Tensor>;
using TorchTensorDict = torch::Dict<std::string, torch::Tensor>;