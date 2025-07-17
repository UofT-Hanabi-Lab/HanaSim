# This is a helper file for hana_sim package
# Before importing hana_sim, you need to set the environment variable HANABI_SIM_PATH to the path of the hana_sim package

import os
import sys
# Add the build directory to path
BUILD_DIR = os.path.join(os.path.dirname(__file__), "build")