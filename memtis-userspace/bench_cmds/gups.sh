#!/bin/bash

BIN=/home/midhul/colloid/apps/gups
gups_cores=${MEMTIS_GUPS_CORES}
BENCH_DRAM="32000MB"
GUPS_DURATION=${MEMTIS_GUPS_DURATION}

workload=${NVM_RATIO}

if [ -n "${MEMTIS_GUPS_MOVE}" ]; then
	BENCH_RUN="${BIN}/${workload} ${gups_cores} move ${MEMTIS_GUPS_MOVE}"
else
    BENCH_RUN="${BIN}/${workload} ${gups_cores}"
fi


export BENCH_RUN
export BENCH_DRAM
export GUPS_DURATION
if [ -n "${MEMTIS_GUPS_DELAY_BG}" ]; then
    DELAY_BG=${MEMTIS_GUPS_DELAY_BG}
    export DELAY_BG
fi
