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


# Bot performance (1000 games)
## HolmesBot
2 players: 16.123, 0 games with perfect score (0%), run duration: 5258689us (5.25869s)

3 players: 16.538, 0 games with perfect score (0%), run duration: 10331169us (10.3312s)

4 players: 15.342, 0 games with perfect score (0%), run duration: 14409079us (14.4091s)

5 players: 13.649, 0 games with perfect score (0%), run duration: 20674030us (20.674s)
## SimpleBot
2 players: 9.443, 0 games with perfect score (0%), run duration: 815536us (0.815536s)

3 players: 10.356, 0 games with perfect score (0%), run duration: 1128006us (1.12801s)

4 players: 11.141, 0 games with perfect score (0%), run duration: 1889337us (1.88934s)

5 players: 10.75, 0 games with perfect score (0%), run duration: 2297542us (2.29754s)
## RandomBot
2 players: 1.225, 0 games with perfect score (0%), run duration: 541186us (0.541186s)

3 players: 1.229, 0 games with perfect score (0%), run duration: 826041us (0.826041s)

4 players: 1.271, 0 games with perfect score (0%), run duration: 1024374us (1.02437s)

5 players: 1.259, 0 games with perfect score (0%), run duration: 1235524us (1.23552s)
## SmartBot
2 players: 17.33, 0 games with perfect score (0%), run duration: 41914822us (41.9148s)

3 players: 18.209, 0 games with perfect score (0%), run duration: 65310328us (65.3103s)

4 players: 16.845, 0 games with perfect score (0%), run duration: 89109993us (89.11s)

5 players: 14.824, 0 games with perfect score (0%), run duration: 121239240us (121.239s)

