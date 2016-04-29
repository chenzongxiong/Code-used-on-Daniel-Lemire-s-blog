// gcc -std=c99 -O3 -o rleunion rleunion.c
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>


#define RDTSC_START(cycles)                                                   \
    do {                                                                      \
        register unsigned cyc_high, cyc_low;                                  \
        __asm volatile(                                                       \
            "cpuid\n\t"                                                       \
            "rdtsc\n\t"                                                       \
            "mov %%edx, %0\n\t"                                               \
            "mov %%eax, %1\n\t"                                               \
            : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx", "%rdx"); \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                      \
    } while (0)

#define RDTSC_FINAL(cycles)                                                   \
    do {                                                                      \
        register unsigned cyc_high, cyc_low;                                  \
        __asm volatile(                                                       \
            "rdtscp\n\t"                                                      \
            "mov %%edx, %0\n\t"                                               \
            "mov %%eax, %1\n\t"                                               \
            "cpuid\n\t"                                                       \
            : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx", "%rdx"); \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                      \
    } while (0)

/*
 * Prints the best number of operations per cycle where
 * test is the function call, answer is the expected answer generated by
 * test, repeat is the number of times we should repeat and size is the
 * number of operations represented by test.
 */
#define BEST_TIME(test, answer, repeat, size)                         \
    do {                                                              \
        printf("%s: ", #test);                                        \
        fflush(NULL);                                                 \
        uint64_t cycles_start, cycles_final, cycles_diff;             \
        uint64_t min_diff = (uint64_t)-1;                             \
        int wrong_answer = 0;                                         \
        for (int i = 0; i < repeat; i++) {                            \
            __asm volatile("" ::: /* pretend to clobber */ "memory"); \
            RDTSC_START(cycles_start);                                \
            if (test != answer) wrong_answer = 1;                     \
            RDTSC_FINAL(cycles_final);                                \
            cycles_diff = (cycles_final - cycles_start);              \
            if (cycles_diff < min_diff) min_diff = cycles_diff;       \
        }                                                             \
        uint64_t S = (uint64_t)size;                                  \
        float cycle_per_op = (min_diff) / (float)S;                   \
        printf(" %.2f cycles per operation", cycle_per_op);           \
        if (wrong_answer) printf(" [ERROR]");                         \
        printf("\n");                                                 \
        fflush(NULL);                                                 \
    } while (0)


/* struct rle16_s - run length pair
 *
 * @value:  start position of the run
 * @length: length of the run is `length + 1`
 *
 * An RLE pair {v, l} would represent the integers between the interval
 * [v, v+l+1], e.g. {3, 2} = [3, 4, 5].
 */
struct rle16_s {
    uint16_t value;
    uint16_t length;
};

typedef struct rle16_s rle16_t;


/**
 * Append run described by vl to the run container, possibly merging.
 * It is assumed that the run would be inserted at the end of the container, no
 * check is made.
 * It is assumed that the run container has the necessary capacity: caller is
 * responsible for checking memory capacity.
 *
 *
 * This is not a safe function, it is meant for performance: use with care.
 */
static inline void run_container_append(rle16_t *run, rle16_t vl,
                                        rle16_t *previousrl, uint32_t * n_runs) {
    const uint32_t previousend = previousrl->value + previousrl->length;
    if (vl.value > previousend + 1) {  // we add a new one
        run[*n_runs] = vl;
        *n_runs = *n_runs + 1;
        *previousrl = vl;
    }
    uint32_t newend = vl.value + vl.length + UINT32_C(1);
    if (newend > previousend) {  // we merge
        previousrl->length = newend - 1 - previousrl->value;
        run[*n_runs - 1] = *previousrl;
    }
}

/**
 * Like run_container_append but it is assumed that the content of run is empty.
 */
static inline rle16_t run_container_append_first(rle16_t *run,
        rle16_t vl, uint32_t * n_runs) {
    run[*n_runs] = vl;
    *n_runs = *n_runs + 1;
    return vl;
}

void print(const rle16_t *src, uint32_t length) {
    for(uint32_t i = 0; i < length; i++) {
        rle16_t current = src[i];
        printf("[%d,%d]",current.value,current.value+current.length);
    }
    printf("\n");
}

uint32_t run_container_union(const rle16_t *src_1, uint32_t length1,
                             const rle16_t *src_2, uint32_t length2, rle16_t *dst) {
    uint32_t n_runs = 0;
    int32_t rlepos = 0;
    int32_t xrlepos = 0;

    rle16_t previousrle;
    if (src_1[rlepos].value <= src_2[xrlepos].value) {
        previousrle = run_container_append_first(dst, src_1[rlepos],&n_runs);
        rlepos++;
    } else {
        previousrle = run_container_append_first(dst, src_2[xrlepos],&n_runs);
        xrlepos++;
    }

    while ((xrlepos < length2) && (rlepos < length1)) {
        rle16_t newrl;
        if (src_1[rlepos].value <= src_2[xrlepos].value) {
            newrl = src_1[rlepos];
            rlepos++;
        } else {
            newrl = src_2[xrlepos];
            xrlepos++;
        }
        run_container_append(dst, newrl, &previousrle,&n_runs);
    }
    while (xrlepos < length2) {
        run_container_append(dst, src_2[xrlepos], &previousrle,&n_runs);
        xrlepos++;
    }
    while (rlepos < length1) {
        run_container_append(dst, src_1[rlepos], &previousrle,&n_runs);
        rlepos++;
    }
    return n_runs;
}


uint32_t run_container_union_twopass(const rle16_t *src_1, uint32_t length1,
                                     const rle16_t *src_2, uint32_t length2, rle16_t *dst) {
    uint32_t n_runs = 0;
    int32_t rlepos = 0;
    int32_t xrlepos = 0;
    if((length1 == 0) || (length2 == 0)) {
        memcpy(dst, src_1, length1 * sizeof(rle16_t));
        memcpy(dst, src_2 , length2 * sizeof(rle16_t));
        return length1 + length2;
    }
    /**
    we proceed in two passes, first merge, then reduce
    */
    /**
    * first we merge
    */
    while(true) {
        if (src_1[rlepos].value <= src_2[xrlepos].value) {
            dst[n_runs++] = src_1[rlepos++];
            if(rlepos == length1)  {
                memcpy(dst + n_runs, src_2 + xrlepos, (length2-xrlepos)*sizeof(rle16_t));
                break;
            }
        } else {
            dst[n_runs++] = src_2[xrlepos++];
            if(xrlepos == length2) {
                memcpy(dst + n_runs, src_1 + rlepos, (length1-rlepos)*sizeof(rle16_t));
                break;
            }
        }
    }
    /**
    then we
    reduce
    */
    uint32_t max_runs = length1 + length2;
    n_runs = 1;
    rle16_t current = dst[0];
    for(uint32_t i = 1; i < max_runs; ++i) {
        rle16_t  trle = dst[i];
        if(trle.value > current.value + current.length + 1) {
            current = trle;
            dst[n_runs++] = trle;
        } else if (trle.value + trle.length > current.value + current.length) {
            current.length = trle.value + trle.length - current.value;
            dst[n_runs] = current;
        }
    }
    return n_runs;
}


int main() {
    uint32_t length1 = 1024;
    uint32_t length2 = 128;
    rle16_t *run1 = malloc(length1 * sizeof(rle16_t));
    rle16_t *run2 = malloc(length2 * sizeof(rle16_t));
    for(int k = 0; k < length1; k ++) {
        run1[k].value = k * 63;
        run1[k].length = (k * 63) % 5;
    }
    for(int k = 0; k < length2; k ++) {
        run2[k].value = k * 111;
        run2[k].length = (k * 111) % 55;
    }
    rle16_t *dst = malloc((length1 + length2) * sizeof(rle16_t));
    uint32_t answer = run_container_union(run1,length1,run2,length2,dst);
    uint32_t answer2 = run_container_union_twopass(run1,length1,run2,length2,dst);
    printf("%d %d \n",answer,answer2);
    assert(answer == answer2);
    uint32_t repeat = 5000;
    uint32_t size = length1 + length2;
    BEST_TIME(run_container_union(run1,length1,run2,length2,dst), answer, repeat, size);
    BEST_TIME(run_container_union_twopass(run1,length1,run2,length2,dst), answer, repeat, size);

    free(run1);
    free(run2);
    free(dst);
}
