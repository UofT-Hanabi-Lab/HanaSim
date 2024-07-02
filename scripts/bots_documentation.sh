#!/bin/bash


bots=("holmes" "search" "random" "smart")
num_players=(2 3 4 5)


for bot in ${bots[@]};
do
    for num_p in ${num_players[@]};
    echo "Performance for $bot :"
    do
        echo "-- $num_p players --"
        ./../build/HanaSim 1000 $num_p $bot
    done
done
