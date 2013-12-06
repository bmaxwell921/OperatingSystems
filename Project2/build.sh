#!/bin/bash
# Used to compile the java code

MAIN_FILE="src/edu/iastate/cs352/project2/main/simVm.java"
BUILD_CMD="javac -sourcepath src -d bin $MAIN_FILE"
MK_BIN="mkdir ./bin"
RUN_CMD="java -cp ./bin/ edu.iastate.cs352.project2.main.simVm <ARGS>"

printf "INFO: Compiling code using:\n"
printf "\t$BUILD_CMD\n"

eval $MK_BIN
eval $BUILD_CMD

printf "INFO: Compiling complete. Run it using:\n"
printf "\t$RUN_CMD\n"
