import time
import numpy as np
from typing import List, Dict, Any
import rela

class CustomHanabiThreadLoop(rela.ThreadLoop):
    """
    自定义的Hanabi线程循环，用于处理自定义Hanabi环境
    """
    
    def __init__(self, envs, actors, eval_mode=False):
        """
        初始化线程循环
        
        Args:
            envs: 环境列表
            actors: 演员列表
            eval_mode: 是否为评估模式
        """
        super().__init__()
        self.envs = envs
        self.actors = actors
        self.eval_mode = eval_mode
        self.running = False
        
        # 验证输入
        assert len(envs) == len(actors), f"Number of envs ({len(envs)}) != number of actor groups ({len(actors)})"
        
        # 初始化环境
        for env in envs:
            env.reset()
        
        print(f"CustomHanabiThreadLoop initialized with {len(envs)} environments")
    
    def run(self):
        """
        运行线程循环
        """
        self.running = True
        print("CustomHanabiThreadLoop started")
        
        while self.running:
            try:
                # 处理每个环境
                for env_idx, (env, actor_group) in enumerate(zip(self.envs, self.actors)):
                    if not self.running:
                        break
                    
                    # 检查环境是否结束
                    if env.terminated():
                        # 重置环境
                        env.reset()
                        continue
                    
                    # 获取当前玩家
                    current_player = env.get_current_player()
                    
                    # 确保有对应的演员
                    if current_player >= len(actor_group):
                        print(f"Warning: No actor for player {current_player}")
                        continue
                    
                    actor = actor_group[current_player]
                    
                    # 获取观察
                    obs = self._get_observation(env, current_player)
                    
                    # 让演员选择动作
                    action = self._get_action(actor, obs)
                    
                    # 执行动作
                    env.step(action)
                    
                    # 短暂休息以避免过度占用CPU
                    time.sleep(0.001)
                
            except Exception as e:
                print(f"Error in CustomHanabiThreadLoop: {e}")
                import traceback
                traceback.print_exc()
                time.sleep(0.1)
    
    def pause(self):
        """
        暂停线程循环
        """
        self.running = False
        print("CustomHanabiThreadLoop paused")
    
    def resume(self):
        """
        恢复线程循环
        """
        self.running = True
        print("CustomHanabiThreadLoop resumed")
    
    def _get_observation(self, env, player_id):
        """
        获取指定玩家的观察
        
        Args:
            env: 环境
            player_id: 玩家ID
            
        Returns:
            观察数据
        """
        # 这里需要根据你的观察格式来调整
        # 假设环境有一个方法来获取特定玩家的观察
        try:
            # 尝试获取特定玩家的观察
            if hasattr(env, 'get_observation_for_player'):
                return env.get_observation_for_player(player_id)
            else:
                # 如果没有特定方法，返回当前状态的观察
                return env.current_state
        except Exception as e:
            print(f"Error getting observation for player {player_id}: {e}")
            return None
    
    def _get_action(self, actor, obs):
        """
        从演员获取动作
        
        Args:
            actor: 演员
            obs: 观察
            
        Returns:
            动作
        """
        try:
            # 调用演员的act方法
            if hasattr(actor, 'act'):
                action = actor.act(obs)
                return action
            else:
                # 如果没有act方法，尝试其他可能的方法名
                for method_name in ['get_action', 'select_action', 'decide_action']:
                    if hasattr(actor, method_name):
                        method = getattr(actor, method_name)
                        action = method(obs)
                        return action
                
                # 如果都没有，返回随机动作
                print("Warning: Actor has no action method, using random action")
                return self._get_random_action(obs)
                
        except Exception as e:
            print(f"Error getting action from actor: {e}")
            return self._get_random_action(obs)
    
    def _get_random_action(self, obs):
        """
        获取随机动作（作为后备）
        
        Args:
            obs: 观察
            
        Returns:
            随机动作
        """
        # 这里需要根据你的动作空间来生成随机动作
        # 示例：返回一个默认动作
        return (1, -1, 0, -1, [], 0, 0)  # 示例动作格式
    
    def get_scores(self):
        """
        获取所有环境的分数
        
        Returns:
            分数列表
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
        获取所有游戏的步数
        
        Returns:
            步数列表
        """
        return [env.get_step() for env in self.envs] 