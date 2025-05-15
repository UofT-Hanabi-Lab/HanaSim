Our conventions for representing Hanabi
=======================================

HanaSim, and consequently, HanaSimPy, obey a number of conventions when representing
Hanabi. These conventions may differ from other Hanabi projects or simulators, so we
document important design decisions below.

#. Card ranks are 1-based
#. There are exactly 5 card colours: red, white, yellow, green, blue
#. When lives are depleted, the game score so far is recorded, *not set to 0*
