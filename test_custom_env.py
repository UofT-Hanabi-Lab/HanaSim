#!/usr/bin/env python3
"""
测试自定义Hanabi环境的功能
"""

import os
import sys
import numpy as np

# Add the current directory to path
current_dir = os.path.dirname(os.path.abspath(__file__))
if current_dir not in sys.path:
    sys.path.insert(0, current_dir)

from custom_hanabi_env import CustomHanabiEnv

def test_basic_functionality():
    """测试基本功能"""
    print("=== 测试基本功能 ===")
    
    # 创建环境
    params = {
        "players": "2",
        "seed": "42",
        "bomb": "0",
        "hand_size": "5",
        "random_start_player": "1"
    }
    
    env = CustomHanabiEnv(params, max_len=80, verbose=True)
    
    # 测试基本属性
    print(f"玩家数量: {env.get_num_players()}")
    print(f"特征大小: {env.feature_size(sad=False)}")
    print(f"动作空间大小: {env.num_action()}")
    
    # 测试重置
    print("\n--- 测试重置 ---")
    env.reset()
    print(f"当前玩家: {env.get_current_player()}")
    print(f"分数: {env.get_score()}")
    print(f"生命: {env.get_life()}")
    print(f"信息令牌: {env.get_info()}")
    print(f"烟花: {env.get_fireworks()}")
    print(f"是否结束: {env.terminated()}")
    
    return env

def test_game_play(env, num_steps=10):
    """测试游戏进行"""
    print(f"\n=== 测试游戏进行 ({num_steps} 步) ===")
    
    for step in range(num_steps):
        if env.terminated():
            print(f"游戏在第 {step} 步结束")
            break
        
        current_player = env.get_current_player()
        print(f"\n步骤 {step + 1}: 玩家 {current_player} 的回合")
        
        # 获取当前状态
        print(f"  分数: {env.get_score()}")
        print(f"  生命: {env.get_life()}")
        print(f"  信息令牌: {env.get_info()}")
        
        # 执行随机动作
        action = get_random_action(env)
        print(f"  执行动作: {action}")
        
        try:
            env.step(action)
            print(f"  奖励: {env.step_reward()}")
        except Exception as e:
            print(f"  执行动作时出错: {e}")
            break
    
    print(f"\n游戏结束，最终分数: {env.last_episode_score()}")

def get_random_action(env):
    """生成随机动作"""
    # 这里需要根据你的动作空间来生成随机动作
    # 示例动作格式: (move_type, target_player, from_player, card_index, card_indices, color, rank)
    
    move_type = np.random.randint(1, 6)  # 1-5
    target_player = np.random.randint(-1, env.get_num_players())  # -1 到 num_players-1
    from_player = np.random.randint(0, env.get_num_players())  # 0 到 num_players-1
    card_index = np.random.randint(-1, 6)  # -1 到 4
    card_indices = [np.random.randint(0, 5) for _ in range(np.random.randint(0, 3))]  # 0-4张牌
    color = np.random.randint(0, 6)  # 0-5
    rank = np.random.randint(0, 6)  # 0-5
    
    return (move_type, target_player, from_player, card_index, card_indices, color, rank)

def test_multiple_envs():
    """测试多个环境"""
    print("\n=== 测试多个环境 ===")
    
    num_envs = 3
    envs = []
    
    for i in range(num_envs):
        params = {
            "players": "2",
            "seed": str(42 + i),
            "bomb": "0",
            "hand_size": "5",
            "random_start_player": "1"
        }
        
        env = CustomHanabiEnv(params, max_len=80, verbose=False)
        env.reset()
        envs.append(env)
        print(f"环境 {i}: 玩家 {env.get_current_player()}, 分数 {env.get_score()}")
    
    # 并行执行几步
    for step in range(5):
        print(f"\n步骤 {step + 1}:")
        for i, env in enumerate(envs):
            if not env.terminated():
                action = get_random_action(env)
                try:
                    env.step(action)
                    print(f"  环境 {i}: 玩家 {env.get_current_player()}, 分数 {env.get_score()}, 奖励 {env.step_reward()}")
                except Exception as e:
                    print(f"  环境 {i}: 错误 {e}")

def test_feature_size():
    """测试特征大小计算"""
    print("\n=== 测试特征大小计算 ===")
    
    for num_players in [2, 3, 4, 5]:
        params = {
            "players": str(num_players),
            "seed": "42",
            "bomb": "0",
            "hand_size": "5",
            "random_start_player": "1"
        }
        
        env = CustomHanabiEnv(params, max_len=80, verbose=False)
        
        # 测试SAD和非SAD模式
        for sad in [False, True]:
            total_size, priv_size, publ_size = env.feature_size(sad)
            print(f"玩家数 {num_players}, SAD {sad}: 总大小 {total_size}, 私有 {priv_size}, 公共 {publ_size}")

def main():
    """主函数"""
    print("开始测试自定义Hanabi环境...")
    
    try:
        # 测试基本功能
        env = test_basic_functionality()
        
        # 测试游戏进行
        test_game_play(env, num_steps=15)
        
        # 测试多个环境
        test_multiple_envs()
        
        # 测试特征大小
        test_feature_size()
        
        print("\n=== 所有测试完成 ===")
        
    except Exception as e:
        print(f"测试过程中出现错误: {e}")
        import traceback
        traceback.print_exc()

if __name__ == "__main__":
    main() 