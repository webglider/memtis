#!/bin/bash

# BENCHMARKS="XSBench graph500 gapbs-pr liblinear silo btree speccpu-bwaves speccpu-roms"
BENCHMARKS="gapbs-pr"
LOCAL_NUMA=1
REMOTE_NUMA=0

sudo dmesg -c

# enable THP
sudo echo "always" | tee /sys/kernel/mm/transparent_hugepage/enabled
sudo echo "always" | tee /sys/kernel/mm/transparent_hugepage/defrag
# decrease uncore freq
# sudo ./scripts/set_uncore_freq.sh on

for BENCH in ${BENCHMARKS};
do
    export GOMP_CPU_AFFINITY=1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,33,35,37,39,41,43,45,47,49,51,53,55,57,59,61,63
    if [[ -e ./bench_cmds/${BENCH}.sh ]]; then
	source ./bench_cmds/${BENCH}.sh
    else
	echo "ERROR: ${BENCH}.sh does not exist."
	continue
    fi

    mkdir -p results/${BENCH}/all-cxl/static
    LOG_DIR=results/${BENCH}/all-cxl/static

    free;sync;echo 3 > /proc/sys/vm/drop_caches;free;

    if [[ "x${BENCH}" =~ "xspeccpu" ]]; then
	/usr/bin/time -f "execution time %e (s)" \
	    taskset -c 1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,33,35,37,39,41,43,45,47,49,51,53,55,57,59,61,63 numactl -m $REMOTE_NUMA ${BENCH_RUN} < ${BENCH_ARG} 2>&1 \
	    | tee ${LOG_DIR}/output.log
    else
	/usr/bin/time -f "execution time %e (s)" \
	    numactl -N $LOCAL_NUMA -m $REMOTE_NUMA ${BENCH_RUN} 2>&1 \
	    | tee ${LOG_DIR}/output.log
    fi
done

# sudo ./scripts/set_uncore_freq.sh off
