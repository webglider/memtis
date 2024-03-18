#!/bin/bash

BIN=/home/midhul/colloid/gups
gups_cores=${MEMTIS_GUPS_CORES}
BENCH_DRAM="32000MB"
GUPS_DURATION=${MEMTIS_GUPS_DURATION}

workload=${NVM_RATIO}

BENCH_RUN="${BIN}/${workload} ${gups_cores}"


export BENCH_RUN
export BENCH_DRAM
export GUPS_DURATION
