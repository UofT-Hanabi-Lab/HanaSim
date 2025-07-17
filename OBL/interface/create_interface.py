import os
import sys

BUILD_DIR = os.path.join(os.path.dirname(__file__), "../build")
if BUILD_DIR not in sys.path:
    sys.path.insert(0, BUILD_DIR)

import hana_sim

class Create_Env():
    """
    Create hanabi environment using hana_sim
    """

    def initialize(self, num_player: int, seed: int, bomb: int, hand_size: int, random_start_player: int) -> hana_sim.HanabiEnv():
        """Initialize game environment."""
        return hana_sim.HanabiEnv(num_players=num_player)
