#!/usr/bin/env python3
"""
Test compatibility of HanaSim/OBL/act_group.py with hana_sim replacing hanalearn
"""

import os
import sys
import torch
import numpy as np

# Add paths
current_dir = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, current_dir)

def test_custom_actor():
    """Test CustomR2D2Actor functionality"""
    print("=== Testing CustomR2D2Actor ===")
    
    try:
        from custom_actor import CustomR2D2Actor, MockModelRunner
        
        # Create mock model runner
        model_runner = MockModelRunner('cpu')
        
        # Create actor
        actor = CustomR2D2Actor(
            model_runner=model_runner,
            seed=42,
            num_player=2,
            player_idx=0,
            explore_eps=[0.1],
            boltzmann_t=[1.0],
            vdn=True,
            sad=False,
            shuffle_color=False,
            hide_action=False,
            trinary=True,
            replay_buffer=None,
            multi_step=3,
            max_len=80,
            gamma=0.999,
        )
        
        print(f"Actor created successfully: player_idx={actor.player_idx}")
        
        # Test actor methods
        assert hasattr(actor, 'act'), "Actor missing 'act' method"
        assert hasattr(actor, 'set_partners'), "Actor missing 'set_partners' method"
        assert hasattr(actor, 'set_belief_runner'), "Actor missing 'set_belief_runner' method"
        assert hasattr(actor, 'get_success_fict_rate'), "Actor missing 'get_success_fict_rate' method"
        
        # Test act method with mock observation
        class MockObs:
            def __init__(self):
                self.vector_encoding = [0.0] * 658
                self.legal_actions = [(1, -1, 0, -1, [], 0, 0)] * 5
                self.hands = [[(1, 2), (2, 3)], [(3, 4), (4, 5)]]
                self.fireworks = {'red': 0, 'white': 1, 'yellow': 0, 'green': 0, 'blue': 0}
                self.hint_tokens = 8
                self.lives_remaining = 3
                self.deck_size = 40
        
        obs = MockObs()
        action = actor.act(obs)
        print(f"Actor returned action: {action}")
        assert isinstance(action, int), "Action should be an integer"
        
        # Test set_partners
        partner_actors = [None, actor]
        actor.set_partners(partner_actors)
        assert actor.partners == partner_actors, "Partners not set correctly"
        
        # Test set_belief_runner
        actor.set_belief_runner(None)
        assert actor.belief_runner is None, "Belief runner not set correctly"
        
        # Test get_success_fict_rate
        rate = actor.get_success_fict_rate()
        assert isinstance(rate, float), "Success rate should be a float"
        assert 0 <= rate <= 1, "Success rate should be between 0 and 1"
        
        print("CustomR2D2Actor test passed!")
        return True
        
    except Exception as e:
        print(f"CustomR2D2Actor test failed: {e}")
        import traceback
        traceback.print_exc()
        return False

def test_act_group():
    """Test ActGroup functionality"""
    print("\n=== Testing ActGroup ===")
    
    try:
        from act_group import ActGroup, MockModelRunner
        import rela
        
        # Mock agent class
        class MockAgent:
            def __init__(self):
                self.device = 'cpu'
            
            def clone(self, device):
                mock_clone = MockAgent()
                mock_clone.device = device
                return mock_clone
        
        # Mock replay buffer
        class MockReplayBuffer:
            def __init__(self):
                pass
            
            def add(self, experience):
                pass
        
        # Create mock components
        agent = MockAgent()
        replay_buffer = MockReplayBuffer()
        
        # Test ActGroup creation
        act_group = ActGroup(
            devices="cpu",
            agent=agent,
            seed=42,
            num_thread=2,
            num_game_per_thread=2,
            num_player=2,
            explore_eps=[0.1, 0.05],
            boltzmann_t=[],
            method="iql",
            sad=False,
            shuffle_color=False,
            hide_action=False,
            trinary=True,
            replay_buffer=replay_buffer,
            multi_step=3,
            max_len=80,
            gamma=0.999,
            off_belief=False,
            belief_model=None,
        )
        
        print(f"ActGroup created with {len(act_group.actors)} thread groups")
        
        # Verify structure
        assert len(act_group.actors) == 2, "Should have 2 thread groups"
        for thread_actors in act_group.actors:
            assert len(thread_actors) == 2, "Each thread should have 2 games"
            for game_actors in thread_actors:
                assert len(game_actors) == 2, "Each game should have 2 actors"
        
        # Test start method
        act_group.start()
        print("ActGroup started successfully")
        
        # Test update_model method
        act_group.update_model(agent)
        print("ActGroup model updated successfully")
        
        print("ActGroup test passed!")
        return True
        
    except Exception as e:
        print(f"ActGroup test failed: {e}")
        import traceback
        traceback.print_exc()
        return False

def test_compatibility_with_create():
    """Test compatibility between act_group and create modules"""
    print("\n=== Testing Compatibility with Create Module ===")
    
    try:
        from create import create_envs, create_threads
        from act_group import ActGroup
        
        # Mock agent and replay buffer
        class MockAgent:
            def clone(self, device):
                return self
        
        class MockReplayBuffer:
            pass
        
        # Create environments
        games = create_envs(
            num_env=4,
            seed=42,
            num_player=2,
            bomb=0,
            max_len=80
        )
        
        print(f"Created {len(games)} environments")
        
        # Create actors
        act_group = ActGroup(
            devices="cpu",
            agent=MockAgent(),
            seed=42,
            num_thread=2,
            num_game_per_thread=2,
            num_player=2,
            explore_eps=[0.1],
            boltzmann_t=[],
            method="iql",
            sad=False,
            shuffle_color=False,
            hide_action=False,
            trinary=True,
            replay_buffer=MockReplayBuffer(),
            multi_step=3,
            max_len=80,
            gamma=0.999,
            off_belief=False,
            belief_model=None,
        )
        
        # Create threads
        context, threads = create_threads(
            num_thread=2,
            num_game_per_thread=2,
            actors=act_group.actors,
            games=games
        )
        
        print(f"Created {len(threads)} threads")
        print(f"Context type: {type(context)}")
        
        # Verify thread types
        for i, thread in enumerate(threads):
            print(f"Thread {i} type: {type(thread)}")
        
        print("Compatibility test passed!")
        return True
        
    except Exception as e:
        print(f"Compatibility test failed: {e}")
        import traceback
        traceback.print_exc()
        return False

def main():
    """Run all tests"""
    print("Testing HanaSim/OBL/act_group.py compatibility")
    print("=" * 60)
    
    tests = [
        test_custom_actor,
        test_act_group,
        test_compatibility_with_create,
    ]
    
    passed = 0
    total = len(tests)
    
    for test in tests:
        if test():
            passed += 1
    
    print("\n" + "=" * 60)
    print(f"Test Results: {passed}/{total} passed")
    
    if passed == total:
        print("🎉 All tests passed! act_group.py successfully replaced hanalearn with hana_sim")
        return True
    else:
        print("❌ Some tests failed, further debugging needed")
        return False

if __name__ == "__main__":
    main() 