// bindings.cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>
#include <pybind11/stl_bind.h>

#include "../include/hanabi_env.h"

namespace py = pybind11;

// Central macro list to define and bind enum values
#define FOR_EACH_PLAYER_NAME(MACRO) \
    MACRO(RandomBot)                \
    MACRO(HolmesBot)                \
    MACRO(SmartBot)

// Convert enum entry to pybind11 binding
#define BIND_ENUM_ENTRY(x) .value(#x, PlayerName::x)

using Vector3DBool = std::vector<std::vector<std::vector<bool>>>;
PYBIND11_MAKE_OPAQUE(Vector3DBool);

PYBIND11_MODULE(hana_sim, m)
{
    m.doc() = "A PyBind11 module that wraps the HanabiEnv for Gym-like usage.";

    py::enum_<PlayerName>(m, "PlayerName")
        FOR_EACH_PLAYER_NAME(BIND_ENUM_ENTRY);

    py::bind_vector<Vector3DBool>(m, "Vector3DBool");

    py::class_<Observation>(m, "Observation")
        .def(py::init<>())
        .def_readwrite("hands", &Observation::hands)
        .def_readwrite("possible_colors", &Observation::possible_colors)
        .def_readwrite("possible_ranks", &Observation::possible_ranks)
        .def_readwrite("fireworks", &Observation::fireworks)
        .def_readwrite("hint_tokens", &Observation::hint_tokens)
        .def_readwrite("lives_remaining", &Observation::lives_remaining)
        .def_readwrite("deck_size", &Observation::deck_size)
        .def_readwrite("discards", &Observation::discards)
        .def_readwrite("legal_actions", &Observation::legal_actions)
        .def_readwrite("current_player_id", &Observation::current_player_id)
        .def_readwrite("vector_encoding", &Observation::vector_encoding);

    py::class_<HanabiEnv::StepResult>(m, "StepResult")
        .def_readonly("observation", &HanabiEnv::StepResult::observation)
        .def_readonly("reward", &HanabiEnv::StepResult::reward)
        .def_readonly("done", &HanabiEnv::StepResult::done)
        .def_readonly("info", &HanabiEnv::StepResult::info)
        .def_readonly("last_move", &HanabiEnv::StepResult::last_move,
                      "The most recently completed move in the game");

    py::class_<HanabiEnv>(m, "HanabiEnv")
        .def(py::init<int>(), py::arg("num_players") = 2)
        .def("reset", &HanabiEnv::reset, "Reset the environment.")
        .def("step", &HanabiEnv::step, "Take a step in the environment using action_id.")
        .def("render", &HanabiEnv::render, "Render the current state.")
        .def("add_player", &HanabiEnv::add_player, "Register a HanaSim player to the environment")
        .def("get_supported_player_names", &HanabiEnv::get_supported_player_names,
            "Get a list of PlayerName enums for players that can be added to the environment")
        .def_property_readonly("deck", &HanabiEnv::get_deck, "The start deck for this game");
}
