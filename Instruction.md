## Step 1: Make sure your system python is installed

```sh
which python    # should output a valid path for python
```

## Step 2: Create and activate a virtual env, and install required packages:

```sh
python -m venv .venv
source .venv/bin/activate
pip install pybind11 numpy gymnasium
```

## Step 3: Compile the lib:
```sh
mkdir build
cd build
cmake -DPython3_EXECUTABLE=$(which python) .. && make
```
