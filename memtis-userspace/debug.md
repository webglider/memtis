### Reproducing memory leak problem

Compile app
```
cd bench_dir/gups
make clean && make
cd ..
cd ..
```

Check free memory
```
numastat -m
```

Run app
```
sudo ./scripts/run_bench.sh -B gups-simple --cxl -V debug-memleak
```

Check free memory
```
numastat -m
```
