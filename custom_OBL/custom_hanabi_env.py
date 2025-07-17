import os
import sys
import numpy as np
from typing import Dict, List, Tuple, Any

# Add the build directory to path
BUILD_DIR = os.path.join(os.path.dirname(__file__), "build")
if BUILD_DIR not in sys.path:
    sys.path.insert(0, BUILD_DIR)

import hana_sim
from hanabi_gym import convert_observation_to_space, convert_action, explain_action

class MyHanabiEnv(gym.Env):
    def __init__(self, num_players=2):
        self.num_players = num_players
        super(MyHanabiEnv, self).__init__()
        self.env = hana_sim.HanabiEnv(num_players=num_players)
        
        # Need do some convertion and define action space and observation space.
        self.action_space = action_space = spaces.Tuple((
                                spaces.Discrete(5),              # a[0] -> move_type ∈ {1,2,3,4,5}
                                spaces.Discrete(self.num_players + 1),  # a[1] -> target_player_id ∈ {-1, 0, 1, ..., num_players-1} (offset)
                                spaces.Discrete(self.num_players),      # a[2] -> from_player_id ∈ {0, 1, ..., num_players-1}
                                spaces.Discrete(6),              # a[3] -> card index ∈ {-1, 0, 1, 2, 3, 4}  (offset)
                                spaces.Sequence(spaces.Discrete(5)), # a[4] -> card_indices ∈ {0,1,2,3,4} 
                                spaces.Discrete(6),              # a[5] -> color ∈ {1,2,3,4,5} 
                                spaces.Discrete(6)               # a[6] -> rank ∈ {1,2,3,4,5} 
                            ))
        self.observation_space = spaces.Dict({
                                "hands": spaces.Box(low=-1, high=5, shape=(2, 5, 2), dtype=np.int32),  # (num_players, max_cards, 2)
                                "possible_colors": spaces.Box(low=0, high=1, shape=(2, 5, 6), dtype=np.bool_),  # (num_players, max_cards, num_colors)
                                "possible_ranks": spaces.Box(low=0, high=1, shape=(2, 5, 6), dtype=np.bool_),  # (num_players, max_cards, num_ranks)
                                "fireworks": spaces.Box(low=0, high=5, shape=(5,), dtype=np.int32),  # (num_colors,)
                                "hint_tokens": spaces.Discrete(9),  # 0 to 8
                                "lives_remaining": spaces.Discrete(4),  # 0 to 3
                                "deck_size": spaces.Discrete(51),  # 0 to 50
                                "discards": spaces.Box(low=-1, high=5, shape=(50, 2), dtype=np.int32),  # (max_discards, 2)
                                "legal_actions": spaces.Box(low=-1, high=5, shape=(20, 11), dtype=np.int32),  # (max_legal_actions, 6 + max_action_params)
                                "current_player_id": spaces.Discrete(2),  # 0 to 1
                                "vector_encoding": spaces.Box(low=0, high=1, shape=(136,), dtype=np.int32)  # Bit vector encoding
                            })

    def reset(self):
        # Reset the environment and return the initial observation
        obs = self.env.reset()
        print_observation_structure(obs)  # Print the raw observation structure
        return self._convert_observation(obs)

    def step(self, action):
        # Execute action in the simulator and return the result
        step_result = self.env.step(action)
        print_observation_structure(step_result.observation)  # Print the raw observation structure
        new_observation = self._convert_observation(step_result.observation)
        reward = step_result.reward
        done = step_result.done
        info = step_result.info
        return new_observation, reward, done, info

    def render(self, mode='human'):
        # Render the environment using the simulator's render method
        self.env.render()

    def _convert_observation(self, obs):
        # Convert the observation from the simulator to the format expected by Gym
        return convert_observation_to_space(obs)

