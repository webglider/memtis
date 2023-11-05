#!/bin/bash

BIN=/home/midhul/memtis/memtis-userspace/bench_dir/gups
gups_cores=4
BENCH_DRAM="28000MB"
GUPS_DURATION=120

BENCH_RUN="${BIN}/gups-r ${gups_cores}"


export BENCH_RUN
export BENCH_DRAM
export GUPS_DURATION
