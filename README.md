# HanaSim
UofT Hanabi Simulator for ML development and testing.

## Dependencies

* `cmake`: 3.22.1+
* `make`: 4.3
* `gcc`: 11.4.0 

## Set-up instructions

See below how to build, depending on your OS.

### Linux

From the `HanaSim` directory:
 
```
mkdir build
cd build
cmake .. & make
```

### Windows

### Mac

## Running the simulator

After the `./HanaSim` executable is created, you can run it with the following arguments:

```
./HanaSim num-games num-players bot1 bot2 ... botn --log
```

* `num-games`: how many games you want to run (needs to be bigger than 0)
* `num-players`: how many players you want there to be (needs to be between 2 and 5)
* `bot1 bot2 ... botn`: bot types (see below) you want the players to be (there has to be exactly `num-players` number of bot types)
  * Valid bot types: `holmes`, `simple`, `random`, `smart`, and `human`
* `--log`: an optional flag you can add at the end if you want to log each game (i.e. every move, every draw, and score).



### Sample usage:

```
./HanaSim 100 3 holmes smart simple --log
```
