#!/bin/bash

BIN=/home/omar/nucleus/apps
gups_cores=8
x_z_ratio=100
duration=300
wss=80

BENCH_DRAM="8GB"

BENCH_RUN="${BIN}/gups-xz-access-r ${gups_cores} ${duration} ${wss} ${x_z_ratio}"

export BENCH_RUN
export BENCH_DRAM
