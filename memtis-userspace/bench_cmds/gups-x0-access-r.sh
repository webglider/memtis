#!/bin/bash

BIN=/home/omar/nucleus/apps
gups_cores=16
duration=7200
wss=192

BENCH_DRAM="96GB"

BENCH_RUN="${BIN}/gups-x0-access-r ${gups_cores} ${duration} ${wss}"

export BENCH_RUN
export BENCH_DRAM
