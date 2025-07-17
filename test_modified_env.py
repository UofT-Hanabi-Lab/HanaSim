#!/usr/bin/env python3
"""
测试修改后的custom_hanabi_env
"""

import os
import sys
import numpy as np

# Add the current directory to path
current_dir = os.path.dirname(os.path.abspath(__file__))
if current_dir not in sys.path:
    sys.path.insert(0, current_dir)

from custom_hanabi_env import CustomHanabiEnv

def test_feature_size_calculation():
    """测试特征大小计算"""
    print("=== 测试特征大小计算 ===")
    
    for num_players in [2, 3, 4, 5]:
        params = {
            "players": str(num_players),
            "seed": "42",
            "bomb": "0",
            "hand_size": "5",
            "random_start_player": "1"
        }
        
        env = CustomHanabiEnv(params, max_len=80, verbose=False)
        
        # 计算预期的向量编码大小
        cards_per_hand = 5 if num_players <= 3 else 4
        hands_section = (num_players - 1) * cards_per_hand * 25 + cards_per_hand * 10
        board_section = 86
        discard_section = 50
        expected_vector_size = hands_section + board_section + discard_section
        
        total_size, priv_size, publ_size = env.feature_size()
        
        print(f"玩家数 {num_players}:")
        print(f"  预期向量编码大小: {expected_vector_size}")
        print(f"  实际总特征大小: {total_size}")
        print(f"  私有特征大小: {priv_size}")
        print(f"  公共特征大小: {publ_size}")
        
        # 验证向量编码大小是否正确
        vector_encoding_size = total_size - (num_players * cards_per_hand * 2 + 
                                           num_players * cards_per_hand * 6 * 2 + 
                                           5 + 1 + 1 + 1 + 50 * 2 + 20 * 11 + 1)
        
        print(f"  计算出的向量编码大小: {vector_encoding_size}")
        print(f"  向量编码大小匹配: {vector_encoding_size == expected_vector_size}")
        print()

def test_action_space_calculation():
    """测试动作空间计算"""
    print("=== 测试动作空间计算 ===")
    
    for num_players in [2, 3, 4, 5]:
        params = {
            "players": str(num_players),
            "seed": "42",
            "bomb": "0",
            "hand_size": "5",
            "random_start_player": "1"
        }
        
        env = CustomHanabiEnv(params, max_len=80, verbose=False)
        
        cards_per_hand = 5 if num_players <= 3 else 4
        
        # 计算预期的动作数量
        color_hints = (num_players - 1) * 5
        rank_hints = (num_players - 1) * 5
        play_actions = cards_per_hand
        discard_actions = cards_per_hand
        expected_actions = color_hints + rank_hints + play_actions + discard_actions + 1
        
        actual_actions = env.num_action()
        
        print(f"玩家数 {num_players}:")
        print(f"  预期动作数量: {expected_actions}")
        print(f"  实际动作数量: {actual_actions}")
        print(f"  动作数量匹配: {expected_actions == actual_actions}")
        print()

def test_action_conversion():
    """测试动作转换"""
    print("=== 测试动作转换 ===")
    
    params = {
        "players": "2",
        "seed": "42",
        "bomb": "0",
        "hand_size": "5",
        "random_start_player": "1"
    }
    
    env = CustomHanabiEnv(params, max_len=80, verbose=False)
    env.reset()
    
    # 测试一些动作转换
    test_actions = [
        (1, 1, 0, -1, [0, 2], 1, 0),  # 颜色提示
        (2, 1, 0, -1, [1], 0, 3),     # 等级提示
        (3, -1, 0, 2, [], 0, 0),      # 出牌
        (4, -1, 0, 0, [], 0, 0),      # 弃牌
    ]
    
    for action_tuple in test_actions:
        action_id = env._action_tuple_to_id(action_tuple)
        converted_tuple = env._action_id_to_tuple(action_id)
        
        print(f"原始动作: {action_tuple}")
        print(f"动作ID: {action_id}")
        print(f"转换后动作: {converted_tuple}")
        print(f"转换正确: {action_tuple == converted_tuple}")
        print()

