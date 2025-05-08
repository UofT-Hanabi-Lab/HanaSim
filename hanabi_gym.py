import os
import sys
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), "build"))

import gymnasium as gym
from gymnasium import spaces
import numpy as np
import random
import hana_sim

def print_observation_structure(obs):
    """Print the structure and content of the observation."""
    print("\n=== Observation Structure ===")
    
    # Print hands
    print("\nHands:")
    for i, hand in enumerate(obs.hands):
        print(f"Player {i}'s hand:", end=" ")
        for card in hand:
            print(f"{card[0]} {card[1]},", end=" ")
        print()
    
    # Print card knowledge
    try:
        print("\nPossible Colors:")
        for i, player_colors in enumerate(obs.possible_colors):
            print(f"Player {i}:")
            for j, card_colors in enumerate(player_colors):
                print(f"  Card {j}:", [idx for idx, possible in enumerate(card_colors) if possible])
        
        print("\nPossible Ranks:")
        for i, player_ranks in enumerate(obs.possible_ranks):
            print(f"Player {i}:")
            for j, card_ranks in enumerate(player_ranks):
                print(f"  Card {j}:", [idx for idx, possible in enumerate(card_ranks) if possible])
    except Exception as e:
        print("\nCard knowledge error:", str(e))
    
    # Print fireworks state
    print("\nFireworks:", dict(obs.fireworks))
    
    # Print game state
    print("\nGame State:")
    print(f"Hint tokens: {obs.hint_tokens}")
    print(f"Lives remaining: {obs.lives_remaining}")
    print(f"Deck size: {obs.deck_size}")
    
    # Print discards
    print("\nDiscards:")
    for card in obs.discards:
        print(f"{card[0]} {card[1]},", end=" ")
    print()
    
    # # Print legal actions
    # print("\nLegal Actions:")
    # for action in obs.legal_actions:
    #     print(action)
    
    print(f"\nCurrent player: {obs.current_player_id}")
    print("\n=========================")

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

    # Convert card knowledge
    possible_colors_np = np.array(raw_dict["possible_colors"], dtype=np.bool_)
    possible_ranks_np = np.array(raw_dict["possible_ranks"], dtype=np.bool_)

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

    # Convert vector encoding
    vector_encoding_np = np.array(raw_dict["vector_encoding"], dtype=np.int32)

    # Assemble the final structured observation
    structured_observation = {
        "hands": hands_np,
        "possible_colors": possible_colors_np,
        "possible_ranks": possible_ranks_np,
        "fireworks": fireworks_np,
        "hint_tokens": raw_dict["hint_tokens"],
        "lives_remaining": raw_dict["lives_remaining"],
        "deck_size": raw_dict["deck_size"],
        "discards": discards_np,
        "legal_actions": legal_actions_np,
        "current_player_id": raw_dict["current_player_id"],
        "vector_encoding": vector_encoding_np
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
    
    # card knowledge
    try:
        possible_colors_py = obs.possible_colors
        possible_ranks_py = obs.possible_ranks
    except AttributeError:
        possible_colors_py = None
        possible_ranks_py = None
    
    # vector encoding
    try:
        vector_encoding_py = obs.vector_encoding
    except AttributeError:
        vector_encoding_py = None
    
    # assemble a dictionary
    observation_dict = {
        "hands": hands_py,
        "possible_colors": possible_colors_py,
        "possible_ranks": possible_ranks_py,
        "fireworks": fireworks_py,
        "hint_tokens": obs.hint_tokens,
        "lives_remaining": obs.lives_remaining,
        "deck_size": obs.deck_size,
        "discards": discards_py,
        "legal_actions": legal_actions_py,
        "current_player_id": obs.current_player_id,
        "vector_encoding": vector_encoding_py
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

def convert_action(rand_action):
    first_part = tuple(rand_action[:4]) 
    last_past = tuple(rand_action[4:6]) 
    filtered_list = [x for x in rand_action[6:] if x != -1] 
    
    return first_part + (filtered_list,) + last_past  # Construct the final tuple

def explain_action(action):
    """Convert action tuple to natural language explanation."""
    # Convert numpy array to list if needed
    if hasattr(action, 'tolist'):
        action = action.tolist()
    
    # Parse the action array
    # Format: [move_type, to_player, from_player, card_index, color/rank, second_value, *card_indices, padding...]
    move_type = action[0]
    to_player = action[1]
    from_player = action[2]
    card_index = action[3]
    
    if move_type == 1:  # COL_HINT
        color = action[4]  # For color hint, this is the color
        # Card indices are all non -1 values after index 6
        card_indices = [i for i in action[6:] if i != -1]
        cards_str = ", ".join([str(i + 1) for i in card_indices])
        # Color mapping (based on the enum in card.h)
        color_names = {1: "red", 2: "white", 3: "yellow", 4: "green", 5: "blue"}
        return f"Player {from_player} gives Color Hint to player {to_player} that card(s) {cards_str} {'is' if len(card_indices) == 1 else 'are'} {color_names[color]}"
    
    elif move_type == 2:  # RANK_HINT
        rank = action[5]  # For rank hint, this is the rank
        # Card indices are all non -1 values after index 6
        card_indices = [i for i in action[6:] if i != -1]
        cards_str = ", ".join([str(i + 1) for i in card_indices])
        return f"Player {from_player} gives Rank Hint to player {to_player} that card(s) {cards_str} {'is' if len(card_indices) == 1 else 'are'} rank {rank}"
    
    elif move_type == 3:  # PLAY
        return f"Player {from_player} plays card {card_index + 1}"
    
    elif move_type == 4:  # DISCARD
        return f"Player {from_player} discards card {card_index + 1}"
    
    return "Invalid move"

def convert_to_standard_json(obs, last_action={"player_id": -1}):
    """
    Convert observation to standard JSON format similar to test.json
    
    Args:
        obs: The observation from the environment
        
    Returns:
        A tuple of two dictionaries in standard JSON format, for player 0 and player 1 respectively
    """
    # Keep the original print for debugging
    print(obs)
    
    # Color mapping (based on enum: enum Color { red = 1, white, yellow, green, blue, invalid_color })
    color_map = {1: "R", 2: "W", 3: "Y", 4: "G", 5: "B"}
    inv_color_map = {"R": 0, "W": 1, "Y": 2, "G": 3, "B": 4}  # For fireworks indexing
    
    # Current player ID (original)
    current_player_id = int(obs["current_player_id"])
    
    # Create JSON views for both players
    player_views = {}
    
    for player_id in [0, 1]:
        # Create info part
        info = {
            "life_tokens": int(obs["lives_remaining"]),
            "info_tokens": int(obs["hint_tokens"]),
            "fireworks": {
                "R": int(obs["fireworks"][0]),
                "Y": int(obs["fireworks"][2]),
                "G": int(obs["fireworks"][3]),
                "W": int(obs["fireworks"][1]),
                "B": int(obs["fireworks"][4])
            },
            "hands": {
                "cur_player": [],
                "others": []
            },
            "deck_size": int(obs["deck_size"]),
            "discards": []
        }
        
        # Determine other player ID
        other_player_id = 1 if player_id == 0 else 0
        
        # Process cur_player's hand (current player cannot see their own cards)
        cur_player_hand = []
        for i in range(5):  # Assume each player has 5 cards
            card_info = {
                "index": i, 
                "card": "XX",  # Current player cannot see their own cards
                "info": ""
            }
            
            # Process color information - only include possible colors
            possible_colors = []
            for c in range(1, 6):  # Color range 1-5
                if obs["possible_colors"][player_id][i][c]:
                    possible_colors.append(color_map[c])
            
            # Process number information - only include possible numbers
            possible_ranks = []
            for r in range(1, 6):  # Number range 1-5
                if obs["possible_ranks"][player_id][i][r]:
                    possible_ranks.append(str(r))
            
            # Determine info
            color_info = ""
            if len(possible_colors) == 1:
                color_info = possible_colors[0]
            else:
                color_info = "X"
                
            rank_info = ""
            if len(possible_ranks) == 1:
                rank_info = possible_ranks[0]
            else:
                rank_info = "X"
                
            card_info["info"] = color_info + rank_info
            
            # Only include possible colors
            card_info["col"] = possible_colors if possible_colors else ["R", "Y", "G", "W", "B"]
            
            # Only include possible numbers
            card_info["rank"] = possible_ranks if possible_ranks else ["1", "2", "3", "4", "5"]
            
            cur_player_hand.append(card_info)
        
        # Process other player's hand (can see the other player's cards)
        other_player_hand = []
        for i in range(5):  # Assume each player has 5 cards
            try:
                color_id = int(obs["hands"][other_player_id][i][0])
                rank = int(obs["hands"][other_player_id][i][1])
                
                if color_id >= 1 and color_id <= 5 and rank >= 1 and rank <= 5:
                    card_val = color_map[color_id] + str(rank)
                else:
                    card_val = "XX"  # Use default value
                
                card_info = {
                    "index": i,
                    "card": card_val,
                    "info": ""
                }
                
                # Process color information - only include possible colors
                possible_colors = []
                for c in range(1, 6):  # Color range 1-5
                    if obs["possible_colors"][other_player_id][i][c]:
                        possible_colors.append(color_map[c])
                
                # Process number information - only include possible numbers
                possible_ranks = []
                for r in range(1, 6):  # Number range 1-5
                    if obs["possible_ranks"][other_player_id][i][r]:
                        possible_ranks.append(str(r))
                
                # Determine info
                color_info = ""
                if len(possible_colors) == 1:
                    color_info = possible_colors[0]
                else:
                    color_info = "X"
                    
                rank_info = ""
                if len(possible_ranks) == 1:
                    rank_info = possible_ranks[0]
                else:
                    rank_info = "X"
                    
                card_info["info"] = color_info + rank_info
                
                # Only include possible colors
                card_info["col"] = possible_colors if possible_colors else ["R", "Y", "G", "W", "B"]
                
                # Only include possible numbers
                card_info["rank"] = possible_ranks if possible_ranks else ["1", "2", "3", "4", "5"]
                
                other_player_hand.append(card_info)
            except Exception as e:
                print(f"Error processing other player's card at index {i}: {e}")
                # Add a default card
                other_player_hand.append({
                    "index": i,
                    "card": "XX",
                    "info": "XX",
                    "col": ["R", "Y", "G", "W", "B"],
                    "rank": ["1", "2", "3", "4", "5"]
                })
        
        # Add hand to info
        info["hands"]["cur_player"] = cur_player_hand
        info["hands"]["others"] = other_player_hand
        
        # Process deck information
        discards = []
        for i in range(len(obs["discards"])):
            try:
                color_id, rank = obs["discards"][i]
                color_id = int(color_id)
                rank = int(rank)
                if color_id != -1 and rank != -1 and color_id in color_map:  # Ensure valid discard
                    discards.append(f"{color_map[color_id]}{rank}")
            except Exception as e:
                print(f"Error processing discard at index {i}: {e}")
        
        info["discards"] = discards
        
        # Process legal actions, convert them to expected format according to test.json
        actions = {}
        
        # Only current player can execute action
        if player_id == current_player_id:
            for i, action in enumerate(obs["legal_actions"]):
                try:
                    if action[0] == -1:  # Skip invalid action
                        continue
                        
                    move_type = int(action[0])
                    action_info = {}
                    
                    if move_type == 3:  # PLAY
                        action_info = {
                            "action_type": "PLAY",
                            "card_index": int(action[3])
                        }
                    elif move_type == 4:  # DISCARD
                        action_info = {
                            "action_type": "DISCARD",
                            "card_index": int(action[3])
                        }
                    elif move_type == 1:  # COLOR HINT
                        # Get color value, ensure it exists in color_map
                        color_value = int(action[4])
                        if color_value in color_map:
                            action_info = {
                                "action_type": "REVEAL_COLOR",
                                "target_offset": int(action[1]),
                                "color": color_map[color_value]
                            }
                        else:
                            print(f"Warning: Invalid color value {color_value} in action {i}")
                            continue
                    elif move_type == 2:  # RANK HINT
                        # Get rank value
                        rank_value = int(action[5]) 
                        if 1 <= rank_value <= 5:
                            action_info = {
                                "action_type": "REVEAL_RANK",
                                "target_offset": int(action[1]),
                                "rank": rank_value-1
                            }
                        else:
                            print(f"Warning: Invalid rank value {rank_value} in action {i}")
                            continue
                    if action_info:
                        actions[i] = action_info
                except Exception as e:
                    print(f"Error processing action at index {i}: {e}")
        
        # Build final result
        result = {
            "info": info,
            "actions": actions,
            "last_action": last_action,  # Initial state, last action player ID is -1
            "player_id": player_id,
            "waiting": "Your turn to play" if player_id == current_player_id else "Waiting for other player"
        }
        
        player_views[player_id] = result
    
    # Save JSON views for both players
    import json
    with open('player0_obs.json', 'w') as f:
        json.dump(player_views[0], f, indent=4)
    
    with open('player1_obs.json', 'w') as f:
        json.dump(player_views[1], f, indent=4)
    
    # Current player's view still saved to current_obs.json
    with open('current_obs.json', 'w') as f:
        json.dump(player_views[current_player_id], f, indent=4)
    
    print(f"Generated JSON for both players. Current player is {current_player_id}")
    print(player_views[0])
    print(player_views[1])
    # Return JSON views for both players
    return player_views[0], player_views[1]

def start_server(host="0.0.0.0", port=8000):
    """
    Start server, connect FastAPI with HanabiGym environment
    
    Args:
        host: Server host name
        port: Server port
    """
    from fastapi import FastAPI, WebSocket
    import uvicorn
    import json
    import uuid
    import threading
    import asyncio
    from datetime import datetime
    import os
    
    # Define HanabiWebAgent class
    class HanabiWebAgent:
        def __init__(self):
            self.action_queue = asyncio.Queue()
            self.websocket = None
            self.player_id = -1
            self.last_action = {"player_id": -1}
        
        def set_player_id(self, player_id):
            self.player_id = player_id
        
        def set_websocket(self, websocket):
            self.websocket = websocket
        
        async def send_observation(self, observation_data):
            if self.websocket:
                await self.websocket.send_json(observation_data)
        
        async def receive_action(self, action_idx):
            await self.action_queue.put(action_idx)
        
        async def act(self, observation):
            # Use convert_to_standard_json function to create JSON data
            player0_obs, player1_obs = convert_to_standard_json(observation, self.last_action)
            
            # Send corresponding observation data based on player ID
            player_obs = player0_obs if self.player_id == 0 else player1_obs
            await self.send_observation(player_obs)
            
            # Wait for user input action
            action_idx = await self.action_queue.get()
            
            # Get user selected action from legal_actions
            action = observation["legal_actions"][action_idx]
            
            # Update last action
            self.last_action = {
                "player_id": self.player_id,
                "action": action_idx
            }
            
            return convert_action(action)
        
        async def update_observation(self, observation, waiting_message):
            # Use convert_to_standard_json function to create JSON data
            player0_obs, player1_obs = convert_to_standard_json(observation, self.last_action)
            
            # Send corresponding observation data and update waiting message
            player_obs = player0_obs if self.player_id == 0 else player1_obs
            player_obs["waiting"] = waiting_message
            
            await self.send_observation(player_obs)
    
    # Game runner
    class HanabiRunner:
        def __init__(self, agents):
            self.env = MyHanabiEnv(2)  # Create Hanabi environment, 2 players
            self.agents = agents
            self.cache = {"time": datetime.now().strftime("%Y-%m-%d %H:%M:%S"), "game_record": []}
        
        async def run(self):
            # Reset environment and get initial observation
            obs = self.env.reset()
            done = False
            reward = 0
            episode_reward = 0
            info = None
            
            # Update second player's observation, inform waiting
            if len(self.agents) > 1:
                await self.agents[1].update_observation(obs, "Waiting for opponent to play")
            
            while not done:
                # Determine current player
                current_player_id = obs["current_player_id"]
                
                # Current player execute action
                agent = self.agents[current_player_id]
                action = await agent.act(obs)
                
                # Record game state
                self.cache['game_record'].append({
                    'last_action': agent.last_action,
                    'state': str(obs)
                })
                
                # Execute action and get new observation
                obs, reward, done, info = self.env.step(action)
                episode_reward += reward
                
                # Determine new current player
                new_current_player = obs["current_player_id"]
                
                # Update all players' observations
                for i, agent in enumerate(self.agents):
                    if i == new_current_player:
                        await agent.update_observation(obs, "Your turn to play")
                    else:
                        await agent.update_observation(obs, "Waiting for opponent to play")
            
            # Record last state
            self.cache['game_record'].append({
                'last_action': self.agents[current_player_id].last_action,
                'state': str(obs)
            })
            
            # Save game log
            log_dir = os.path.join(os.getcwd(), "game_log")
            os.makedirs(log_dir, exist_ok=True)
            timestamp = datetime.now().strftime("%Y%m%d%H%M%S")
            log_file = os.path.join(log_dir, f"{timestamp}.json")
            with open(log_file, "w") as f:
                json.dump(self.cache, f, indent=4)
            
            # Notify players game end
            for agent in self.agents:
                await agent.send_observation({
                    'event': f'game end with score {episode_reward}'
                })
            
            return episode_reward
    
    # Set room management
    rooms = {}
    
    # Create FastAPI application
    app = FastAPI()
    
    @app.websocket("/ws")
    async def websocket_endpoint(websocket: WebSocket):
        await websocket.accept()
        agent = HanabiWebAgent()
        agent.set_websocket(websocket)
        current_room_id = None
        
        try:
            while True:
                data = await websocket.receive_text()
                try:
                    message = json.loads(data)
                except json.JSONDecodeError:
                    await websocket.send_json({"status": "error", "message": "Invalid JSON"})
                    continue
                
                command = message.get("command")
                if command == "create_room":
                    # Create room, default to waiting for other players to join
                    room_id = str(uuid.uuid4())
                    current_room_id = room_id
                    agent.set_player_id(0)
                    rooms[room_id] = {'players': [agent], 'mode': 'human'}
                    
                    await websocket.send_json({
                        "status": "room_created",
                        "room_id": room_id,
                        "mode": "human"
                    })
                    print(f"Room {room_id} created, waiting for another player to join.")
                
                elif command == "join_room":
                    # Join specified room
                    room_id = message.get("room_id")
                    if room_id in rooms:
                        room = rooms[room_id]
                        agent.set_player_id(1)
                        room['players'].append(agent)
                        current_room_id = room_id
                        
                        await websocket.send_json({
                            "status": "joined_room",
                            "room_id": room_id
                        })
                        print(f"A player joined room {room_id}, starting the game.")
                        
                        # Start game
                        players = room['players']
                        del rooms[room_id]  # Remove room record before game starts
                        
                        # Create and run game
                        runner = HanabiRunner(players)
                        asyncio.create_task(runner.run())
                    else:
                        await websocket.send_json({"status": "error", "message": "Room does not exist"})
                
                elif "action" in message:
                    # Process player action
                    try:
                        action_idx = int(message["action"])
                        print(f"Received action {action_idx} from room {current_room_id}")
                        await agent.receive_action(action_idx)
                    except ValueError:
                        await websocket.send_json({"status": "error", "message": "Invalid action value"})
                
                else:
                    await websocket.send_json({"status": "error", "message": "Unknown command"})
        
        except Exception as e:
            print(f"WebSocket connection error: {e}")
        finally:
            print(f"Connection closed, associated room: {current_room_id}")
    
    # Start server
    uvicorn.run(app, host=host, port=port)

if __name__ == "__main__":
    env = MyHanabiEnv(2)
    env.render()
    
    obs = env.reset()
    
    while True:
        ac_len = len(obs["legal_actions"])
        # Print all legal actions and let user choose
        print("\nAvailable actions:")
        print(obs["vector_encoding"])
        for i, action in enumerate(obs["legal_actions"]):
            print(f"[{i}]: {explain_action(action)}")

        convert_to_standard_json(obs)
        
        # Get user input
        while True:
            try:
                choice = int(input("\nChoose action index: "))
                if 0 <= choice < ac_len:
                    break
                print(f"Please enter a number between 0 and {ac_len-1}")
            except ValueError:
                print("Please enter a valid number")
        
        chosen_action = obs["legal_actions"][choice]
        print("Executing action:", explain_action(chosen_action))
        
        obs, reward, done, info = env.step(convert_action(chosen_action))
        
        print(f"Current score: {info}")
        
        if done:
            break
        
        print("\n" * 3) 