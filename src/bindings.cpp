// bindings.cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // 如果要返回/接收 STL 容器
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>

#include "../include/hanabi_env.h"

namespace py = pybind11;

// 为了返回 (obs, reward, done, info)，我们可以将 StepResult 也绑定一下，
// 这样Python就能拿到里面的成员变量。
PYBIND11_MODULE(hana_sim, m) {
    m.doc() = "A PyBind11 module that wraps the HanabiEnv for Gym-like usage.";

    py::class_<Observation>(m, "Observation")
        .def_readwrite("hands", &Observation::hands)
        .def_readwrite("fireworks", &Observation::fireworks)
        .def_readwrite("hint_tokens", &Observation::hint_tokens)
        .def_readwrite("lives_remaining", &Observation::lives_remaining)
        .def_readwrite("deck_size", &Observation::deck_size)
        .def_readwrite("discards", &Observation::discards)
        .def_readwrite("legal_actions", &Observation::legal_actions)
        .def_readwrite("current_player_id", &Observation::current_player_id);
    py::class_<HanabiEnv::StepResult>(m, "StepResult")
        .def_readwrite("observation", &HanabiEnv::StepResult::observation)
        .def_readwrite("reward", &HanabiEnv::StepResult::reward)
        .def_readwrite("done", &HanabiEnv::StepResult::done)
        .def_readwrite("info", &HanabiEnv::StepResult::info);
    

    // 再绑定HanabiEnv本体
    py::class_<HanabiEnv>(m, "HanabiEnv")
        .def(py::init<int>(), py::arg("num_players")=2)  // 构造函数
        .def("reset", &HanabiEnv::reset, "Reset the environment.")
        .def("step", &HanabiEnv::step, "Take a step in the environment using action_id.")
        .def("render", &HanabiEnv::render, "Render the current state.")
        ;
    
}