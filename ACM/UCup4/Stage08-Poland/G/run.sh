#!/bin/bash
# Testerka dla zawodnika do problemu interaktywnego G z AMPPZ 2025.
# Nie weryfikuje limitu czasu (ani nie kończy się, jeśli program działa
# zbyt długo).

# Użycie: ./run.sh $COMMAND_TO_RUN_PROGRAM $TEST_FILE_NAME
# Przykładowo:
# g++ G.cpp -o G.e && ./run.sh "./G.e" G0a.in
# Lub:
# ./run.sh "python3 G.py" G0a.in


# Stwórz pipe'y do komunikacji między sprawdzaczką a programem zawodnika.
declare -a FDS
# Skompiluj sprawdzaczkę.
g++ -Wall -O3 Gsoc.cpp -o Gsoc.e
# Uruchom sprawdzaczkę, przepinając pipe'y na stdin/out i podając dane testu
coproc FDS { ./Gsoc.e $2; }
# Uruchom program użytkownika, przepinając pipe'y na stdin/out.
$1 <&${FDS[0]} >&${FDS[1]}

