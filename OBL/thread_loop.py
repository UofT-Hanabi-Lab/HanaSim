import time
import numpy as np
from typing import List, Dict, Any
import rela

class CustomHanabiThreadLoop(rela.ThreadLoop):
    """
    Custom Hanabi thread loop for handling custom Hanabi environments
    """
    
    def __init__(self, envs, actors, eval_mode=False):
        """
        Initialize thread loop
        
        Args:
            envs: List of environments
            actors: List of actors
            eval_mode: Whether in evaluation mode
        """
        super().__init__()
        self.envs = envs
        self.actors = actors
        self.eval_mode = eval_mode
        self.running = False
        
        # Validate inputs
        assert len(envs) == len(actors), f"Number of envs ({len(envs)}) != number of actor groups ({len(actors)})"
        
        # Initialize environments
        for env in envs:
            env.reset()
        
        print(f"CustomHanabiThreadLoop initialized with {len(envs)} environments")
    
    def run(self):
        """
        Run thread loop
        """
        self.running = True
        print("CustomHanabiThreadLoop started")
        
        while self.running:
            try:
                # Process each environment
                for env_idx, (env, actor_group) in enumerate(zip(self.envs, self.actors)):
                    if not self.running:
                        break
                    
                    # Check if environment is terminated
                    if env.terminated():
                        # Reset environment
                        env.reset()
                        continue
                    
                    # Get current player
                    current_player = env.get_current_player()
                    
                    # Ensure corresponding actor exists
                    if current_player >= len(actor_group):
                        print(f"Warning: No actor for player {current_player}")
                        continue
                    
                    actor = actor_group[current_player]
                    
                    # Get observation
                    obs = self._get_observation(env, current_player)
                    
                    # Let actor choose action
                    action = self._get_action(actor, obs)
                    
                    # Execute action
                    env.step(action)
                    
                    # Brief pause to avoid excessive CPU usage
                    time.sleep(0.001)
                
            except Exception as e:
                print(f"Error in CustomHanabiThreadLoop: {e}")
                import traceback
                traceback.print_exc()
                time.sleep(0.1)
    
    def pause(self):
        """
        Pause thread loop
        """
        self.running = False
        print("CustomHanabiThreadLoop paused")
    
    def resume(self):
        """
        Resume thread loop
        """
        self.running = True
        print("CustomHanabiThreadLoop resumed")
    
    def _get_observation(self, env, player_id):
        """
        Get observation for specified player
        
        Args:
            env: Environment
            player_id: Player ID
            
        Returns:
            Observation data
        """
        # This needs to be adjusted based on your observation format
        # Assume environment has a method to get specific player's observation
        try:
            # Try to get specific player's observation
            if hasattr(env, 'get_observation_for_player'):
                return env.get_observation_for_player(player_id)
            else:
                # If no specific method, return current state observation
                return env.current_state
        except Exception as e:
            print(f"Error getting observation for player {player_id}: {e}")
            return None
    
    def _get_action(self, actor, obs):
        """
        Get action from actor
        
        Args:
            actor: Actor
            obs: Observation
            
        Returns:
            Action
        """
        try:
            # Call actor's act method
            if hasattr(actor, 'act'):
                action = actor.act(obs)
                return action
            else:
                # If no act method, try other possible method names
                for method_name in ['get_action', 'select_action', 'decide_action']:
                    if hasattr(actor, method_name):
                        method = getattr(actor, method_name)
                        action = method(obs)
                        return action
                
                # If none exist, return random action
                print("Warning: Actor has no action method, using random action")
                return self._get_random_action(obs)
                
        except Exception as e:
            print(f"Error getting action from actor: {e}")
            return self._get_random_action(obs)
    
    def _get_random_action(self, obs):
        """
        Get random action (as fallback)
        
        Args:
            obs: Observation
            
        Returns:
            Random action
        """
        # This needs to generate random actions based on your action space
        # Example: return a default action
        return (1, -1, 0, -1, [], 0, 0)  # Example action format
    
    def get_scores(self):
        """
        Get scores of all environments
        
        Returns:
            List of scores
        """
        scores = []
        for env in self.envs:
            if env.terminated():
                scores.append(env.last_episode_score())
            else:
                scores.append(env.get_score())
        return scores
    
    def get_game_lengths(self):
        """
        Get step counts of all games
        
        Returns:
            List of step counts
        """
        return [env.get_step() for env in self.envs] 