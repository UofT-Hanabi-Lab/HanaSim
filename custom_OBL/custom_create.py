# Copyright (c) Facebook, Inc. and its affiliates.
# All rights reserved.
#
# This source code is licensed under the license found in the
# LICENSE file in the root directory of this source tree.
#
# All rights reserved.
#
# This source code is licensed under the license found in the
# LICENSE file in the root directory of this source tree.
#
import os
import sys
import set_path

# Add the current directory to path for custom modules
current_dir = os.path.dirname(os.path.abspath(__file__))
if current_dir not in sys.path:
    sys.path.insert(0, current_dir)

set_path.append_sys_path()

import rela
import hanalearn

assert rela.__file__.endswith(".so")
assert hanalearn.__file__.endswith(".so")

from custom_hanabi_env import CustomHanabiEnv
from custom_thread_loop import CustomHanabiThreadLoop

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
            False,
        )
        games.append(game)
    
    print(f"Created {len(games)} custom Hanabi environments")
    return games


def create_threads(num_thread, num_game_per_thread, actors, games):
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


def flatten(actors):
    flat = []
    for actor_list in actors:
        flat.extend(actor_list)
    return flat 