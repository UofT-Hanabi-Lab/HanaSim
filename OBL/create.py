import os
import sys

BUILD_DIR = os.path.join(os.path.dirname(__file__), "../build")
if BUILD_DIR not in sys.path:
    sys.path.insert(0, BUILD_DIR)

import hana_sim


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
    games = []
    for game_idx in range(num_env):
        game = hana_sim.HanabiEnv(num_player=num_player)
        games.append(game)
    return games

# TODO:
def flatten(s):
    if s == []:
        return s
    if isinstance(s[0], list):
        return flatten(s[0]) + flatten(s[1:])
    return s[:1] + flatten(s[1:])

# TODO:
def create_threads(num_thread, num_game_per_thread, actors, games):
    context = rela.Context()
    threads = []
    for thread_idx in range(num_thread):
        envs = games[
            thread_idx * num_game_per_thread : (thread_idx + 1) * num_game_per_thread
        ]
        thread = hanalearn.HanabiThreadLoop(envs, actors[thread_idx], False)
        threads.append(thread)
        context.push_thread_loop(thread)
    print(
        "Finished creating %d threads with %d games and %d actors"
        % (len(threads), len(games), len(flatten(actors)))
    )
    return context, threads
