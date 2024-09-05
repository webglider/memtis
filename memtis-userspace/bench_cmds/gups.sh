#!/bin/bash

BIN=/home/midhul/colloid/apps/gups
gups_cores=${MEMTIS_GUPS_CORES}
BENCH_DRAM="32000MB"
GUPS_DURATION=${MEMTIS_GUPS_DURATION}

workload=${NVM_RATIO}

if [ -n "${MEMTIS_GUPS_MOVE}" ]; then
	BENCH_RUN="${BIN}/${workload} ${gups_cores} move ${MEMTIS_GUPS_MOVE}"
elif [ -n "${MEMTIS_GUPS_DELAY_BG}" ]; then
	BENCH_RUN="DELAY_BG=${MEMTIS_GUPS_DELAY_BG} ${BIN}/${workload} ${gups_cores}"
else
	BENCH_RUN="${BIN}/${workload} ${gups_cores}"
fi


export BENCH_RUN
export BENCH_DRAM
export GUPS_DURATION
