# HanaSim
UofT Hanabi Simulator for ML development and testing.

For local:
Example: <br />
    ./HanaSim  100 3 holmes holmes holmes


For docker:

docker build --progress=plain -t hanasim . <br />
docker run hanasim:latest {COMMAND LINE ARGS} <br />
Example: <br />
    docker run hanasim:latest 100 3 holmes holmes holmes

