import os
import sys

BUILD_DIR = os.path.join(os.path.dirname(__file__), "../build")
if BUILD_DIR not in sys.path:
    sys.path.insert(0, BUILD_DIR)

import hana_sim
import rela
from hanabi_env import CustomHanabiEnv
from thread_loop import CustomHanabiThreadLoop


def create_envs(
    num_env,
    seed,
    num_player,
    bomb,
    max_len,
    *,
    hand_size=5,
    random_start_player=1,
):
    """
    Create environments using CustomHanabiEnv to replace hanalearn.HanabiEnv
    This function is compatible with the original OBL/pyhanabi/create.py version
    """
    games = []
    for game_idx in range(num_env):
        params = {
            "players": str(num_player),
            "seed": str(seed + game_idx),
            "bomb": str(bomb),
            "hand_size": str(hand_size),
            "random_start_player": str(random_start_player),
        }
        game = CustomHanabiEnv(
            params,
            max_len,
            False,  # verbose
        )
        games.append(game)
    return games


def flatten(s):
    """
    Recursively flatten nested lists
    """
    if s == []:
        return s
    if isinstance(s[0], list):
        return flatten(s[0]) + flatten(s[1:])
    return s[:1] + flatten(s[1:])


def create_threads(num_thread, num_game_per_thread, actors, games):
    """
    Create threads using CustomHanabiThreadLoop to replace hanalearn.HanabiThreadLoop
    This function is compatible with the original OBL/pyhanabi/create.py version
    """
    context = rela.Context()
    threads = []
    for thread_idx in range(num_thread):
        envs = games[
            thread_idx * num_game_per_thread : (thread_idx + 1) * num_game_per_thread
        ]
        thread = CustomHanabiThreadLoop(envs, actors[thread_idx], False)
        threads.append(thread)
        context.push_thread_loop(thread)
    print(
        "Finished creating %d threads with %d games and %d actors"
        % (len(threads), len(games), len(flatten(actors)))
    )
    return context, threads
