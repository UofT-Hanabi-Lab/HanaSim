## Step 1: Make sure your system python is installed

```sh
which python    # should output a valid path for python
```

## Step 2: Create and activate a virtual env, and install required packages:

```sh
python -m venv hanasim
source hanasim/bin/activate
pip install pybind11
pip install numpy
pip install gymnasium
```

## Step 3: Compile the lib:
```sh
mkdir build
cd build
cmake -DPython3_EXECUTABLE=$(which python) .. & make
```
## Step 4: Run the program:
```sh
python test.py
python HanabiEnv.py
```
