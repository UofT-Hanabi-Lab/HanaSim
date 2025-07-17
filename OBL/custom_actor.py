"""
Custom R2D2Actor implementation to replace hanalearn.R2D2Actor
"""

import numpy as np
import torch
from typing import List, Dict, Any, Optional
import rela


class CustomR2D2Actor:
    """
    Custom R2D2Actor to replace hanalearn.R2D2Actor
    """
    
    def __init__(
        self,
        model_runner,
        seed: int,
        num_player: int,
        player_idx: int,
        explore_eps: List[float],
        boltzmann_t: List[float],
        vdn: bool,
        sad: bool,
        shuffle_color: bool,
        hide_action: bool,
        trinary: bool,
        replay_buffer,
        multi_step: int,
        max_len: int,
        gamma: float,
    ):
        """
        Initialize Custom R2D2Actor
        
        Args:
            model_runner: Model runner for inference
            seed: Random seed
            num_player: Number of players
            player_idx: This player's index
            explore_eps: Exploration epsilon values
            boltzmann_t: Boltzmann temperature values
            vdn: Whether using VDN method
            sad: Whether using SAD (self-attention decoder)
            shuffle_color: Whether to shuffle colors
            hide_action: Whether to hide actions
            trinary: Whether using trinary encoding
            replay_buffer: Replay buffer for storing experiences
            multi_step: Multi-step learning steps
            max_len: Maximum episode length
            gamma: Discount factor
        """
        self.model_runner = model_runner
        self.seed = seed
        self.num_player = num_player
        self.player_idx = player_idx
        self.explore_eps = explore_eps if isinstance(explore_eps, list) else [explore_eps]
        self.boltzmann_t = boltzmann_t if isinstance(boltzmann_t, list) else [boltzmann_t]
        self.vdn = vdn
        self.sad = sad
        self.shuffle_color = shuffle_color
        self.hide_action = hide_action
        self.trinary = trinary
        self.replay_buffer = replay_buffer
        self.multi_step = multi_step
        self.max_len = max_len
        self.gamma = gamma
        
        # Internal state
        self.rng = np.random.RandomState(seed)
        self.partners = []
        self.belief_runner = None
        self.current_eps = self.explore_eps[0] if self.explore_eps else 0.1
        self.current_temp = self.boltzmann_t[0] if self.boltzmann_t else 1.0
        
        # Episode data
        self.episode_data = []
        self.last_obs = None
        self.last_action = None
        self.step_count = 0
        
        print(f"CustomR2D2Actor initialized: player_idx={player_idx}, vdn={vdn}")
    
    def act(self, obs) -> int:
        """
        Choose action based on observation
        
        Args:
            obs: Observation from environment
            
        Returns:
            Action ID
        """
        try:
            # Convert observation to tensor format expected by model
            obs_tensor = self._convert_obs_to_tensor(obs)
            
            # Get action from model
            if self.model_runner is not None:
                # Use model runner for inference
                action_logits = self._get_model_output(obs_tensor)
                action = self._sample_action(action_logits, obs)
            else:
                # Fallback to random action
                action = self._get_random_action(obs)
            
            # Store experience for replay buffer
            if self.replay_buffer is not None and self.last_obs is not None:
                self._store_experience(self.last_obs, self.last_action, obs)
            
            # Update state
            self.last_obs = obs
            self.last_action = action
            self.step_count += 1
            
            return action
            
        except Exception as e:
            print(f"Error in CustomR2D2Actor.act: {e}")
            return self._get_random_action(obs)
    
    def set_partners(self, partners: List):
        """
        Set partner actors for multi-agent learning
        
        Args:
            partners: List of partner actors (None for self)
        """
        self.partners = partners
    
    def set_belief_runner(self, belief_runner):
        """
        Set belief model runner for off-belief learning
        
        Args:
            belief_runner: Belief model runner
        """
        self.belief_runner = belief_runner
    
    def get_success_fict_rate(self) -> float:
        """
        Get success rate for fictitious state sampling
        
        Returns:
            Success rate (placeholder implementation)
        """
        return 0.8  # Placeholder value
    
    def _convert_obs_to_tensor(self, obs) -> Dict[str, torch.Tensor]:
        """
        Convert observation to tensor format expected by model
        
        Args:
            obs: Raw observation
            
        Returns:
            Tensorized observation
        """
        try:
            if hasattr(obs, 'vector_encoding'):
                # Use vector encoding if available
                encoding = torch.FloatTensor(obs.vector_encoding).unsqueeze(0)
            elif hasattr(obs, '__dict__'):
                # Convert observation attributes to tensor
                encoding = self._obs_to_vector(obs)
                encoding = torch.FloatTensor(encoding).unsqueeze(0)
            else:
                # Fallback: create dummy encoding
                encoding = torch.zeros(1, 658)  # Standard Hanabi encoding size
            
            # Create tensor dictionary
            tensor_obs = {
                'obs': encoding,
                'legal_move': self._get_legal_moves_tensor(obs),
                'temperature': torch.FloatTensor([self.current_temp]),
            }
            
            return tensor_obs
            
        except Exception as e:
            print(f"Error converting observation to tensor: {e}")
            # Return dummy tensors
            return {
                'obs': torch.zeros(1, 658),
                'legal_move': torch.ones(1, 21),  # Assume all actions legal
                'temperature': torch.FloatTensor([self.current_temp]),
            }
    
    def _obs_to_vector(self, obs) -> List[float]:
        """
        Convert observation to vector encoding
        
        Args:
            obs: Observation object
            
        Returns:
            Vector encoding
        """
        # This is a simplified implementation
        # In practice, you'd need to implement proper observation encoding
        encoding = []
        
        # Add hand information
        if hasattr(obs, 'hands'):
            for hand in obs.hands:
                for card in hand:
                    if isinstance(card, (list, tuple)) and len(card) >= 2:
                        encoding.extend([card[0], card[1]])  # color, rank
                    else:
                        encoding.extend([0, 0])  # default values
        
        # Add board state
        if hasattr(obs, 'fireworks'):
            if isinstance(obs.fireworks, dict):
                for color in ['red', 'white', 'yellow', 'green', 'blue']:
                    encoding.append(obs.fireworks.get(color, 0))
            elif isinstance(obs.fireworks, list):
                encoding.extend(obs.fireworks)
        
        # Add other features
        if hasattr(obs, 'hint_tokens'):
            encoding.append(obs.hint_tokens)
        if hasattr(obs, 'lives_remaining'):
            encoding.append(obs.lives_remaining)
        if hasattr(obs, 'deck_size'):
            encoding.append(obs.deck_size)
        
        # Pad to standard size
        target_size = 658  # Standard Hanabi encoding size
        while len(encoding) < target_size:
            encoding.append(0.0)
        
        return encoding[:target_size]
    
    def _get_legal_moves_tensor(self, obs) -> torch.Tensor:
        """
        Get legal moves as tensor
        
        Args:
            obs: Observation
            
        Returns:
            Legal moves tensor
        """
        try:
            if hasattr(obs, 'legal_actions'):
                # Create mask based on legal actions
                num_actions = 21  # Standard action space size
                legal_mask = torch.zeros(1, num_actions)
                
                for i, action in enumerate(obs.legal_actions):
                    if i < num_actions:
                        legal_mask[0, i] = 1.0
                
                return legal_mask
            else:
                # Default: all actions legal
                return torch.ones(1, 21)
                
        except Exception as e:
            print(f"Error getting legal moves: {e}")
            return torch.ones(1, 21)
    
    def _get_model_output(self, obs_tensor: Dict[str, torch.Tensor]) -> torch.Tensor:
        """
        Get model output for action selection
        
        Args:
            obs_tensor: Tensorized observation
            
        Returns:
            Action logits
        """
        try:
            if hasattr(self.model_runner, 'call'):
                # Use model runner
                output = self.model_runner.call("act", obs_tensor)
                if isinstance(output, dict) and 'action' in output:
                    return output['action']
                else:
                    return output
            else:
                # Fallback: random logits
                return torch.randn(1, 21)
                
        except Exception as e:
            print(f"Error getting model output: {e}")
            return torch.randn(1, 21)
    
    def _sample_action(self, action_logits: torch.Tensor, obs) -> int:
        """
        Sample action from logits
        
        Args:
            action_logits: Action logits from model
            obs: Observation
            
        Returns:
            Sampled action ID
        """
        try:
            # Apply exploration
            if self.rng.random() < self.current_eps:
                # Random exploration
                return self._get_random_action(obs)
            
            # Boltzmann sampling if temperature > 0
            if len(self.boltzmann_t) > 0 and self.current_temp > 0:
                probs = torch.softmax(action_logits / self.current_temp, dim=-1)
                action = torch.multinomial(probs, 1).item()
            else:
                # Greedy action
                action = torch.argmax(action_logits, dim=-1).item()
            
            return action
            
        except Exception as e:
            print(f"Error sampling action: {e}")
            return self._get_random_action(obs)
    
    def _get_random_action(self, obs) -> int:
        """
        Get random legal action
        
        Args:
            obs: Observation
            
        Returns:
            Random action ID
        """
        try:
            if hasattr(obs, 'legal_actions') and len(obs.legal_actions) > 0:
                return self.rng.randint(0, len(obs.legal_actions))
            else:
                return self.rng.randint(0, 21)  # Standard action space
                
        except Exception as e:
            print(f"Error getting random action: {e}")
            return 0
    
    def _store_experience(self, last_obs, last_action, current_obs):
        """
        Store experience in replay buffer
        
        Args:
            last_obs: Previous observation
            last_action: Previous action
            current_obs: Current observation
        """
        try:
            if self.replay_buffer is not None:
                # Create experience tuple
                # This is a simplified implementation
                experience = {
                    'obs': last_obs,
                    'action': last_action,
                    'reward': 0.0,  # Would need to get from environment
                    'next_obs': current_obs,
                    'done': False,
                }
                
                # Add to replay buffer (implementation depends on buffer interface)
                if hasattr(self.replay_buffer, 'add'):
                    self.replay_buffer.add(experience)
                    
        except Exception as e:
            print(f"Error storing experience: {e}")


class MockModelRunner:
    """
    Mock model runner for testing when no real model is available
    """
    
    def __init__(self, device='cpu'):
        self.device = device
    
    def call(self, method: str, input_data: Dict[str, torch.Tensor]) -> torch.Tensor:
        """
        Mock call to model
        
        Args:
            method: Method name
            input_data: Input data
            
        Returns:
            Mock output
        """
        if method == "act":
            # Return random logits
            batch_size = input_data['obs'].shape[0] if 'obs' in input_data else 1
            return torch.randn(batch_size, 21)
        else:
            return torch.randn(1, 21)
    
    def start(self):
        """Mock start method"""
        pass
    
    def update_model(self, model):
        """Mock update model method"""
        pass 