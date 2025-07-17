#!/usr/bin/env python3
"""
测试 HanaSim/OBL/create.py 是否能正确替代 OBL/pyhanabi/create.py 中的 hanalearn 功能
"""

import os
import sys

# Add paths
current_dir = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, current_dir)

def test_create_envs():
    """测试 create_envs 函数"""
    print("=== 测试 create_envs 函数 ===")
    
    try:
        from create import create_envs
        
        # 创建环境
        games = create_envs(
            num_env=2,
            seed=42,
            num_player=2,
            bomb=0,
            max_len=80,
            hand_size=5,
            random_start_player=1
        )
        
        print(f"成功创建 {len(games)} 个环境")
        
        # 测试第一个游戏的基本功能
        game = games[0]
        print(f"游戏类型: {type(game)}")
        print(f"玩家数量: {game.get_num_players()}")
        print(f"特征维度: {game.feature_size(sad=False)}")
        print(f"动作空间大小: {game.num_action()}")
        
        # 测试重置
        game.reset()
        print(f"当前玩家: {game.get_current_player()}")
        print(f"游戏是否结束: {game.terminated()}")
        
        print("create_envs 测试通过!")
        return True
        
    except Exception as e:
        print(f"create_envs 测试失败: {e}")
        import traceback
        traceback.print_exc()
        return False

def test_flatten():
    """测试 flatten 函数"""
    print("\n=== 测试 flatten 函数 ===")
    
    try:
        from create import flatten
        
        # 测试用例1: 空列表
        result1 = flatten([])
        assert result1 == [], f"期望 [], 得到 {result1}"
        
        # 测试用例2: 简单列表
        result2 = flatten([1, 2, 3])
        assert result2 == [1, 2, 3], f"期望 [1, 2, 3], 得到 {result2}"
        
        # 测试用例3: 嵌套列表
        result3 = flatten([[1, 2], [3, 4]])
        assert result3 == [1, 2, 3, 4], f"期望 [1, 2, 3, 4], 得到 {result3}"
        
        # 测试用例4: 深度嵌套
        result4 = flatten([[[1, 2], 3], [4, 5]])
        assert result4 == [1, 2, 3, 4, 5], f"期望 [1, 2, 3, 4, 5], 得到 {result4}"
        
        print("flatten 测试通过!")
        return True
        
    except Exception as e:
        print(f"flatten 测试失败: {e}")
        import traceback
        traceback.print_exc()
        return False

def test_create_threads():
    """测试 create_threads 函数"""
    print("\n=== 测试 create_threads 函数 ===")
    
    try:
        from create import create_envs, create_threads
        import rela
        
        # 创建模拟的 actors
        class MockActor:
            def __init__(self, player_id):
                self.player_id = player_id
            
            def act(self, obs):
                # 返回一个简单的动作
                return (1, -1, 0, -1, [], 0, 0)
        
        # 创建环境
        games = create_envs(
            num_env=4,
            seed=42,
            num_player=2,
            bomb=0,
            max_len=80
        )
        
        # 创建演员组
        actors = []
        for thread_idx in range(2):  # 2个线程
            thread_actors = []
            for game_idx in range(2):  # 每个线程2个游戏
                game_actors = []
                for player_idx in range(2):  # 每个游戏2个玩家
                    game_actors.append(MockActor(player_idx))
                thread_actors.append(game_actors)
            actors.append(thread_actors)
        
        # 创建线程
        context, threads = create_threads(
            num_thread=2,
            num_game_per_thread=2,
            actors=actors,
            games=games
        )
        
        print(f"成功创建 {len(threads)} 个线程")
        print(f"Context 类型: {type(context)}")
        
        # 检查线程类型
        for i, thread in enumerate(threads):
            print(f"线程 {i} 类型: {type(thread)}")
        
        print("create_threads 测试通过!")
        return True
        
    except Exception as e:
        print(f"create_threads 测试失败: {e}")
        import traceback
        traceback.print_exc()
        return False

def test_compatibility():
    """测试与原版的兼容性"""
    print("\n=== 测试兼容性 ===")
    
    try:
        from create import create_envs
        
        # 使用与原版相同的参数创建环境
        games = create_envs(
            num_env=1,
            seed=10001,
            num_player=2,
            bomb=0,
            max_len=80,
            hand_size=5,
            random_start_player=1
        )
        
        game = games[0]
        
        # 检查是否有原版 hanalearn.HanabiEnv 的主要方法
        required_methods = [
            'feature_size', 'num_action', 'reset', 'step', 'terminated',
            'get_current_player', 'get_num_players', 'get_score', 
            'get_life', 'get_info', 'get_fireworks'
        ]
        
        missing_methods = []
        for method in required_methods:
            if not hasattr(game, method):
                missing_methods.append(method)
        
        if missing_methods:
            print(f"缺少方法: {missing_methods}")
            return False
        
        print("所有必需的方法都存在")
        
        # 测试方法调用
        feature_size = game.feature_size(sad=False)
        print(f"特征维度: {feature_size}")
        
        num_action = game.num_action()
        print(f"动作空间: {num_action}")
        
        print("兼容性测试通过!")
        return True
        
    except Exception as e:
        print(f"兼容性测试失败: {e}")
        import traceback
        traceback.print_exc()
        return False

def main():
    """运行所有测试"""
    print("开始测试 HanaSim/OBL/create.py 兼容性")
    print("=" * 50)
    
    tests = [
        test_create_envs,
        test_flatten,
        test_create_threads,
        test_compatibility
    ]
    
    passed = 0
    total = len(tests)
    
    for test in tests:
        if test():
            passed += 1
    
    print("\n" + "=" * 50)
    print(f"测试结果: {passed}/{total} 通过")
    
    if passed == total:
        print("🎉 所有测试都通过！HanaSim/OBL/create.py 可以替代原版 hanalearn 功能")
        return True
    else:
        print("❌ 部分测试失败，需要进一步修复")
        return False

if __name__ == "__main__":
    main() 