#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <sys/time.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <immintrin.h>
#include <fcntl.h>
#include <sched.h>
#include <pthread.h>
#include <stdatomic.h>

#define MAX_THREADS 8
#define LOCAL_NUMA 3
#define REMOTE_NUMA 2
#define CORE_LIST 3,7,11,15,19,23,27,31 // Update this to list of cores on NUMA node

#define WSS 68719476736ULL // 64GiB
#define HOTSS 17179869184ULL // 16GiB
#define CHUNK_SIZE 4096

size_t pg_size;

typedef struct {
    int thread_id;
    size_t buf_size;
    size_t hot_size;
    _Atomic uint64_t *count_ptr;
    int manual_placement;
    size_t local_hot_pages;
    int reset_mbind;
    _Atomic int finish;
} ThreadArgs;

void *thread_function(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    // char *a = (char *)malloc(args->buf_size);
    int mmap_flags =  MAP_PRIVATE |  MAP_ANONYMOUS;
    char *a = mmap(0, args->buf_size, PROT_READ | PROT_WRITE, mmap_flags, -1, 0);
    if(a == NULL) {
        printf("mmap failed\n");
        return NULL;
    }
    
    memset(a, 'm', args->buf_size);

    // Prevent compiler reordering
    asm volatile("" : : : "memory");

    uint64_t x = 432437644 + args->thread_id;
    uint64_t count = 0, prev_count = 0;
    __m512i sum = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    int i;
    while(count < 999999999999999ULL) {
        for(i = 0; i < 1024; i++) {
            char *start;
            size_t slots;
            x ^= x << 13;
            x ^= x >> 7;
            x ^= x << 17;
            if(x%100 < 90) {
                // access hot region with 90% probability
                start = a + (args->buf_size - args->hot_size);
                slots = args->hot_size / CHUNK_SIZE;
            } else {
                // access cold region with 10% probability
                start = a;
                slots = (args->buf_size - args->hot_size)/CHUNK_SIZE;
            }

            // access random slot
            x ^= x << 13;
            x ^= x >> 7;
            x ^= x << 17;
            char *chunk = start + CHUNK_SIZE*(x%slots);
            int k;
            for(k = 0; k < 64; k++) {
                __m512i mm_a = _mm512_load_si512(&chunk[64*k]);
                sum = _mm512_add_epi32(sum, mm_a);
            }
            count++;
        }
        atomic_store(args->count_ptr, count);
        if(atomic_load(&(args->finish))) {
            return NULL;
        }
    }

        // The below is just to make sure that the compiler doen't optimize away the above
        uint64_t read_checksum;
        int chx0, chx1, chx2, chx3;
        __m128i chx;
        chx = _mm512_extracti32x4_epi32(sum, 0);
        chx0 = _mm_extract_epi32(chx, 0);
        chx1 = _mm_extract_epi32(chx, 1);
        chx2 = _mm_extract_epi32(chx, 2);
        chx3 = _mm_extract_epi32(chx, 3);
        read_checksum += chx0 + chx1 + chx2 + chx3;
        chx = _mm512_extracti32x4_epi32(sum, 1);
        chx0 = _mm_extract_epi32(chx, 0);
        chx1 = _mm_extract_epi32(chx, 1);
        chx2 = _mm_extract_epi32(chx, 2);
        chx3 = _mm_extract_epi32(chx, 3);
        read_checksum += chx0 + chx1 + chx2 + chx3;
        chx = _mm512_extracti32x4_epi32(sum, 2);
        chx0 = _mm_extract_epi32(chx, 0);
        chx1 = _mm_extract_epi32(chx, 1);
        chx2 = _mm_extract_epi32(chx, 2);
        chx3 = _mm_extract_epi32(chx, 3);
        read_checksum += chx0 + chx1 + chx2 + chx3;
        chx = _mm512_extracti32x4_epi32(sum, 3);
        chx0 = _mm_extract_epi32(chx, 0);
        chx1 = _mm_extract_epi32(chx, 1);
        chx2 = _mm_extract_epi32(chx, 2);
        chx3 = _mm_extract_epi32(chx, 3);
        read_checksum += chx0 + chx1 + chx2 + chx3;
        printf("checksum reached: %lu\n", read_checksum);
        int xyz;
        uint64_t wrchk = 0;
        for(xyz = 0; xyz < args->buf_size; xyz++) {
            wrchk += (int)(a[xyz]);
        }
        printf("wrchk: %lu\n", wrchk);
    
    return NULL;
}

int main(int argc, char *argv[]) {
    pg_size = 2ULL*1024ULL*1024ULL;
    setbuf(stdout, NULL);
    int cores[8] = {3,7,11,15,19,23,27,31};
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    if (num_threads <= 0 || num_threads > MAX_THREADS) {
        fprintf(stderr, "Number of threads invalid\n");
        return 1;
    }

    _Atomic uint64_t thread_counts[MAX_THREADS];
    pthread_t threads[MAX_THREADS];
    ThreadArgs thread_args[MAX_THREADS];
    cpu_set_t cpuset;

    for (int i = 0; i < num_threads; ++i) {
        thread_args[i].thread_id = i;
        thread_args[i].buf_size = ((WSS/pg_size)/((size_t)num_threads))*pg_size;
        thread_args[i].hot_size = ((HOTSS/pg_size)/((size_t)num_threads))*pg_size;
        atomic_init(&thread_counts[i], 0);
        thread_args[i].count_ptr = &thread_counts[i];
        atomic_init(&(thread_args[i].finish), 0);
        
        CPU_ZERO(&cpuset);
        CPU_SET(cores[i], &cpuset);

        if (pthread_create(&threads[i], NULL, thread_function, &thread_args[i]) != 0) {
            perror("pthread_create");
            return 1;
        }

        if (pthread_setaffinity_np(threads[i], sizeof(cpu_set_t), &cpuset) != 0) {
            perror("pthread_setaffinity_np");
            return 1;
        }
    }

    uint64_t prev_op_count = 0;
    int elapsed = 0;
    int max_duration = 100000;
    if(getenv("GUPS_DURATION") != NULL) {
        max_duration = atoi(getenv("GUPS_DURATION"));
    }
    while(elapsed < max_duration) {
        sleep(1);
        uint64_t cur_op_count = 0;
        for(int i = 0; i < num_threads; i++) {
            cur_op_count += atomic_load(&thread_counts[i]);
        }
        printf("%lu\n", cur_op_count - prev_op_count);
        prev_op_count = cur_op_count;
        elapsed++;
    }

    for(int i = 0; i < num_threads; i++) {
        atomic_store(&(thread_args[i].finish), 1);
    }

    for (int i = 0; i < num_threads; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            return 1;
        }
    }

    return 0;
}
