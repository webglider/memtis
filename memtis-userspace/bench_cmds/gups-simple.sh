#!/bin/bash

BIN=/home/midhul/memtis/memtis-userspace/bench_dir/gups
gups_cores=30
BENCH_DRAM="32000MB"
GUPS_DURATION=60

BENCH_RUN="${BIN}/gups64-rw ${gups_cores}"


export BENCH_RUN
export BENCH_DRAM
export GUPS_DURATION
