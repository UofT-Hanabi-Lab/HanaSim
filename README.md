# HanaSim
UofT Hanabi Simulator for ML development and testing.

For local:
Example: <br />
    ./HanaSim  100 3 holmes holmes holmes <br />
Note: approx 16~17 seconds for 1000 games for 3 holmes bot on Mac


For docker:

docker build --progress=plain -t hanasim . <br />
docker run hanasim:latest {COMMAND LINE ARGS} <br />
Example: <br />
    docker run hanasim:latest 100 3 holmes holmes holmes <br />
Note: approx 6~7 seconds for 1000 games for 3 holmes bot on Docker (sequential)

