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

using Vector3DBool = std::vector<std::vector<std::vector<bool>>>;
PYBIND11_MAKE_OPAQUE(Vector3DBool);

/* START: Python docstrings */

const char* obs_doc = R"pbdoc(
    Publicly available information from a particular game state.
)pbdoc";

const char* obs_hands_doc = R"pbdoc(
    :type: list[list[tuple[str, int]]]
)pbdoc";

const char* obs_fireworks_doc = R"pbdoc(
    :type: dict[str, int]
)pbdoc";

const char* obs_hints_doc = R"pbdoc(
    :type: int
)pbdoc";

const char* obs_lives_doc = R"pbdoc(
    :type: int
)pbdoc";

const char* obs_deck_size_doc = R"pbdoc(
    :type: int
)pbdoc";

const char* env_doc = R"pbdoc(
    Represents a Hanabi game ready to be simulated or in the process of simulation.

    This is the backbone of the binding: you must instantiate and keep track of an
    instance of this class to interact with HanaSim.

    :param int num_players: The number of players in the game, defaults to 2
)pbdoc";

const char* env_reset_doc = R"pbdoc(
    Reset the environment (i.e. game) to the initial state.

    :returns: an :py:class:`Observation` representing the initial game state
)pbdoc";

const char* env_step_doc = R"pbdoc(
    Simulate an action performed by the current player and return the updated game
    state. **This method mutates the environment.**

    :param action: Tuple-encoding of the current player's action
    :type action: tuple[int, int, int, int, list[int], int, int]
    :returns: a :py:class:`StepResult` containing the updated game state
)pbdoc";

const char* env_render_doc = R"pbdoc(
    Print the hands of all players in the current game state to stdout.
)pbdoc";

const char* step_result_doc = R"pbdoc(
    The result of a game state update performed by :py:meth:`HanabiEnv.step`.
)pbdoc";

const char* step_result_obs_doc = R"pbdoc(
    Publicly available information about the game state after the step.

    :type: Observation
)pbdoc";

const char* step_result_reward_doc = R"pbdoc(
    The numeric reward associated with the step. For use in reinforcement learning.

    **Unstable.**

    :type: float
)pbdoc";

const char* step_result_done_doc = R"pbdoc(
    Whether the updated game state represents a completed game.

    :type: bool
)pbdoc";

const char* step_result_info_doc = R"pbdoc(
    Additional info associated with the step.

    For development use, **do not use in production**.

    :type: str
)pbdoc";

/* END: Python docstrings */

PYBIND11_MODULE(hana_sim, m) {
    m.doc() = "A Python binding for the HanaSim Hanabi simulator.";

    py::bind_vector<Vector3DBool>(m, "Vector3DBool");

    py::class_<Observation>(m, "Observation", obs_doc)
        .def(py::init<>())
        .def_readwrite("hands", &Observation::hands, obs_hands_doc)
        .def_readwrite("possible_colors", &Observation::possible_colors)
        .def_readwrite("possible_ranks", &Observation::possible_ranks)
        .def_readwrite("fireworks", &Observation::fireworks, obs_fireworks_doc)
        .def_readwrite("hint_tokens", &Observation::hint_tokens, obs_hints_doc)
        .def_readwrite("lives_remaining", &Observation::lives_remaining, obs_lives_doc)
        .def_readwrite("deck_size", &Observation::deck_size, obs_deck_size_doc)
        .def_readwrite("discards", &Observation::discards)
        .def_readwrite("legal_actions", &Observation::legal_actions)
        .def_readwrite("current_player_id", &Observation::current_player_id)
        .def_readwrite("vector_encoding", &Observation::vector_encoding);

    py::class_<HanabiEnv::StepResult>(m, "StepResult", step_result_doc)
        .def_readwrite("observation", &HanabiEnv::StepResult::observation,
            step_result_obs_doc)
        .def_readwrite("reward", &HanabiEnv::StepResult::reward, step_result_reward_doc)
        .def_readwrite("done", &HanabiEnv::StepResult::done, step_result_done_doc)
        .def_readwrite("info", &HanabiEnv::StepResult::info, step_result_info_doc);

    py::class_<HanabiEnv>(m, "HanabiEnv", env_doc)
        .def(py::init<int>(), py::arg("num_players")=2)
        .def("reset", &HanabiEnv::reset, env_reset_doc)
        .def("step", &HanabiEnv::step, env_step_doc)
        .def("render", &HanabiEnv::render, env_render_doc);
};