class CustomHanabiEnv:
    
    def __init__(self, game_params: Dict[str, str], max_len: int, verbose: bool = False):

        self.num_players = int(game_params.get('players', '2'))
        self.seed = int(game_params.get('seed', '0'))
        self.bomb = int(game_params.get('bomb', '0'))
        self.hand_size = int(game_params.get('hand_size', '5'))
        self.random_start_player = int(game_params.get('random_start_player', '1'))
        self.max_len = max_len
        self.verbose = verbose
        
        self.env = hana_sim.HanabiEnv(num_players=self.num_players)
        
        self.current_state = None
        self.num_step = 0
        self.last_episode_score = -1
        self.last_active_player = -1
        self.last_move = None
        self.step_reward = 0.0
        self.color_reward = 0.0
        
        if self.seed > 0:
            np.random.seed(self.seed)
        
        if self.verbose:
            print(f"CustomHanabiEnv created with parameters:")
            print(f"  num_players: {self.num_players}")
            print(f"  seed: {self.seed}")
            print(f"  bomb: {self.bomb}")
            print(f"  hand_size: {self.hand_size}")
            print(f"  random_start_player: {self.random_start_player}")
            print(f"  max_len: {max_len}")
    
    def feature_size(self, sad: bool = False) -> Tuple[int, int, int]:
        """
        Calculate feature dimension sizes based on hana_sim's actual implementation
        """
        # Calculate based on hana_sim's encoding functions
        cards_per_hand = 5 if self.num_players <= 3 else 4
        
        # HandsSectionLength: (num_players - 1) * cards_per_hand * 25 + cards_per_hand * 10
        hands_section = (self.num_players - 1) * cards_per_hand * 25 + cards_per_hand * 10
        
        # BoardSectionLength: 50 + 25 + 8 + 3 = 86
        board_section = 86
        
        # DiscardSectionLength: 50
        discard_section = 50
        
        # Total vector encoding size
        vector_encoding_size = hands_section + board_section + discard_section
        
        # Other observation components
        hands_size = self.num_players * cards_per_hand * 2  # (color, rank)
        possible_colors_size = self.num_players * cards_per_hand * 6  # 6 colors
        possible_ranks_size = self.num_players * cards_per_hand * 6   # 6 ranks
        fireworks_size = 5  # 5 colors
        discards_size = 50 * 2  # Maximum 50 discarded cards, each (color, rank)
        legal_actions_size = 20 * 11  # Maximum 20 legal actions, each 11-dimensional
        
        # Base feature size
        base_size = (hands_size + possible_colors_size + possible_ranks_size + 
                    fireworks_size + 1 + 1 + 1 + discards_size + legal_actions_size + 
                    1 + vector_encoding_size)  # +1 for hint_tokens, lives, deck_size, current_player
        
        if sad:
            # Add extra features in SAD mode
            sad_size = 50  # Example value, needs adjustment based on actual situation
            base_size += sad_size
        
        # Private features (current player's hand information)
        priv_size = cards_per_hand * 2  # Current player's hand
        
        # Public features (information visible to all players)
        publ_size = base_size - self.num_players * cards_per_hand * 2
        
        return (base_size, priv_size, publ_size)
    
    def num_action(self) -> int:
        """
        Calculate action space size based on hana_sim's get_legal_moves function
        """
        cards_per_hand = 5 if self.num_players <= 3 else 4
        
        # Calculate maximum possible legal actions
        # 1. Color hints: each color for each other player
        color_hints = (self.num_players - 1) * 5  # 5 colors
        
        # 2. Rank hints: each rank for each other player
        rank_hints = (self.num_players - 1) * 5   # 5 ranks
        
        # 3. Play: each card of current player
        play_actions = cards_per_hand
        
        # 4. Discard: each card of current player (when hint tokens < 8)
        discard_actions = cards_per_hand
        
        # Total actions
        total_actions = color_hints + rank_hints + play_actions + discard_actions
        
        # Add one invalid action ID
        return total_actions + 1
    
    def reset(self):
        if not self.terminated():
            raise RuntimeError("Cannot reset non-terminated environment")
        
        obs = self.env.reset()
        self.current_state = obs
        self.num_step = 0
        self.last_episode_score = -1
        self.last_active_player = -1
        self.last_move = None
        self.step_reward = 0.0
        
        if self.verbose:
            print("Environment reset")
    
    
    def step(self, action):
        """
        Execute one step action
        
        Args:
            action: Action, can be action ID or action tuple
        """
        if self.terminated():
            raise RuntimeError("Cannot step terminated environment")
        
        if self.max_len > 0 and self.num_step >= self.max_len:
            raise RuntimeError(f"Maximum steps ({self.max_len}) reached")
        
        self.num_step += 1
        self.last_active_player = self.get_current_player()
        
        # Convert action format
        if isinstance(action, int):
            # If it's an action ID, need to convert to action tuple
            # Need to implement action ID to action tuple mapping here
            action_tuple = self._action_id_to_tuple(action)
        else:
            action_tuple = action
        
        # Record action
        self.last_move = action_tuple
        
        # Execute action
        step_result = self.env.step(action_tuple)
        self.current_state = step_result.observation
        self.step_reward = step_result.reward
        
        # Check if ended
        if step_result.done:
            self.last_episode_score = self.get_score()
        
        # Add color reward (if enabled)
        if self.color_reward > 0 and self._is_color_hint_action(action_tuple):
            self.step_reward += self.color_reward
        
        if self.verbose:
            print(f"Step {self.num_step}: Action {action_tuple}, Reward {self.step_reward}")
    
    def terminated(self) -> bool:
        """
        Check if environment is terminated based on hana_sim's game end conditions
        """
        if self.current_state is None:
            return True
        
        # Check maximum steps
        if self.max_len > 0 and self.num_step >= self.max_len:
            return True
        
        # Check if game is ended (based on hana_sim logic)
        # Game end conditions: score reaches 25, or lives <= 0, or deck is empty
        score = self.get_score()
        lives = self.get_life()
        deck_size = self.current_state.deck_size if self.current_state else 0
        
        # From hana_sim's step function: done = !((last_score_ < 25) && (curr_state_->get_num_lives() > 0) && !(curr_state_->get_deck().empty()))
        return not (score < 25 and lives > 0 and deck_size > 0)
    
    def get_current_player(self) -> int:
        """
        Get current player ID
        """
        if self.current_state is None:
            raise RuntimeError("Environment not initialized")
        return self.current_state.current_player_id
    
    def last_episode_score(self) -> int:
        return self.last_episode_score
    
    def get_num_players(self) -> int:
        return self.num_players
    
    def get_score(self) -> int:
        if self.current_state is None:
            return 0
        
        fireworks = self.current_state.fireworks
        score = sum(fireworks.values())
        return score
    
    def get_life(self) -> int:
        if self.current_state is None:
            return 3
        return self.current_state.lives_remaining
    
    def get_info(self) -> int:
        if self.current_state is None:
            return 8
        return self.current_state.hint_tokens
    
    def get_fireworks(self) -> List[int]:
        """
        Get fireworks state, returns list format [0, red, white, yellow, green, blue]
        Note: Index 0 is unused, consistent with hana_sim's piles_ format
        """
        if self.current_state is None:
            return [0, 0, 0, 0, 0, 0]  # 6 elements, index 0 unused
        
        fireworks = self.current_state.fireworks
        # Convert to list format [0, red, white, yellow, green, blue]
        color_order = ['red', 'white', 'yellow', 'green', 'blue']
        result = [0]  # Index 0 unused
        for color in color_order:
            result.append(fireworks.get(color, 0))
        return result
    
    def get_last_action(self) -> int:
        """
        Get the ID of the last action
        """
        if self.last_move is None:
            return -1
        return self._action_tuple_to_id(self.last_move)
    
    def get_step(self) -> int:
        """
        Get current step number
        """
        return self.num_step
    
    def set_color_reward(self, color_reward: float):
        """
        Set color hint reward
        """
        self.color_reward = color_reward
    
    def step_reward(self) -> float:
        """
        Get reward from last step
        """
        return self.step_reward
    
    def last_active_player(self) -> int:
        """
        Get last active player
        """
        return self.last_active_player
    
    def last_move(self):
        """
        Get last move
        """
        return self.last_move
    
    
    def get_hle_state(self):
        """
        Get HLE state (for compatibility)
        """
        # Return current state for compatibility
        return self.current_state
    
    def get_move(self, uid: int):
        """
        Get action based on UID
        """
        # TODO: Need to implement action ID to action mapping here
        return self._action_id_to_tuple(uid)
    
    def get_obs_show_cards(self):

        return self.current_state
    
    # Helper methods
    def _action_id_to_tuple(self, action_id: int):
        """
        Convert action ID to action tuple, format: (move_type, to, from, card_index, card_indices, color, rank)
        Based on hana_sim's decode_move function
        """
        cards_per_hand = 5 if self.num_players <= 3 else 4
        
        if action_id == 0:
            return (1, -1, 0, -1, [], 0, 0)  # Default action
        
        # Calculate number of various actions
        color_hints_per_player = 5
        rank_hints_per_player = 5
        hints_per_player = color_hints_per_player + rank_hints_per_player
        
        current_id = 1
        
        # Color hint actions
        for player in range(self.num_players):
            if player == 0:  # Skip self
                continue
            for color in range(1, 6):  # Colors 1-5
                if action_id == current_id:
                    # Find corresponding card indices
                    card_indices = self._get_card_indices_for_color(player, color)
                    return (1, player, 0, -1, card_indices, color, 0)  # COL_HINT
                current_id += 1
        
        # Rank hint actions
        for player in range(self.num_players):
            if player == 0:  # Skip self
                continue
            for rank in range(1, 6):  # Ranks 1-5
                if action_id == current_id:
                    # Find corresponding card indices
                    card_indices = self._get_card_indices_for_rank(player, rank)
                    return (2, player, 0, -1, card_indices, 0, rank)  # RANK_HINT
                current_id += 1
        
        # Play actions
        for card_idx in range(cards_per_hand):
            if action_id == current_id:
                return (3, -1, 0, card_idx, [], 0, 0)  # PLAY
            current_id += 1
        
        # Discard actions
        for card_idx in range(cards_per_hand):
            if action_id == current_id:
                return (4, -1, 0, card_idx, [], 0, 0)  # DISCARD
            current_id += 1
        
        # Default return invalid action
        return (5, -1, 0, -1, [], 0, 0)  # INVALID_MOVE
    
    def _action_tuple_to_id(self, action_tuple) -> int:
        """
        Convert action tuple to action ID
        """
        if not action_tuple or len(action_tuple) < 7:
            return 0
        
        move_type, to_player, from_player, card_index, card_indices, color, rank = action_tuple
        
        cards_per_hand = 5 if self.num_players <= 3 else 4
        current_id = 1
        
        # Color hint actions
        for player in range(self.num_players):
            if player == 0:  # Skip self
                continue
            for c in range(1, 6):  # Colors 1-5
                if move_type == 1 and to_player == player and color == c:
                    return current_id
                current_id += 1
        
        # Rank hint actions
        for player in range(self.num_players):
            if player == 0:  # Skip self
                continue
            for r in range(1, 6):  # Ranks 1-5
                if move_type == 2 and to_player == player and rank == r:
                    return current_id
                current_id += 1
        
        # Play actions
        for card_idx in range(cards_per_hand):
            if move_type == 3 and card_index == card_idx:
                return current_id
            current_id += 1
        
        # Discard actions
        for card_idx in range(cards_per_hand):
            if move_type == 4 and card_index == card_idx:
                return current_id
            current_id += 1
        
        return 0  # Invalid action
    
    def _get_card_indices_for_color(self, player_id: int, color: int) -> List[int]:
        """
        Get card indices for specified player and color
        """
        if self.current_state is None:
            return []
        
        indices = []
        for i, card in enumerate(self.current_state.hands[player_id]):
            if card[0] == self._color_to_string(color):
                indices.append(i)
        return indices
    
    def _get_card_indices_for_rank(self, player_id: int, rank: int) -> List[int]:
        """
        Get card indices for specified player and rank
        """
        if self.current_state is None:
            return []
        
        indices = []
        for i, card in enumerate(self.current_state.hands[player_id]):
            if card[1] == rank:
                indices.append(i)
        return indices
    
    def _color_to_string(self, color_id: int) -> str:
        """
        Convert color ID to string
        """
        color_map = {1: "red", 2: "white", 3: "yellow", 4: "green", 5: "blue"}
        return color_map.get(color_id, "red")
    
    def _is_color_hint_action(self, action_tuple) -> bool:
        """
        Check if action is a color hint action
        """
        if not action_tuple or len(action_tuple) < 1:
            return False
        return action_tuple[0] == 1  # COL_HINT