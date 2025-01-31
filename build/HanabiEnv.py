import gymnasium as gym
from gymnasium import spaces
import numpy as np
import random
import hana_sim

def convert_observation_to_space(obs, num_players=2, max_cards=5, max_discards=50):
    raw_dict = convert_observation_to_dict(obs)
    # Color mapping based on the given enum
    color_map = {"red": 1, "white": 2, "yellow": 3, "green": 4, "blue": 5}
    default_card = (-1, -1)  # Placeholder for missing cards

    # Convert hands
    hands_np = np.full((num_players, max_cards, 2), default_card, dtype=np.int32)
    for player_idx, hand in enumerate(raw_dict["hands"]):
        for card_idx, (color, rank) in enumerate(hand):
            if card_idx < max_cards:
                hands_np[player_idx, card_idx] = (color_map[color], rank)

    # Convert fireworks
    fireworks_np = np.zeros(5, dtype=np.int32)
    fireworks_order = ["red", "white", "yellow", "green", "blue"]
    for idx, color in enumerate(fireworks_order):
        fireworks_np[idx] = raw_dict["fireworks"].get(color, 0)

    # Convert discards
    discards_np = np.full((max_discards, 2), default_card, dtype=np.int32)
    for idx, (color, rank) in enumerate(raw_dict["discards"]):
        if idx < max_discards:
            discards_np[idx] = (color_map[color], rank)

    # Convert legal actions
    num_actions = len(raw_dict["legal_actions"])
    max_card_inx_len = 5
    legal_actions_np = np.full((num_actions, 6 + max_card_inx_len), -1, dtype=np.int32)

    for i, action in enumerate(raw_dict["legal_actions"]):
        fixed_part = list(action[:4]) + list(action[5:]) 
        action_list = list(action[4])  

        padded_action_list = action_list + [-1] * (max_card_inx_len - len(action_list))
        legal_actions_np[i] = fixed_part + padded_action_list


    # Assemble the final structured observation
    structured_observation = {
        "hands": hands_np,
        "fireworks": fireworks_np,
        "hint_tokens": raw_dict["hint_tokens"],
        "lives_remaining": raw_dict["lives_remaining"],
        "deck_size": raw_dict["deck_size"],
        "discards": discards_np,
        "legal_actions": legal_actions_np,
        "current_player_id": raw_dict["current_player_id"]
    }

    return structured_observation

def convert_observation_to_dict(obs) -> dict:
    # hands: List[List[Tuple[str, int]]]
    hands_py = [
        [(color, rank) for (color, rank) in player_hand]
        for player_hand in obs.hands
    ]
    
    # fireworks: Dict[str, int]
    fireworks_py = dict(obs.fireworks)
    
    # discards: List[Tuple[str, int]]
    discards_py = [(color, rank) for (color, rank) in obs.discards]
    
    # legal_actions: List[Tuple[...]], ensure we convert them to Python tuples
    legal_actions_py = [tuple(action) for action in obs.legal_actions]
    
    # assemble a dictionary
    observation_dict = {
        "hands": hands_py,
        "fireworks": fireworks_py,
        "hint_tokens": obs.hint_tokens,
        "lives_remaining": obs.lives_remaining,
        "deck_size": obs.deck_size,
        "discards": discards_py,
        "legal_actions": legal_actions_py,
        "current_player_id": obs.current_player_id
    }
    
    return observation_dict

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
                                "fireworks": spaces.Box(low=0, high=5, shape=(5,), dtype=np.int32),  # (num_colors,)
                                "hint_tokens": spaces.Discrete(9),  # 0 to 8
                                "lives_remaining": spaces.Discrete(4),  # 0 to 3
                                "deck_size": spaces.Discrete(51),  # 0 to 50
                                "discards": spaces.Box(low=-1, high=5, shape=(50, 2), dtype=np.int32),  # (max_discards, 2)
                                "legal_actions": spaces.Box(low=-1, high=5, shape=(20, 11), dtype=np.int32),  # (max_legal_actions, 6 + max_action_params)
                                "current_player_id": spaces.Discrete(2)  # 0 to 1
                            })

    def reset(self):
        # Reset the environment and return the initial observation
        obs = self.env.reset()
        return self._convert_observation(obs)

    def step(self, action):
        # Execute action in the simulator and return the result
        step_result = self.env.step(action)
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

def convert_action(rand_action):
    first_part = tuple(rand_action[:4]) 
    last_past = tuple(rand_action[4:6]) 
    filtered_list = [x for x in rand_action[6:] if x != -1] 
    
    return first_part + (filtered_list,) + last_past  # Construct the final tuple


if __name__ == "__main__":
    env = MyHanabiEnv(2)
    env.render()
    
    obs = env.reset()
    print(obs)
    
    while True:
        ac_len = len(obs["legal_actions"])
        # Randomly select a legal action
        rand_action = obs["legal_actions"][random.randint(0, ac_len - 1)]
        print("Chosen action:", rand_action)
        
        new_obs, reward, done, info = env.step(convert_action(rand_action))  # Convert to tuple to match action_space
        
        print(new_obs)
        print(f"Current score: {info}")
        
        if done:
            break
        
        print("\n" * 3)

    