def test_game_mechanics():
    """测试游戏机制"""
    print("=== 测试游戏机制 ===")
    
    params = {
        "players": "2",
        "seed": "42",
        "bomb": "0",
        "hand_size": "5",
        "random_start_player": "1"
    }
    
    env = CustomHanabiEnv(params, max_len=80, verbose=True)
    env.reset()
    
    print(f"初始状态:")
    print(f"  当前玩家: {env.get_current_player()}")
    print(f"  分数: {env.get_score()}")
    print(f"  生命: {env.get_life()}")
    print(f"  信息令牌: {env.get_info()}")
    print(f"  烟花: {env.get_fireworks()}")
    print(f"  牌组大小: {env.current_state.deck_size if env.current_state else 0}")
    print(f"  是否结束: {env.terminated()}")
    
    # 执行几步动作
    for step in range(5):
        if env.terminated():
            print(f"游戏在第 {step} 步结束")
            break
        
        print(f"\n步骤 {step + 1}:")
        current_player = env.get_current_player()
        print(f"  当前玩家: {current_player}")
        
        # 获取一个随机动作
        action_id = np.random.randint(1, env.num_action())
        action_tuple = env._action_id_to_tuple(action_id)
        
        print(f"  执行动作ID: {action_id}")
        print(f"  动作元组: {action_tuple}")
        
        try:
            env.step(action_tuple)
            print(f"  奖励: {env.step_reward()}")
            print(f"  新分数: {env.get_score()}")
            print(f"  新生命: {env.get_life()}")
            print(f"  新信息令牌: {env.get_info()}")
        except Exception as e:
            print(f"  执行动作时出错: {e}")
            break
    
    print(f"\n游戏结束，最终分数: {env.last_episode_score()}")

def test_observation_structure():
    """测试观察结构"""
    print("=== 测试观察结构 ===")
    
    params = {
        "players": "2",
        "seed": "42",
        "bomb": "0",
        "hand_size": "5",
        "random_start_player": "1"
    }
    
    env = CustomHanabiEnv(params, max_len=80, verbose=False)
    env.reset()
    
    if env.current_state:
        obs = env.current_state
        print(f"观察结构:")
        print(f"  手牌数量: {len(obs.hands)}")
        print(f"  手牌形状: {[len(hand) for hand in obs.hands]}")
        print(f"  可能的颜色形状: {np.array(obs.possible_colors).shape if obs.possible_colors else 'None'}")
        print(f"  可能的等级形状: {np.array(obs.possible_ranks).shape if obs.possible_ranks else 'None'}")
        print(f"  烟花: {obs.fireworks}")
        print(f"  信息令牌: {obs.hint_tokens}")
        print(f"  生命: {obs.lives_remaining}")
        print(f"  牌组大小: {obs.deck_size}")
        print(f"  弃牌数量: {len(obs.discards)}")
        print(f"  合法动作数量: {len(obs.legal_actions)}")
        print(f"  当前玩家: {obs.current_player_id}")
        print(f"  向量编码长度: {len(obs.vector_encoding) if obs.vector_encoding else 0}")

def main():
    """主函数"""
    print("开始测试修改后的custom_hanabi_env...")
    
    try:
        # 测试特征大小计算
        test_feature_size_calculation()
        
        # 测试动作空间计算
        test_action_space_calculation()
        
        # 测试动作转换
        test_action_conversion()
        
        # 测试观察结构
        test_observation_structure()
        
        # 测试游戏机制
        test_game_mechanics()
        
        print("\n=== 所有测试完成 ===")
        
    except Exception as e:
        print(f"测试过程中出现错误: {e}")
        import traceback
        traceback.print_exc()

if __name__ == "__main__":
    main() 