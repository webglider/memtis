#!/bin/bash

BIN=/home/omar/nucleus/apps
gups_cores=16
x_y_ratio=10000
duration=300
wss=64

BENCH_DRAM="32GB"

BENCH_RUN="${BIN}/gups-xy-access-r ${gups_cores} ${duration} ${wss} ${x_y_ratio}"

export BENCH_RUN
export BENCH_DRAM
