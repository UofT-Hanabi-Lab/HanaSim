import hana_sim
import sys
from io import StringIO
from contextlib import redirect_stdout
import random
import numpy as np
import time

def print_observation(obs):
    """
    Print the observation object
    """
    print("Observation:")
    
    # Hand for the current player
    print("Hands:")
    for i, hand in enumerate(obs.hands):
        print(f"Hands of player {i + 1}")
        for color, rank in hand:
            print(f"    - {color} {rank}")
    
    print("  Fireworks:")
    for color, rank in obs.fireworks.items():
        print(f"    - {color}: {rank}")
    
    print(f"  Hint Tokens: {obs.hint_tokens}")
    print(f"  Lives Remaining: {obs.lives_remaining}")
    print(f"  Deck Size: {obs.deck_size}")
    
    print("  Discards:")
    for color, rank in obs.discards:
        print(f"    - {color} {rank}")
    
    print("  Legal Actions:")
    for action in obs.legal_actions:
        print(action)
    print(f"  Current Player ID: {obs.current_player_id}")

env = hana_sim.HanabiEnv(num_players=2)
env.render()
obs = env.reset()
stepResult = None
print_observation(obs)
while True:
    ac_len = len(obs.legal_actions)
    # Random select legal actions.
    rand_action = obs.legal_actions[random.randint(0, ac_len-1)]
    print("choose action:", rand_action)
    stepResult = env.step(rand_action)#(3, -1, 0, 1, [], 6, 6))#obs.legal_actions[0])
    
    print_observation(stepResult.observation)
    print(f"current score: {stepResult.info}")
    if stepResult.done:
        break
    print("\n\n\n\n\n\n\n\n\n\n")
    time.sleep(0.1)