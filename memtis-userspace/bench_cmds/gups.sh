#!/bin/bash

BIN=/home/midhul/colloid/gups
gups_cores=4
BENCH_DRAM="28000MB"
GUPS_DURATION=400

workload=${NVM_RATIO}

BENCH_RUN="${BIN}/${workload} ${gups_cores}"


export BENCH_RUN
export BENCH_DRAM
export GUPS_DURATION
