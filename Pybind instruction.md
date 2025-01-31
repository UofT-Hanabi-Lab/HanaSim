# Using Pybind in This Project

## Important Notice
Currently, I have not explored how to make Pybind11 work within a virtual environment (`venv`). Instead, we **strongly recommend** installing Python via Homebrew and using the system-wide installation.

---

## Step 1: Install Python via Homebrew
Ensure you have **Homebrew** installed. If not, you can install it using:
```sh
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
Then, install python, get the python path:
```sh
brew install python
which python3
```
output should similar to '/opt/homebrew/bin/python3'

## Step 2: Set path in CMakeList.txt
```sh
brew install pybind11
brew info pybind11
# output like /opt/homebrew/opt/pybind11/lib/python3.13/site-packages/pybind11/include
python3-config --includes
# output like -I/opt/homebrew/opt/python@3.13/Frameworks/Python.framework/Versions/3.13/include/python3.13
```
Set the path above in Cmake include dir, like:
```c
include_directories(/opt/homebrew/opt/pybind11/lib/python3.13/site-packages/pybind11/include)
include_directories(/opt/homebrew/opt/python@3.13/Frameworks/Python.framework/Versions/3.13/include/python3.13)
```

## Step 3: Install Gymnasium globally:
Since we are using brew python, Gymnasium should also be install for brew python:
```sh
/opt/homebrew/bin/python3 -m pip install gymnasium --break-system-packages
```
