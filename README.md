# HanaSim
UofT Hanabi Simulator for ML development and testing.

## Dependencies

* `cmake`: 3.22.1+
* `make`: 4.3
* `gcc`: 11.4.0 

## Set-up instructions

Before building, you must download the [greenfish repo](https://github.com/greenfish77/gaenari), and change these lines in `gaenari/include/gaenari/gaenari/common/json.hpp`:

* Line 1495: `path.push_back(0)` to `path.push_back((size_t)0)`
* Line 1553: `path.emplace_back(0)` to `path.emplace_back((size_t)0)`
* Line 1610: `path.emplace_back(0)` to `path.emplace_back((size_t)0)`
* Line 1983: `sax.path.emplace_back(0)` to `sax.path.emplace_back((size_t)0)`

After downloading the greenfish repo and changing the above lines, you must change this line in `HanaSim`'s `CMakeLists.txt` to point to the location of where you saved the greenfish repo:

```
add_subdirectory("../../greenfish-repo/gaenari" build) # <--- for treeagent. Change to where you have the greenfish/gaenari directory.
```

Now, see below how to build, depending on your OS.

### Linux

From the `HanaSim` directory:
 
```
mkdir build
cd build
cmake .. & make
```

### Windows

### Mac

From the `HanaSim` directory:

```
mkdir build
cd build
cmake .. & make
```

To install OpenMP:

```
brew install libomp
```
Add the following lines into the ./zshrc file:
```
export OpenMP_ROOT=$(brew --prefix)/opt/libomp
export PATH="/opt/homebrew/bin:$PATH"
```
Then
```
source ~/.zshrc
```



To install PyTorch/Libtorch (OpenMP must be installed to run Libtorch):

Visit: https://pytorch.org/get-started/locally/

Install the Mac Libtorch version, move the .zip file into HanaSim/include, and unpack the .zip file

Running the simulator will cause a "Library not loaded: @rpath/libomp.dylib" error. 

To resolve this error, locate the "libomp.dylib" executable in your homebrew folder 

For example: /opt/homebrew/Cellar/libomp/18.1.8/lib

Copy the executable and paste it into /usr/local/lib, or any other folders from the error message. 


## Running the simulator

After the `./HanaSim` executable is created, you can run it with the following arguments:

```
./HanaSim num-games num-players bot1 bot2 ... botn --log
```

* `num-games`: how many games you want to run (needs to be bigger than 0)
* `num-players`: how many players you want there to be (needs to be between 2 and 5)
* `bot1 bot2 ... botn`: bot types (see below) you want the players to be (there has to be exactly `num-players` number of bot types)
  * Valid bot types: `holmes`, `simple`, `random`, `smart`, `human`, and `tree` (for `tree`, you can only run it in a two-player setting where it is the second player, and the first player is either `holmes` or `smart`)
* `--log`: an optional flag you can add at the end if you want to log each game (i.e. every move, every draw, and score).



### Sample usage:

```
./HanaSim 100 3 holmes smart random --log
```
