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


# Sample bot performance (NOT STANDARDIZED) (1000 games)
## HolmesBot
2 players: 20.284, 28 games with perfect score (0.028%), run duration: 8968748us (8.96875s)

3 players: 21.147, 11 games with perfect score (0.011%), run duration: 16737891us (16.7379s)

4 players: 20.08, 1 games with perfect score (0.001%), run duration: 26786344us (26.7863s)

5 players: 18.366, 0 games with perfect score (0%), run duration: 38537717us (38.5377s)
## SimpleBot
2 players: 9.906, 0 games with perfect score (0%), run duration: 1370075us (1.37007s)

3 players: 10.625, 0 games with perfect score (0%), run duration: 1762869us (1.76287s)

4 players: 11.597, 0 games with perfect score (0%), run duration: 2471319us (2.47132s)

5 players: 12.372, 0 games with perfect score (0%), run duration: 3417108us (3.41711s)
## RandomBot
2 players: 1.273, 0 games with perfect score (0%), run duration: 710727us (0.710727s)

3 players: 1.287, 0 games with perfect score (0%), run duration: 1063775us (1.06377s)

4 players: 1.224, 0 games with perfect score (0%), run duration: 1293957us (1.29396s)

5 players: 1.299, 0 games with perfect score (0%), run duration: 1520993us (1.52099s)
## SmartBot
2 players: 21.31, 92 games with perfect score (0.092%), run duration: 72518898us (72.5189s)

3 players: 22.799, 222 games with perfect score (0.222%), run duration: 114002347us (114.002s)

4 players: 22.281, 48 games with perfect score (0.048%), run duration: 155664236us (155.664s)

5 players: 20.645, 0 games with perfect score (0%), run duration: 201465092us (201.465s)
