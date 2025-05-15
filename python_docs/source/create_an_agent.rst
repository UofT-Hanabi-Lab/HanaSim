Creating your own Python agent
==============================

Your agent must consume the game state represented by :py:class:`Observation`.
We recommend creating a field in your agent to store the current state.

When your agent is ready to make an action, encode the action into the tuple format
accepted by :py:meth:`HanabiEnv.step` and pass it to the method. This will produce a
:py:class:`StepResult` object that contains the next game state
(:py:attr:`StepResult.observation`) as well as some additional info.

If the game isn't over yet, you can keep passing actions to :py:meth:`HanabiEnv.step`
and reading the results.

After your agent takes a step that ends the game (e.g. by depleting the deck),
:py:attr:`StepResult.done` will be set to `True`.
