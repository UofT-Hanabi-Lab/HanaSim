#!/bin/bash


bots=("holmes" "simple" "random" "smart")
num_players=(2 3 4 5)


for bot in ${bots[@]};
do
    echo "Performance for $bot :"
    echo "-- 2 players --"
    ./../build/HanaSim 1000 2 $bot $bot
    echo "-- 3 players --"
    ./../build/HanaSim 1000 3 $bot $bot $bot
    echo "-- 4 players --"
    ./../build/HanaSim 1000 4 $bot $bot $bot $bot
    echo "-- 5 players --"
    ./../build/HanaSim 1000 5 $bot $bot $bot $bot $bot

done
