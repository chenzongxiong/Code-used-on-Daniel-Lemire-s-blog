

#include <algorithm>
#include <climits>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <chrono>

#ifdef __clang__
#warning "LLVM is not recommended (does not generate cmov)"
#else
#ifndef __GNUG__
#warning "GNU G++ is recommended but not detected"
#endif
#endif

inline uint64_t splitmix64_stateless(uint64_t index) {
  uint64_t z = (index + UINT64_C(0x9E3779B97F4A7C15));
  z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
  z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
  return z ^ (z >> 31);
}

__attribute__((noinline)) size_t
element_access(const std::vector<std::vector<int>> &data,
               int *__restrict counter) {
  size_t hw = 0;
  for (size_t i = 0; i < data.size(); i++) {
    const int *__restrict z = data[i].data();
    const size_t len = data[i].size();
    for (size_t c = 0; c < len; c += 1) {
      *counter += z[c];
      hw++;
    }
  }
  return hw;
}

size_t branchy_search(const int *source, size_t n, int target) {
  size_t lo = 0;
  size_t hi = n;
  while (lo < hi) {
    size_t m = (lo + hi) / 2;
    if (target < source[m]) {
      hi = m;
    } else if (target > source[m]) {
      lo = m + 1;
    } else {
      return m;
    }
  }
  return hi;
}

__attribute__((noinline)) void
branchy(const std::vector<std::vector<int>> &data,
        const std::vector<int> &targets, std::vector<size_t> &solution) {
  for (size_t i = 0; i < targets.size(); i++) {
    solution[i] = branchy_search(data[i].data(), data[i].size(), targets[i]);
  }
}

size_t branchfree_search(const int *source, size_t n, int target) {
  if (n == 0)
    return 0;
  const int *base = source;
  while (n > 1) {
    size_t half = n >> 1;

    base = (base[half] < target) ? &base[half] : base;
    n -= half;
  }
  return (*base < target) + base - source;
}

__attribute__((noinline)) void
branchless(const std::vector<std::vector<int>> &data,
           const std::vector<int> &targets, std::vector<size_t> &solution) {
  for (size_t i = 0; i < targets.size(); i++) {
    solution[i] = branchfree_search(data[i].data(), data[i].size(), targets[i]);
  }
}

void binsearch2(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 2) {
      for(; i <= howmany - 2; i += 2) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch3(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 3) {
      for(; i <= howmany - 3; i += 3) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch4(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 4) {
      for(; i <= howmany - 4; i += 4) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch5(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 5) {
      for(; i <= howmany - 5; i += 5) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch6(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 6) {
      for(; i <= howmany - 6; i += 6) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch7(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 7) {
      for(; i <= howmany - 7; i += 7) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch8(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 8) {
      for(; i <= howmany - 8; i += 8) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch9(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 9) {
      for(; i <= howmany - 9; i += 9) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch10(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 10) {
      for(; i <= howmany - 10; i += 10) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch11(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 11) {
      for(; i <= howmany - 11; i += 11) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch12(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 12) {
      for(; i <= howmany - 12; i += 12) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch13(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 13) {
      for(; i <= howmany - 13; i += 13) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch14(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 14) {
      for(; i <= howmany - 14; i += 14) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch15(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 15) {
      for(; i <= howmany - 15; i += 15) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch16(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 16) {
      for(; i <= howmany - 16; i += 16) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch17(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 17) {
      for(; i <= howmany - 17; i += 17) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch18(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 18) {
      for(; i <= howmany - 18; i += 18) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch19(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 19) {
      for(; i <= howmany - 19; i += 19) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch20(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 20) {
      for(; i <= howmany - 20; i += 20) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch21(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 21) {
      for(; i <= howmany - 21; i += 21) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch22(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 22) {
      for(; i <= howmany - 22; i += 22) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch23(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 23) {
      for(; i <= howmany - 23; i += 23) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch24(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 24) {
      for(; i <= howmany - 24; i += 24) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch25(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 25) {
      for(; i <= howmany - 25; i += 25) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch26(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 26) {
      for(; i <= howmany - 26; i += 26) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch27(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 27) {
      for(; i <= howmany - 27; i += 27) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch28(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 28) {
      for(; i <= howmany - 28; i += 28) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch29(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 29) {
      for(; i <= howmany - 29; i += 29) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch30(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 30) {
      for(; i <= howmany - 30; i += 30) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch31(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 31) {
      for(; i <= howmany - 31; i += 31) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch32(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 32) {
      for(; i <= howmany - 32; i += 32) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch33(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 33) {
      for(; i <= howmany - 33; i += 33) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch34(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 34) {
      for(; i <= howmany - 34; i += 34) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch35(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 35) {
      for(; i <= howmany - 35; i += 35) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch36(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 36) {
      for(; i <= howmany - 36; i += 36) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch37(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 37) {
      for(; i <= howmany - 37; i += 37) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch38(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 38) {
      for(; i <= howmany - 38; i += 38) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch39(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 39) {
      for(; i <= howmany - 39; i += 39) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch40(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 40) {
      for(; i <= howmany - 40; i += 40) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch41(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 41) {
      for(; i <= howmany - 41; i += 41) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch42(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 42) {
      for(; i <= howmany - 42; i += 42) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch43(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 43) {
      for(; i <= howmany - 43; i += 43) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch44(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 44) {
      for(; i <= howmany - 44; i += 44) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch45(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 45) {
      for(; i <= howmany - 45; i += 45) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        const int * base44 = data[i + 44];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          base44 = (base44[half] < targets[i + 44]) ? &base44[half] : base44;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
        solutions[i + 44] = (base44[0] < targets[i + 44]) + base44 - data[i + 44];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch46(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 46) {
      for(; i <= howmany - 46; i += 46) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        const int * base44 = data[i + 44];
        const int * base45 = data[i + 45];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          base44 = (base44[half] < targets[i + 44]) ? &base44[half] : base44;
          base45 = (base45[half] < targets[i + 45]) ? &base45[half] : base45;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
        solutions[i + 44] = (base44[0] < targets[i + 44]) + base44 - data[i + 44];
        solutions[i + 45] = (base45[0] < targets[i + 45]) + base45 - data[i + 45];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch47(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 47) {
      for(; i <= howmany - 47; i += 47) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        const int * base44 = data[i + 44];
        const int * base45 = data[i + 45];
        const int * base46 = data[i + 46];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          base44 = (base44[half] < targets[i + 44]) ? &base44[half] : base44;
          base45 = (base45[half] < targets[i + 45]) ? &base45[half] : base45;
          base46 = (base46[half] < targets[i + 46]) ? &base46[half] : base46;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
        solutions[i + 44] = (base44[0] < targets[i + 44]) + base44 - data[i + 44];
        solutions[i + 45] = (base45[0] < targets[i + 45]) + base45 - data[i + 45];
        solutions[i + 46] = (base46[0] < targets[i + 46]) + base46 - data[i + 46];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch48(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 48) {
      for(; i <= howmany - 48; i += 48) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        const int * base44 = data[i + 44];
        const int * base45 = data[i + 45];
        const int * base46 = data[i + 46];
        const int * base47 = data[i + 47];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          base44 = (base44[half] < targets[i + 44]) ? &base44[half] : base44;
          base45 = (base45[half] < targets[i + 45]) ? &base45[half] : base45;
          base46 = (base46[half] < targets[i + 46]) ? &base46[half] : base46;
          base47 = (base47[half] < targets[i + 47]) ? &base47[half] : base47;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
        solutions[i + 44] = (base44[0] < targets[i + 44]) + base44 - data[i + 44];
        solutions[i + 45] = (base45[0] < targets[i + 45]) + base45 - data[i + 45];
        solutions[i + 46] = (base46[0] < targets[i + 46]) + base46 - data[i + 46];
        solutions[i + 47] = (base47[0] < targets[i + 47]) + base47 - data[i + 47];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch49(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 49) {
      for(; i <= howmany - 49; i += 49) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        const int * base44 = data[i + 44];
        const int * base45 = data[i + 45];
        const int * base46 = data[i + 46];
        const int * base47 = data[i + 47];
        const int * base48 = data[i + 48];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          base44 = (base44[half] < targets[i + 44]) ? &base44[half] : base44;
          base45 = (base45[half] < targets[i + 45]) ? &base45[half] : base45;
          base46 = (base46[half] < targets[i + 46]) ? &base46[half] : base46;
          base47 = (base47[half] < targets[i + 47]) ? &base47[half] : base47;
          base48 = (base48[half] < targets[i + 48]) ? &base48[half] : base48;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
        solutions[i + 44] = (base44[0] < targets[i + 44]) + base44 - data[i + 44];
        solutions[i + 45] = (base45[0] < targets[i + 45]) + base45 - data[i + 45];
        solutions[i + 46] = (base46[0] < targets[i + 46]) + base46 - data[i + 46];
        solutions[i + 47] = (base47[0] < targets[i + 47]) + base47 - data[i + 47];
        solutions[i + 48] = (base48[0] < targets[i + 48]) + base48 - data[i + 48];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch50(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 50) {
      for(; i <= howmany - 50; i += 50) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        const int * base44 = data[i + 44];
        const int * base45 = data[i + 45];
        const int * base46 = data[i + 46];
        const int * base47 = data[i + 47];
        const int * base48 = data[i + 48];
        const int * base49 = data[i + 49];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          base44 = (base44[half] < targets[i + 44]) ? &base44[half] : base44;
          base45 = (base45[half] < targets[i + 45]) ? &base45[half] : base45;
          base46 = (base46[half] < targets[i + 46]) ? &base46[half] : base46;
          base47 = (base47[half] < targets[i + 47]) ? &base47[half] : base47;
          base48 = (base48[half] < targets[i + 48]) ? &base48[half] : base48;
          base49 = (base49[half] < targets[i + 49]) ? &base49[half] : base49;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
        solutions[i + 44] = (base44[0] < targets[i + 44]) + base44 - data[i + 44];
        solutions[i + 45] = (base45[0] < targets[i + 45]) + base45 - data[i + 45];
        solutions[i + 46] = (base46[0] < targets[i + 46]) + base46 - data[i + 46];
        solutions[i + 47] = (base47[0] < targets[i + 47]) + base47 - data[i + 47];
        solutions[i + 48] = (base48[0] < targets[i + 48]) + base48 - data[i + 48];
        solutions[i + 49] = (base49[0] < targets[i + 49]) + base49 - data[i + 49];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch51(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 51) {
      for(; i <= howmany - 51; i += 51) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        const int * base44 = data[i + 44];
        const int * base45 = data[i + 45];
        const int * base46 = data[i + 46];
        const int * base47 = data[i + 47];
        const int * base48 = data[i + 48];
        const int * base49 = data[i + 49];
        const int * base50 = data[i + 50];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          base44 = (base44[half] < targets[i + 44]) ? &base44[half] : base44;
          base45 = (base45[half] < targets[i + 45]) ? &base45[half] : base45;
          base46 = (base46[half] < targets[i + 46]) ? &base46[half] : base46;
          base47 = (base47[half] < targets[i + 47]) ? &base47[half] : base47;
          base48 = (base48[half] < targets[i + 48]) ? &base48[half] : base48;
          base49 = (base49[half] < targets[i + 49]) ? &base49[half] : base49;
          base50 = (base50[half] < targets[i + 50]) ? &base50[half] : base50;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
        solutions[i + 44] = (base44[0] < targets[i + 44]) + base44 - data[i + 44];
        solutions[i + 45] = (base45[0] < targets[i + 45]) + base45 - data[i + 45];
        solutions[i + 46] = (base46[0] < targets[i + 46]) + base46 - data[i + 46];
        solutions[i + 47] = (base47[0] < targets[i + 47]) + base47 - data[i + 47];
        solutions[i + 48] = (base48[0] < targets[i + 48]) + base48 - data[i + 48];
        solutions[i + 49] = (base49[0] < targets[i + 49]) + base49 - data[i + 49];
        solutions[i + 50] = (base50[0] < targets[i + 50]) + base50 - data[i + 50];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch52(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 52) {
      for(; i <= howmany - 52; i += 52) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        const int * base44 = data[i + 44];
        const int * base45 = data[i + 45];
        const int * base46 = data[i + 46];
        const int * base47 = data[i + 47];
        const int * base48 = data[i + 48];
        const int * base49 = data[i + 49];
        const int * base50 = data[i + 50];
        const int * base51 = data[i + 51];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          base44 = (base44[half] < targets[i + 44]) ? &base44[half] : base44;
          base45 = (base45[half] < targets[i + 45]) ? &base45[half] : base45;
          base46 = (base46[half] < targets[i + 46]) ? &base46[half] : base46;
          base47 = (base47[half] < targets[i + 47]) ? &base47[half] : base47;
          base48 = (base48[half] < targets[i + 48]) ? &base48[half] : base48;
          base49 = (base49[half] < targets[i + 49]) ? &base49[half] : base49;
          base50 = (base50[half] < targets[i + 50]) ? &base50[half] : base50;
          base51 = (base51[half] < targets[i + 51]) ? &base51[half] : base51;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
        solutions[i + 44] = (base44[0] < targets[i + 44]) + base44 - data[i + 44];
        solutions[i + 45] = (base45[0] < targets[i + 45]) + base45 - data[i + 45];
        solutions[i + 46] = (base46[0] < targets[i + 46]) + base46 - data[i + 46];
        solutions[i + 47] = (base47[0] < targets[i + 47]) + base47 - data[i + 47];
        solutions[i + 48] = (base48[0] < targets[i + 48]) + base48 - data[i + 48];
        solutions[i + 49] = (base49[0] < targets[i + 49]) + base49 - data[i + 49];
        solutions[i + 50] = (base50[0] < targets[i + 50]) + base50 - data[i + 50];
        solutions[i + 51] = (base51[0] < targets[i + 51]) + base51 - data[i + 51];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch53(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 53) {
      for(; i <= howmany - 53; i += 53) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        const int * base44 = data[i + 44];
        const int * base45 = data[i + 45];
        const int * base46 = data[i + 46];
        const int * base47 = data[i + 47];
        const int * base48 = data[i + 48];
        const int * base49 = data[i + 49];
        const int * base50 = data[i + 50];
        const int * base51 = data[i + 51];
        const int * base52 = data[i + 52];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          base44 = (base44[half] < targets[i + 44]) ? &base44[half] : base44;
          base45 = (base45[half] < targets[i + 45]) ? &base45[half] : base45;
          base46 = (base46[half] < targets[i + 46]) ? &base46[half] : base46;
          base47 = (base47[half] < targets[i + 47]) ? &base47[half] : base47;
          base48 = (base48[half] < targets[i + 48]) ? &base48[half] : base48;
          base49 = (base49[half] < targets[i + 49]) ? &base49[half] : base49;
          base50 = (base50[half] < targets[i + 50]) ? &base50[half] : base50;
          base51 = (base51[half] < targets[i + 51]) ? &base51[half] : base51;
          base52 = (base52[half] < targets[i + 52]) ? &base52[half] : base52;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
        solutions[i + 44] = (base44[0] < targets[i + 44]) + base44 - data[i + 44];
        solutions[i + 45] = (base45[0] < targets[i + 45]) + base45 - data[i + 45];
        solutions[i + 46] = (base46[0] < targets[i + 46]) + base46 - data[i + 46];
        solutions[i + 47] = (base47[0] < targets[i + 47]) + base47 - data[i + 47];
        solutions[i + 48] = (base48[0] < targets[i + 48]) + base48 - data[i + 48];
        solutions[i + 49] = (base49[0] < targets[i + 49]) + base49 - data[i + 49];
        solutions[i + 50] = (base50[0] < targets[i + 50]) + base50 - data[i + 50];
        solutions[i + 51] = (base51[0] < targets[i + 51]) + base51 - data[i + 51];
        solutions[i + 52] = (base52[0] < targets[i + 52]) + base52 - data[i + 52];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch54(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 54) {
      for(; i <= howmany - 54; i += 54) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        const int * base44 = data[i + 44];
        const int * base45 = data[i + 45];
        const int * base46 = data[i + 46];
        const int * base47 = data[i + 47];
        const int * base48 = data[i + 48];
        const int * base49 = data[i + 49];
        const int * base50 = data[i + 50];
        const int * base51 = data[i + 51];
        const int * base52 = data[i + 52];
        const int * base53 = data[i + 53];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          base44 = (base44[half] < targets[i + 44]) ? &base44[half] : base44;
          base45 = (base45[half] < targets[i + 45]) ? &base45[half] : base45;
          base46 = (base46[half] < targets[i + 46]) ? &base46[half] : base46;
          base47 = (base47[half] < targets[i + 47]) ? &base47[half] : base47;
          base48 = (base48[half] < targets[i + 48]) ? &base48[half] : base48;
          base49 = (base49[half] < targets[i + 49]) ? &base49[half] : base49;
          base50 = (base50[half] < targets[i + 50]) ? &base50[half] : base50;
          base51 = (base51[half] < targets[i + 51]) ? &base51[half] : base51;
          base52 = (base52[half] < targets[i + 52]) ? &base52[half] : base52;
          base53 = (base53[half] < targets[i + 53]) ? &base53[half] : base53;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
        solutions[i + 44] = (base44[0] < targets[i + 44]) + base44 - data[i + 44];
        solutions[i + 45] = (base45[0] < targets[i + 45]) + base45 - data[i + 45];
        solutions[i + 46] = (base46[0] < targets[i + 46]) + base46 - data[i + 46];
        solutions[i + 47] = (base47[0] < targets[i + 47]) + base47 - data[i + 47];
        solutions[i + 48] = (base48[0] < targets[i + 48]) + base48 - data[i + 48];
        solutions[i + 49] = (base49[0] < targets[i + 49]) + base49 - data[i + 49];
        solutions[i + 50] = (base50[0] < targets[i + 50]) + base50 - data[i + 50];
        solutions[i + 51] = (base51[0] < targets[i + 51]) + base51 - data[i + 51];
        solutions[i + 52] = (base52[0] < targets[i + 52]) + base52 - data[i + 52];
        solutions[i + 53] = (base53[0] < targets[i + 53]) + base53 - data[i + 53];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch55(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 55) {
      for(; i <= howmany - 55; i += 55) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        const int * base44 = data[i + 44];
        const int * base45 = data[i + 45];
        const int * base46 = data[i + 46];
        const int * base47 = data[i + 47];
        const int * base48 = data[i + 48];
        const int * base49 = data[i + 49];
        const int * base50 = data[i + 50];
        const int * base51 = data[i + 51];
        const int * base52 = data[i + 52];
        const int * base53 = data[i + 53];
        const int * base54 = data[i + 54];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          base44 = (base44[half] < targets[i + 44]) ? &base44[half] : base44;
          base45 = (base45[half] < targets[i + 45]) ? &base45[half] : base45;
          base46 = (base46[half] < targets[i + 46]) ? &base46[half] : base46;
          base47 = (base47[half] < targets[i + 47]) ? &base47[half] : base47;
          base48 = (base48[half] < targets[i + 48]) ? &base48[half] : base48;
          base49 = (base49[half] < targets[i + 49]) ? &base49[half] : base49;
          base50 = (base50[half] < targets[i + 50]) ? &base50[half] : base50;
          base51 = (base51[half] < targets[i + 51]) ? &base51[half] : base51;
          base52 = (base52[half] < targets[i + 52]) ? &base52[half] : base52;
          base53 = (base53[half] < targets[i + 53]) ? &base53[half] : base53;
          base54 = (base54[half] < targets[i + 54]) ? &base54[half] : base54;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
        solutions[i + 44] = (base44[0] < targets[i + 44]) + base44 - data[i + 44];
        solutions[i + 45] = (base45[0] < targets[i + 45]) + base45 - data[i + 45];
        solutions[i + 46] = (base46[0] < targets[i + 46]) + base46 - data[i + 46];
        solutions[i + 47] = (base47[0] < targets[i + 47]) + base47 - data[i + 47];
        solutions[i + 48] = (base48[0] < targets[i + 48]) + base48 - data[i + 48];
        solutions[i + 49] = (base49[0] < targets[i + 49]) + base49 - data[i + 49];
        solutions[i + 50] = (base50[0] < targets[i + 50]) + base50 - data[i + 50];
        solutions[i + 51] = (base51[0] < targets[i + 51]) + base51 - data[i + 51];
        solutions[i + 52] = (base52[0] < targets[i + 52]) + base52 - data[i + 52];
        solutions[i + 53] = (base53[0] < targets[i + 53]) + base53 - data[i + 53];
        solutions[i + 54] = (base54[0] < targets[i + 54]) + base54 - data[i + 54];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch56(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 56) {
      for(; i <= howmany - 56; i += 56) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        const int * base44 = data[i + 44];
        const int * base45 = data[i + 45];
        const int * base46 = data[i + 46];
        const int * base47 = data[i + 47];
        const int * base48 = data[i + 48];
        const int * base49 = data[i + 49];
        const int * base50 = data[i + 50];
        const int * base51 = data[i + 51];
        const int * base52 = data[i + 52];
        const int * base53 = data[i + 53];
        const int * base54 = data[i + 54];
        const int * base55 = data[i + 55];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          base44 = (base44[half] < targets[i + 44]) ? &base44[half] : base44;
          base45 = (base45[half] < targets[i + 45]) ? &base45[half] : base45;
          base46 = (base46[half] < targets[i + 46]) ? &base46[half] : base46;
          base47 = (base47[half] < targets[i + 47]) ? &base47[half] : base47;
          base48 = (base48[half] < targets[i + 48]) ? &base48[half] : base48;
          base49 = (base49[half] < targets[i + 49]) ? &base49[half] : base49;
          base50 = (base50[half] < targets[i + 50]) ? &base50[half] : base50;
          base51 = (base51[half] < targets[i + 51]) ? &base51[half] : base51;
          base52 = (base52[half] < targets[i + 52]) ? &base52[half] : base52;
          base53 = (base53[half] < targets[i + 53]) ? &base53[half] : base53;
          base54 = (base54[half] < targets[i + 54]) ? &base54[half] : base54;
          base55 = (base55[half] < targets[i + 55]) ? &base55[half] : base55;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
        solutions[i + 44] = (base44[0] < targets[i + 44]) + base44 - data[i + 44];
        solutions[i + 45] = (base45[0] < targets[i + 45]) + base45 - data[i + 45];
        solutions[i + 46] = (base46[0] < targets[i + 46]) + base46 - data[i + 46];
        solutions[i + 47] = (base47[0] < targets[i + 47]) + base47 - data[i + 47];
        solutions[i + 48] = (base48[0] < targets[i + 48]) + base48 - data[i + 48];
        solutions[i + 49] = (base49[0] < targets[i + 49]) + base49 - data[i + 49];
        solutions[i + 50] = (base50[0] < targets[i + 50]) + base50 - data[i + 50];
        solutions[i + 51] = (base51[0] < targets[i + 51]) + base51 - data[i + 51];
        solutions[i + 52] = (base52[0] < targets[i + 52]) + base52 - data[i + 52];
        solutions[i + 53] = (base53[0] < targets[i + 53]) + base53 - data[i + 53];
        solutions[i + 54] = (base54[0] < targets[i + 54]) + base54 - data[i + 54];
        solutions[i + 55] = (base55[0] < targets[i + 55]) + base55 - data[i + 55];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch57(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 57) {
      for(; i <= howmany - 57; i += 57) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        const int * base44 = data[i + 44];
        const int * base45 = data[i + 45];
        const int * base46 = data[i + 46];
        const int * base47 = data[i + 47];
        const int * base48 = data[i + 48];
        const int * base49 = data[i + 49];
        const int * base50 = data[i + 50];
        const int * base51 = data[i + 51];
        const int * base52 = data[i + 52];
        const int * base53 = data[i + 53];
        const int * base54 = data[i + 54];
        const int * base55 = data[i + 55];
        const int * base56 = data[i + 56];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          base44 = (base44[half] < targets[i + 44]) ? &base44[half] : base44;
          base45 = (base45[half] < targets[i + 45]) ? &base45[half] : base45;
          base46 = (base46[half] < targets[i + 46]) ? &base46[half] : base46;
          base47 = (base47[half] < targets[i + 47]) ? &base47[half] : base47;
          base48 = (base48[half] < targets[i + 48]) ? &base48[half] : base48;
          base49 = (base49[half] < targets[i + 49]) ? &base49[half] : base49;
          base50 = (base50[half] < targets[i + 50]) ? &base50[half] : base50;
          base51 = (base51[half] < targets[i + 51]) ? &base51[half] : base51;
          base52 = (base52[half] < targets[i + 52]) ? &base52[half] : base52;
          base53 = (base53[half] < targets[i + 53]) ? &base53[half] : base53;
          base54 = (base54[half] < targets[i + 54]) ? &base54[half] : base54;
          base55 = (base55[half] < targets[i + 55]) ? &base55[half] : base55;
          base56 = (base56[half] < targets[i + 56]) ? &base56[half] : base56;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
        solutions[i + 44] = (base44[0] < targets[i + 44]) + base44 - data[i + 44];
        solutions[i + 45] = (base45[0] < targets[i + 45]) + base45 - data[i + 45];
        solutions[i + 46] = (base46[0] < targets[i + 46]) + base46 - data[i + 46];
        solutions[i + 47] = (base47[0] < targets[i + 47]) + base47 - data[i + 47];
        solutions[i + 48] = (base48[0] < targets[i + 48]) + base48 - data[i + 48];
        solutions[i + 49] = (base49[0] < targets[i + 49]) + base49 - data[i + 49];
        solutions[i + 50] = (base50[0] < targets[i + 50]) + base50 - data[i + 50];
        solutions[i + 51] = (base51[0] < targets[i + 51]) + base51 - data[i + 51];
        solutions[i + 52] = (base52[0] < targets[i + 52]) + base52 - data[i + 52];
        solutions[i + 53] = (base53[0] < targets[i + 53]) + base53 - data[i + 53];
        solutions[i + 54] = (base54[0] < targets[i + 54]) + base54 - data[i + 54];
        solutions[i + 55] = (base55[0] < targets[i + 55]) + base55 - data[i + 55];
        solutions[i + 56] = (base56[0] < targets[i + 56]) + base56 - data[i + 56];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch58(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 58) {
      for(; i <= howmany - 58; i += 58) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        const int * base44 = data[i + 44];
        const int * base45 = data[i + 45];
        const int * base46 = data[i + 46];
        const int * base47 = data[i + 47];
        const int * base48 = data[i + 48];
        const int * base49 = data[i + 49];
        const int * base50 = data[i + 50];
        const int * base51 = data[i + 51];
        const int * base52 = data[i + 52];
        const int * base53 = data[i + 53];
        const int * base54 = data[i + 54];
        const int * base55 = data[i + 55];
        const int * base56 = data[i + 56];
        const int * base57 = data[i + 57];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          base44 = (base44[half] < targets[i + 44]) ? &base44[half] : base44;
          base45 = (base45[half] < targets[i + 45]) ? &base45[half] : base45;
          base46 = (base46[half] < targets[i + 46]) ? &base46[half] : base46;
          base47 = (base47[half] < targets[i + 47]) ? &base47[half] : base47;
          base48 = (base48[half] < targets[i + 48]) ? &base48[half] : base48;
          base49 = (base49[half] < targets[i + 49]) ? &base49[half] : base49;
          base50 = (base50[half] < targets[i + 50]) ? &base50[half] : base50;
          base51 = (base51[half] < targets[i + 51]) ? &base51[half] : base51;
          base52 = (base52[half] < targets[i + 52]) ? &base52[half] : base52;
          base53 = (base53[half] < targets[i + 53]) ? &base53[half] : base53;
          base54 = (base54[half] < targets[i + 54]) ? &base54[half] : base54;
          base55 = (base55[half] < targets[i + 55]) ? &base55[half] : base55;
          base56 = (base56[half] < targets[i + 56]) ? &base56[half] : base56;
          base57 = (base57[half] < targets[i + 57]) ? &base57[half] : base57;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
        solutions[i + 44] = (base44[0] < targets[i + 44]) + base44 - data[i + 44];
        solutions[i + 45] = (base45[0] < targets[i + 45]) + base45 - data[i + 45];
        solutions[i + 46] = (base46[0] < targets[i + 46]) + base46 - data[i + 46];
        solutions[i + 47] = (base47[0] < targets[i + 47]) + base47 - data[i + 47];
        solutions[i + 48] = (base48[0] < targets[i + 48]) + base48 - data[i + 48];
        solutions[i + 49] = (base49[0] < targets[i + 49]) + base49 - data[i + 49];
        solutions[i + 50] = (base50[0] < targets[i + 50]) + base50 - data[i + 50];
        solutions[i + 51] = (base51[0] < targets[i + 51]) + base51 - data[i + 51];
        solutions[i + 52] = (base52[0] < targets[i + 52]) + base52 - data[i + 52];
        solutions[i + 53] = (base53[0] < targets[i + 53]) + base53 - data[i + 53];
        solutions[i + 54] = (base54[0] < targets[i + 54]) + base54 - data[i + 54];
        solutions[i + 55] = (base55[0] < targets[i + 55]) + base55 - data[i + 55];
        solutions[i + 56] = (base56[0] < targets[i + 56]) + base56 - data[i + 56];
        solutions[i + 57] = (base57[0] < targets[i + 57]) + base57 - data[i + 57];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch59(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 59) {
      for(; i <= howmany - 59; i += 59) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        const int * base44 = data[i + 44];
        const int * base45 = data[i + 45];
        const int * base46 = data[i + 46];
        const int * base47 = data[i + 47];
        const int * base48 = data[i + 48];
        const int * base49 = data[i + 49];
        const int * base50 = data[i + 50];
        const int * base51 = data[i + 51];
        const int * base52 = data[i + 52];
        const int * base53 = data[i + 53];
        const int * base54 = data[i + 54];
        const int * base55 = data[i + 55];
        const int * base56 = data[i + 56];
        const int * base57 = data[i + 57];
        const int * base58 = data[i + 58];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          base44 = (base44[half] < targets[i + 44]) ? &base44[half] : base44;
          base45 = (base45[half] < targets[i + 45]) ? &base45[half] : base45;
          base46 = (base46[half] < targets[i + 46]) ? &base46[half] : base46;
          base47 = (base47[half] < targets[i + 47]) ? &base47[half] : base47;
          base48 = (base48[half] < targets[i + 48]) ? &base48[half] : base48;
          base49 = (base49[half] < targets[i + 49]) ? &base49[half] : base49;
          base50 = (base50[half] < targets[i + 50]) ? &base50[half] : base50;
          base51 = (base51[half] < targets[i + 51]) ? &base51[half] : base51;
          base52 = (base52[half] < targets[i + 52]) ? &base52[half] : base52;
          base53 = (base53[half] < targets[i + 53]) ? &base53[half] : base53;
          base54 = (base54[half] < targets[i + 54]) ? &base54[half] : base54;
          base55 = (base55[half] < targets[i + 55]) ? &base55[half] : base55;
          base56 = (base56[half] < targets[i + 56]) ? &base56[half] : base56;
          base57 = (base57[half] < targets[i + 57]) ? &base57[half] : base57;
          base58 = (base58[half] < targets[i + 58]) ? &base58[half] : base58;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
        solutions[i + 44] = (base44[0] < targets[i + 44]) + base44 - data[i + 44];
        solutions[i + 45] = (base45[0] < targets[i + 45]) + base45 - data[i + 45];
        solutions[i + 46] = (base46[0] < targets[i + 46]) + base46 - data[i + 46];
        solutions[i + 47] = (base47[0] < targets[i + 47]) + base47 - data[i + 47];
        solutions[i + 48] = (base48[0] < targets[i + 48]) + base48 - data[i + 48];
        solutions[i + 49] = (base49[0] < targets[i + 49]) + base49 - data[i + 49];
        solutions[i + 50] = (base50[0] < targets[i + 50]) + base50 - data[i + 50];
        solutions[i + 51] = (base51[0] < targets[i + 51]) + base51 - data[i + 51];
        solutions[i + 52] = (base52[0] < targets[i + 52]) + base52 - data[i + 52];
        solutions[i + 53] = (base53[0] < targets[i + 53]) + base53 - data[i + 53];
        solutions[i + 54] = (base54[0] < targets[i + 54]) + base54 - data[i + 54];
        solutions[i + 55] = (base55[0] < targets[i + 55]) + base55 - data[i + 55];
        solutions[i + 56] = (base56[0] < targets[i + 56]) + base56 - data[i + 56];
        solutions[i + 57] = (base57[0] < targets[i + 57]) + base57 - data[i + 57];
        solutions[i + 58] = (base58[0] < targets[i + 58]) + base58 - data[i + 58];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch60(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 60) {
      for(; i <= howmany - 60; i += 60) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        const int * base44 = data[i + 44];
        const int * base45 = data[i + 45];
        const int * base46 = data[i + 46];
        const int * base47 = data[i + 47];
        const int * base48 = data[i + 48];
        const int * base49 = data[i + 49];
        const int * base50 = data[i + 50];
        const int * base51 = data[i + 51];
        const int * base52 = data[i + 52];
        const int * base53 = data[i + 53];
        const int * base54 = data[i + 54];
        const int * base55 = data[i + 55];
        const int * base56 = data[i + 56];
        const int * base57 = data[i + 57];
        const int * base58 = data[i + 58];
        const int * base59 = data[i + 59];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          base44 = (base44[half] < targets[i + 44]) ? &base44[half] : base44;
          base45 = (base45[half] < targets[i + 45]) ? &base45[half] : base45;
          base46 = (base46[half] < targets[i + 46]) ? &base46[half] : base46;
          base47 = (base47[half] < targets[i + 47]) ? &base47[half] : base47;
          base48 = (base48[half] < targets[i + 48]) ? &base48[half] : base48;
          base49 = (base49[half] < targets[i + 49]) ? &base49[half] : base49;
          base50 = (base50[half] < targets[i + 50]) ? &base50[half] : base50;
          base51 = (base51[half] < targets[i + 51]) ? &base51[half] : base51;
          base52 = (base52[half] < targets[i + 52]) ? &base52[half] : base52;
          base53 = (base53[half] < targets[i + 53]) ? &base53[half] : base53;
          base54 = (base54[half] < targets[i + 54]) ? &base54[half] : base54;
          base55 = (base55[half] < targets[i + 55]) ? &base55[half] : base55;
          base56 = (base56[half] < targets[i + 56]) ? &base56[half] : base56;
          base57 = (base57[half] < targets[i + 57]) ? &base57[half] : base57;
          base58 = (base58[half] < targets[i + 58]) ? &base58[half] : base58;
          base59 = (base59[half] < targets[i + 59]) ? &base59[half] : base59;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
        solutions[i + 44] = (base44[0] < targets[i + 44]) + base44 - data[i + 44];
        solutions[i + 45] = (base45[0] < targets[i + 45]) + base45 - data[i + 45];
        solutions[i + 46] = (base46[0] < targets[i + 46]) + base46 - data[i + 46];
        solutions[i + 47] = (base47[0] < targets[i + 47]) + base47 - data[i + 47];
        solutions[i + 48] = (base48[0] < targets[i + 48]) + base48 - data[i + 48];
        solutions[i + 49] = (base49[0] < targets[i + 49]) + base49 - data[i + 49];
        solutions[i + 50] = (base50[0] < targets[i + 50]) + base50 - data[i + 50];
        solutions[i + 51] = (base51[0] < targets[i + 51]) + base51 - data[i + 51];
        solutions[i + 52] = (base52[0] < targets[i + 52]) + base52 - data[i + 52];
        solutions[i + 53] = (base53[0] < targets[i + 53]) + base53 - data[i + 53];
        solutions[i + 54] = (base54[0] < targets[i + 54]) + base54 - data[i + 54];
        solutions[i + 55] = (base55[0] < targets[i + 55]) + base55 - data[i + 55];
        solutions[i + 56] = (base56[0] < targets[i + 56]) + base56 - data[i + 56];
        solutions[i + 57] = (base57[0] < targets[i + 57]) + base57 - data[i + 57];
        solutions[i + 58] = (base58[0] < targets[i + 58]) + base58 - data[i + 58];
        solutions[i + 59] = (base59[0] < targets[i + 59]) + base59 - data[i + 59];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch61(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 61) {
      for(; i <= howmany - 61; i += 61) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        const int * base44 = data[i + 44];
        const int * base45 = data[i + 45];
        const int * base46 = data[i + 46];
        const int * base47 = data[i + 47];
        const int * base48 = data[i + 48];
        const int * base49 = data[i + 49];
        const int * base50 = data[i + 50];
        const int * base51 = data[i + 51];
        const int * base52 = data[i + 52];
        const int * base53 = data[i + 53];
        const int * base54 = data[i + 54];
        const int * base55 = data[i + 55];
        const int * base56 = data[i + 56];
        const int * base57 = data[i + 57];
        const int * base58 = data[i + 58];
        const int * base59 = data[i + 59];
        const int * base60 = data[i + 60];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          base44 = (base44[half] < targets[i + 44]) ? &base44[half] : base44;
          base45 = (base45[half] < targets[i + 45]) ? &base45[half] : base45;
          base46 = (base46[half] < targets[i + 46]) ? &base46[half] : base46;
          base47 = (base47[half] < targets[i + 47]) ? &base47[half] : base47;
          base48 = (base48[half] < targets[i + 48]) ? &base48[half] : base48;
          base49 = (base49[half] < targets[i + 49]) ? &base49[half] : base49;
          base50 = (base50[half] < targets[i + 50]) ? &base50[half] : base50;
          base51 = (base51[half] < targets[i + 51]) ? &base51[half] : base51;
          base52 = (base52[half] < targets[i + 52]) ? &base52[half] : base52;
          base53 = (base53[half] < targets[i + 53]) ? &base53[half] : base53;
          base54 = (base54[half] < targets[i + 54]) ? &base54[half] : base54;
          base55 = (base55[half] < targets[i + 55]) ? &base55[half] : base55;
          base56 = (base56[half] < targets[i + 56]) ? &base56[half] : base56;
          base57 = (base57[half] < targets[i + 57]) ? &base57[half] : base57;
          base58 = (base58[half] < targets[i + 58]) ? &base58[half] : base58;
          base59 = (base59[half] < targets[i + 59]) ? &base59[half] : base59;
          base60 = (base60[half] < targets[i + 60]) ? &base60[half] : base60;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
        solutions[i + 44] = (base44[0] < targets[i + 44]) + base44 - data[i + 44];
        solutions[i + 45] = (base45[0] < targets[i + 45]) + base45 - data[i + 45];
        solutions[i + 46] = (base46[0] < targets[i + 46]) + base46 - data[i + 46];
        solutions[i + 47] = (base47[0] < targets[i + 47]) + base47 - data[i + 47];
        solutions[i + 48] = (base48[0] < targets[i + 48]) + base48 - data[i + 48];
        solutions[i + 49] = (base49[0] < targets[i + 49]) + base49 - data[i + 49];
        solutions[i + 50] = (base50[0] < targets[i + 50]) + base50 - data[i + 50];
        solutions[i + 51] = (base51[0] < targets[i + 51]) + base51 - data[i + 51];
        solutions[i + 52] = (base52[0] < targets[i + 52]) + base52 - data[i + 52];
        solutions[i + 53] = (base53[0] < targets[i + 53]) + base53 - data[i + 53];
        solutions[i + 54] = (base54[0] < targets[i + 54]) + base54 - data[i + 54];
        solutions[i + 55] = (base55[0] < targets[i + 55]) + base55 - data[i + 55];
        solutions[i + 56] = (base56[0] < targets[i + 56]) + base56 - data[i + 56];
        solutions[i + 57] = (base57[0] < targets[i + 57]) + base57 - data[i + 57];
        solutions[i + 58] = (base58[0] < targets[i + 58]) + base58 - data[i + 58];
        solutions[i + 59] = (base59[0] < targets[i + 59]) + base59 - data[i + 59];
        solutions[i + 60] = (base60[0] < targets[i + 60]) + base60 - data[i + 60];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch62(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 62) {
      for(; i <= howmany - 62; i += 62) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        const int * base44 = data[i + 44];
        const int * base45 = data[i + 45];
        const int * base46 = data[i + 46];
        const int * base47 = data[i + 47];
        const int * base48 = data[i + 48];
        const int * base49 = data[i + 49];
        const int * base50 = data[i + 50];
        const int * base51 = data[i + 51];
        const int * base52 = data[i + 52];
        const int * base53 = data[i + 53];
        const int * base54 = data[i + 54];
        const int * base55 = data[i + 55];
        const int * base56 = data[i + 56];
        const int * base57 = data[i + 57];
        const int * base58 = data[i + 58];
        const int * base59 = data[i + 59];
        const int * base60 = data[i + 60];
        const int * base61 = data[i + 61];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          base44 = (base44[half] < targets[i + 44]) ? &base44[half] : base44;
          base45 = (base45[half] < targets[i + 45]) ? &base45[half] : base45;
          base46 = (base46[half] < targets[i + 46]) ? &base46[half] : base46;
          base47 = (base47[half] < targets[i + 47]) ? &base47[half] : base47;
          base48 = (base48[half] < targets[i + 48]) ? &base48[half] : base48;
          base49 = (base49[half] < targets[i + 49]) ? &base49[half] : base49;
          base50 = (base50[half] < targets[i + 50]) ? &base50[half] : base50;
          base51 = (base51[half] < targets[i + 51]) ? &base51[half] : base51;
          base52 = (base52[half] < targets[i + 52]) ? &base52[half] : base52;
          base53 = (base53[half] < targets[i + 53]) ? &base53[half] : base53;
          base54 = (base54[half] < targets[i + 54]) ? &base54[half] : base54;
          base55 = (base55[half] < targets[i + 55]) ? &base55[half] : base55;
          base56 = (base56[half] < targets[i + 56]) ? &base56[half] : base56;
          base57 = (base57[half] < targets[i + 57]) ? &base57[half] : base57;
          base58 = (base58[half] < targets[i + 58]) ? &base58[half] : base58;
          base59 = (base59[half] < targets[i + 59]) ? &base59[half] : base59;
          base60 = (base60[half] < targets[i + 60]) ? &base60[half] : base60;
          base61 = (base61[half] < targets[i + 61]) ? &base61[half] : base61;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
        solutions[i + 44] = (base44[0] < targets[i + 44]) + base44 - data[i + 44];
        solutions[i + 45] = (base45[0] < targets[i + 45]) + base45 - data[i + 45];
        solutions[i + 46] = (base46[0] < targets[i + 46]) + base46 - data[i + 46];
        solutions[i + 47] = (base47[0] < targets[i + 47]) + base47 - data[i + 47];
        solutions[i + 48] = (base48[0] < targets[i + 48]) + base48 - data[i + 48];
        solutions[i + 49] = (base49[0] < targets[i + 49]) + base49 - data[i + 49];
        solutions[i + 50] = (base50[0] < targets[i + 50]) + base50 - data[i + 50];
        solutions[i + 51] = (base51[0] < targets[i + 51]) + base51 - data[i + 51];
        solutions[i + 52] = (base52[0] < targets[i + 52]) + base52 - data[i + 52];
        solutions[i + 53] = (base53[0] < targets[i + 53]) + base53 - data[i + 53];
        solutions[i + 54] = (base54[0] < targets[i + 54]) + base54 - data[i + 54];
        solutions[i + 55] = (base55[0] < targets[i + 55]) + base55 - data[i + 55];
        solutions[i + 56] = (base56[0] < targets[i + 56]) + base56 - data[i + 56];
        solutions[i + 57] = (base57[0] < targets[i + 57]) + base57 - data[i + 57];
        solutions[i + 58] = (base58[0] < targets[i + 58]) + base58 - data[i + 58];
        solutions[i + 59] = (base59[0] < targets[i + 59]) + base59 - data[i + 59];
        solutions[i + 60] = (base60[0] < targets[i + 60]) + base60 - data[i + 60];
        solutions[i + 61] = (base61[0] < targets[i + 61]) + base61 - data[i + 61];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch63(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 63) {
      for(; i <= howmany - 63; i += 63) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        const int * base44 = data[i + 44];
        const int * base45 = data[i + 45];
        const int * base46 = data[i + 46];
        const int * base47 = data[i + 47];
        const int * base48 = data[i + 48];
        const int * base49 = data[i + 49];
        const int * base50 = data[i + 50];
        const int * base51 = data[i + 51];
        const int * base52 = data[i + 52];
        const int * base53 = data[i + 53];
        const int * base54 = data[i + 54];
        const int * base55 = data[i + 55];
        const int * base56 = data[i + 56];
        const int * base57 = data[i + 57];
        const int * base58 = data[i + 58];
        const int * base59 = data[i + 59];
        const int * base60 = data[i + 60];
        const int * base61 = data[i + 61];
        const int * base62 = data[i + 62];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          base44 = (base44[half] < targets[i + 44]) ? &base44[half] : base44;
          base45 = (base45[half] < targets[i + 45]) ? &base45[half] : base45;
          base46 = (base46[half] < targets[i + 46]) ? &base46[half] : base46;
          base47 = (base47[half] < targets[i + 47]) ? &base47[half] : base47;
          base48 = (base48[half] < targets[i + 48]) ? &base48[half] : base48;
          base49 = (base49[half] < targets[i + 49]) ? &base49[half] : base49;
          base50 = (base50[half] < targets[i + 50]) ? &base50[half] : base50;
          base51 = (base51[half] < targets[i + 51]) ? &base51[half] : base51;
          base52 = (base52[half] < targets[i + 52]) ? &base52[half] : base52;
          base53 = (base53[half] < targets[i + 53]) ? &base53[half] : base53;
          base54 = (base54[half] < targets[i + 54]) ? &base54[half] : base54;
          base55 = (base55[half] < targets[i + 55]) ? &base55[half] : base55;
          base56 = (base56[half] < targets[i + 56]) ? &base56[half] : base56;
          base57 = (base57[half] < targets[i + 57]) ? &base57[half] : base57;
          base58 = (base58[half] < targets[i + 58]) ? &base58[half] : base58;
          base59 = (base59[half] < targets[i + 59]) ? &base59[half] : base59;
          base60 = (base60[half] < targets[i + 60]) ? &base60[half] : base60;
          base61 = (base61[half] < targets[i + 61]) ? &base61[half] : base61;
          base62 = (base62[half] < targets[i + 62]) ? &base62[half] : base62;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
        solutions[i + 44] = (base44[0] < targets[i + 44]) + base44 - data[i + 44];
        solutions[i + 45] = (base45[0] < targets[i + 45]) + base45 - data[i + 45];
        solutions[i + 46] = (base46[0] < targets[i + 46]) + base46 - data[i + 46];
        solutions[i + 47] = (base47[0] < targets[i + 47]) + base47 - data[i + 47];
        solutions[i + 48] = (base48[0] < targets[i + 48]) + base48 - data[i + 48];
        solutions[i + 49] = (base49[0] < targets[i + 49]) + base49 - data[i + 49];
        solutions[i + 50] = (base50[0] < targets[i + 50]) + base50 - data[i + 50];
        solutions[i + 51] = (base51[0] < targets[i + 51]) + base51 - data[i + 51];
        solutions[i + 52] = (base52[0] < targets[i + 52]) + base52 - data[i + 52];
        solutions[i + 53] = (base53[0] < targets[i + 53]) + base53 - data[i + 53];
        solutions[i + 54] = (base54[0] < targets[i + 54]) + base54 - data[i + 54];
        solutions[i + 55] = (base55[0] < targets[i + 55]) + base55 - data[i + 55];
        solutions[i + 56] = (base56[0] < targets[i + 56]) + base56 - data[i + 56];
        solutions[i + 57] = (base57[0] < targets[i + 57]) + base57 - data[i + 57];
        solutions[i + 58] = (base58[0] < targets[i + 58]) + base58 - data[i + 58];
        solutions[i + 59] = (base59[0] < targets[i + 59]) + base59 - data[i + 59];
        solutions[i + 60] = (base60[0] < targets[i + 60]) + base60 - data[i + 60];
        solutions[i + 61] = (base61[0] < targets[i + 61]) + base61 - data[i + 61];
        solutions[i + 62] = (base62[0] < targets[i + 62]) + base62 - data[i + 62];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}
void binsearch64(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= 64) {
      for(; i <= howmany - 64; i += 64) {
        const int * base0 = data[i + 0];
        const int * base1 = data[i + 1];
        const int * base2 = data[i + 2];
        const int * base3 = data[i + 3];
        const int * base4 = data[i + 4];
        const int * base5 = data[i + 5];
        const int * base6 = data[i + 6];
        const int * base7 = data[i + 7];
        const int * base8 = data[i + 8];
        const int * base9 = data[i + 9];
        const int * base10 = data[i + 10];
        const int * base11 = data[i + 11];
        const int * base12 = data[i + 12];
        const int * base13 = data[i + 13];
        const int * base14 = data[i + 14];
        const int * base15 = data[i + 15];
        const int * base16 = data[i + 16];
        const int * base17 = data[i + 17];
        const int * base18 = data[i + 18];
        const int * base19 = data[i + 19];
        const int * base20 = data[i + 20];
        const int * base21 = data[i + 21];
        const int * base22 = data[i + 22];
        const int * base23 = data[i + 23];
        const int * base24 = data[i + 24];
        const int * base25 = data[i + 25];
        const int * base26 = data[i + 26];
        const int * base27 = data[i + 27];
        const int * base28 = data[i + 28];
        const int * base29 = data[i + 29];
        const int * base30 = data[i + 30];
        const int * base31 = data[i + 31];
        const int * base32 = data[i + 32];
        const int * base33 = data[i + 33];
        const int * base34 = data[i + 34];
        const int * base35 = data[i + 35];
        const int * base36 = data[i + 36];
        const int * base37 = data[i + 37];
        const int * base38 = data[i + 38];
        const int * base39 = data[i + 39];
        const int * base40 = data[i + 40];
        const int * base41 = data[i + 41];
        const int * base42 = data[i + 42];
        const int * base43 = data[i + 43];
        const int * base44 = data[i + 44];
        const int * base45 = data[i + 45];
        const int * base46 = data[i + 46];
        const int * base47 = data[i + 47];
        const int * base48 = data[i + 48];
        const int * base49 = data[i + 49];
        const int * base50 = data[i + 50];
        const int * base51 = data[i + 51];
        const int * base52 = data[i + 52];
        const int * base53 = data[i + 53];
        const int * base54 = data[i + 54];
        const int * base55 = data[i + 55];
        const int * base56 = data[i + 56];
        const int * base57 = data[i + 57];
        const int * base58 = data[i + 58];
        const int * base59 = data[i + 59];
        const int * base60 = data[i + 60];
        const int * base61 = data[i + 61];
        const int * base62 = data[i + 62];
        const int * base63 = data[i + 63];
        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
          base0 = (base0[half] < targets[i + 0]) ? &base0[half] : base0;
          base1 = (base1[half] < targets[i + 1]) ? &base1[half] : base1;
          base2 = (base2[half] < targets[i + 2]) ? &base2[half] : base2;
          base3 = (base3[half] < targets[i + 3]) ? &base3[half] : base3;
          base4 = (base4[half] < targets[i + 4]) ? &base4[half] : base4;
          base5 = (base5[half] < targets[i + 5]) ? &base5[half] : base5;
          base6 = (base6[half] < targets[i + 6]) ? &base6[half] : base6;
          base7 = (base7[half] < targets[i + 7]) ? &base7[half] : base7;
          base8 = (base8[half] < targets[i + 8]) ? &base8[half] : base8;
          base9 = (base9[half] < targets[i + 9]) ? &base9[half] : base9;
          base10 = (base10[half] < targets[i + 10]) ? &base10[half] : base10;
          base11 = (base11[half] < targets[i + 11]) ? &base11[half] : base11;
          base12 = (base12[half] < targets[i + 12]) ? &base12[half] : base12;
          base13 = (base13[half] < targets[i + 13]) ? &base13[half] : base13;
          base14 = (base14[half] < targets[i + 14]) ? &base14[half] : base14;
          base15 = (base15[half] < targets[i + 15]) ? &base15[half] : base15;
          base16 = (base16[half] < targets[i + 16]) ? &base16[half] : base16;
          base17 = (base17[half] < targets[i + 17]) ? &base17[half] : base17;
          base18 = (base18[half] < targets[i + 18]) ? &base18[half] : base18;
          base19 = (base19[half] < targets[i + 19]) ? &base19[half] : base19;
          base20 = (base20[half] < targets[i + 20]) ? &base20[half] : base20;
          base21 = (base21[half] < targets[i + 21]) ? &base21[half] : base21;
          base22 = (base22[half] < targets[i + 22]) ? &base22[half] : base22;
          base23 = (base23[half] < targets[i + 23]) ? &base23[half] : base23;
          base24 = (base24[half] < targets[i + 24]) ? &base24[half] : base24;
          base25 = (base25[half] < targets[i + 25]) ? &base25[half] : base25;
          base26 = (base26[half] < targets[i + 26]) ? &base26[half] : base26;
          base27 = (base27[half] < targets[i + 27]) ? &base27[half] : base27;
          base28 = (base28[half] < targets[i + 28]) ? &base28[half] : base28;
          base29 = (base29[half] < targets[i + 29]) ? &base29[half] : base29;
          base30 = (base30[half] < targets[i + 30]) ? &base30[half] : base30;
          base31 = (base31[half] < targets[i + 31]) ? &base31[half] : base31;
          base32 = (base32[half] < targets[i + 32]) ? &base32[half] : base32;
          base33 = (base33[half] < targets[i + 33]) ? &base33[half] : base33;
          base34 = (base34[half] < targets[i + 34]) ? &base34[half] : base34;
          base35 = (base35[half] < targets[i + 35]) ? &base35[half] : base35;
          base36 = (base36[half] < targets[i + 36]) ? &base36[half] : base36;
          base37 = (base37[half] < targets[i + 37]) ? &base37[half] : base37;
          base38 = (base38[half] < targets[i + 38]) ? &base38[half] : base38;
          base39 = (base39[half] < targets[i + 39]) ? &base39[half] : base39;
          base40 = (base40[half] < targets[i + 40]) ? &base40[half] : base40;
          base41 = (base41[half] < targets[i + 41]) ? &base41[half] : base41;
          base42 = (base42[half] < targets[i + 42]) ? &base42[half] : base42;
          base43 = (base43[half] < targets[i + 43]) ? &base43[half] : base43;
          base44 = (base44[half] < targets[i + 44]) ? &base44[half] : base44;
          base45 = (base45[half] < targets[i + 45]) ? &base45[half] : base45;
          base46 = (base46[half] < targets[i + 46]) ? &base46[half] : base46;
          base47 = (base47[half] < targets[i + 47]) ? &base47[half] : base47;
          base48 = (base48[half] < targets[i + 48]) ? &base48[half] : base48;
          base49 = (base49[half] < targets[i + 49]) ? &base49[half] : base49;
          base50 = (base50[half] < targets[i + 50]) ? &base50[half] : base50;
          base51 = (base51[half] < targets[i + 51]) ? &base51[half] : base51;
          base52 = (base52[half] < targets[i + 52]) ? &base52[half] : base52;
          base53 = (base53[half] < targets[i + 53]) ? &base53[half] : base53;
          base54 = (base54[half] < targets[i + 54]) ? &base54[half] : base54;
          base55 = (base55[half] < targets[i + 55]) ? &base55[half] : base55;
          base56 = (base56[half] < targets[i + 56]) ? &base56[half] : base56;
          base57 = (base57[half] < targets[i + 57]) ? &base57[half] : base57;
          base58 = (base58[half] < targets[i + 58]) ? &base58[half] : base58;
          base59 = (base59[half] < targets[i + 59]) ? &base59[half] : base59;
          base60 = (base60[half] < targets[i + 60]) ? &base60[half] : base60;
          base61 = (base61[half] < targets[i + 61]) ? &base61[half] : base61;
          base62 = (base62[half] < targets[i + 62]) ? &base62[half] : base62;
          base63 = (base63[half] < targets[i + 63]) ? &base63[half] : base63;
          n -= half;
        }
        solutions[i + 0] = (base0[0] < targets[i + 0]) + base0 - data[i + 0];
        solutions[i + 1] = (base1[0] < targets[i + 1]) + base1 - data[i + 1];
        solutions[i + 2] = (base2[0] < targets[i + 2]) + base2 - data[i + 2];
        solutions[i + 3] = (base3[0] < targets[i + 3]) + base3 - data[i + 3];
        solutions[i + 4] = (base4[0] < targets[i + 4]) + base4 - data[i + 4];
        solutions[i + 5] = (base5[0] < targets[i + 5]) + base5 - data[i + 5];
        solutions[i + 6] = (base6[0] < targets[i + 6]) + base6 - data[i + 6];
        solutions[i + 7] = (base7[0] < targets[i + 7]) + base7 - data[i + 7];
        solutions[i + 8] = (base8[0] < targets[i + 8]) + base8 - data[i + 8];
        solutions[i + 9] = (base9[0] < targets[i + 9]) + base9 - data[i + 9];
        solutions[i + 10] = (base10[0] < targets[i + 10]) + base10 - data[i + 10];
        solutions[i + 11] = (base11[0] < targets[i + 11]) + base11 - data[i + 11];
        solutions[i + 12] = (base12[0] < targets[i + 12]) + base12 - data[i + 12];
        solutions[i + 13] = (base13[0] < targets[i + 13]) + base13 - data[i + 13];
        solutions[i + 14] = (base14[0] < targets[i + 14]) + base14 - data[i + 14];
        solutions[i + 15] = (base15[0] < targets[i + 15]) + base15 - data[i + 15];
        solutions[i + 16] = (base16[0] < targets[i + 16]) + base16 - data[i + 16];
        solutions[i + 17] = (base17[0] < targets[i + 17]) + base17 - data[i + 17];
        solutions[i + 18] = (base18[0] < targets[i + 18]) + base18 - data[i + 18];
        solutions[i + 19] = (base19[0] < targets[i + 19]) + base19 - data[i + 19];
        solutions[i + 20] = (base20[0] < targets[i + 20]) + base20 - data[i + 20];
        solutions[i + 21] = (base21[0] < targets[i + 21]) + base21 - data[i + 21];
        solutions[i + 22] = (base22[0] < targets[i + 22]) + base22 - data[i + 22];
        solutions[i + 23] = (base23[0] < targets[i + 23]) + base23 - data[i + 23];
        solutions[i + 24] = (base24[0] < targets[i + 24]) + base24 - data[i + 24];
        solutions[i + 25] = (base25[0] < targets[i + 25]) + base25 - data[i + 25];
        solutions[i + 26] = (base26[0] < targets[i + 26]) + base26 - data[i + 26];
        solutions[i + 27] = (base27[0] < targets[i + 27]) + base27 - data[i + 27];
        solutions[i + 28] = (base28[0] < targets[i + 28]) + base28 - data[i + 28];
        solutions[i + 29] = (base29[0] < targets[i + 29]) + base29 - data[i + 29];
        solutions[i + 30] = (base30[0] < targets[i + 30]) + base30 - data[i + 30];
        solutions[i + 31] = (base31[0] < targets[i + 31]) + base31 - data[i + 31];
        solutions[i + 32] = (base32[0] < targets[i + 32]) + base32 - data[i + 32];
        solutions[i + 33] = (base33[0] < targets[i + 33]) + base33 - data[i + 33];
        solutions[i + 34] = (base34[0] < targets[i + 34]) + base34 - data[i + 34];
        solutions[i + 35] = (base35[0] < targets[i + 35]) + base35 - data[i + 35];
        solutions[i + 36] = (base36[0] < targets[i + 36]) + base36 - data[i + 36];
        solutions[i + 37] = (base37[0] < targets[i + 37]) + base37 - data[i + 37];
        solutions[i + 38] = (base38[0] < targets[i + 38]) + base38 - data[i + 38];
        solutions[i + 39] = (base39[0] < targets[i + 39]) + base39 - data[i + 39];
        solutions[i + 40] = (base40[0] < targets[i + 40]) + base40 - data[i + 40];
        solutions[i + 41] = (base41[0] < targets[i + 41]) + base41 - data[i + 41];
        solutions[i + 42] = (base42[0] < targets[i + 42]) + base42 - data[i + 42];
        solutions[i + 43] = (base43[0] < targets[i + 43]) + base43 - data[i + 43];
        solutions[i + 44] = (base44[0] < targets[i + 44]) + base44 - data[i + 44];
        solutions[i + 45] = (base45[0] < targets[i + 45]) + base45 - data[i + 45];
        solutions[i + 46] = (base46[0] < targets[i + 46]) + base46 - data[i + 46];
        solutions[i + 47] = (base47[0] < targets[i + 47]) + base47 - data[i + 47];
        solutions[i + 48] = (base48[0] < targets[i + 48]) + base48 - data[i + 48];
        solutions[i + 49] = (base49[0] < targets[i + 49]) + base49 - data[i + 49];
        solutions[i + 50] = (base50[0] < targets[i + 50]) + base50 - data[i + 50];
        solutions[i + 51] = (base51[0] < targets[i + 51]) + base51 - data[i + 51];
        solutions[i + 52] = (base52[0] < targets[i + 52]) + base52 - data[i + 52];
        solutions[i + 53] = (base53[0] < targets[i + 53]) + base53 - data[i + 53];
        solutions[i + 54] = (base54[0] < targets[i + 54]) + base54 - data[i + 54];
        solutions[i + 55] = (base55[0] < targets[i + 55]) + base55 - data[i + 55];
        solutions[i + 56] = (base56[0] < targets[i + 56]) + base56 - data[i + 56];
        solutions[i + 57] = (base57[0] < targets[i + 57]) + base57 - data[i + 57];
        solutions[i + 58] = (base58[0] < targets[i + 58]) + base58 - data[i + 58];
        solutions[i + 59] = (base59[0] < targets[i + 59]) + base59 - data[i + 59];
        solutions[i + 60] = (base60[0] < targets[i + 60]) + base60 - data[i + 60];
        solutions[i + 61] = (base61[0] < targets[i + 61]) + base61 - data[i + 61];
        solutions[i + 62] = (base62[0] < targets[i + 62]) + base62 - data[i + 62];
        solutions[i + 63] = (base63[0] < targets[i + 63]) + base63 - data[i + 63];
      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}

int main() {
  size_t small = 1024;
  size_t large = 64 * 1000 * 1000 / small;

  printf("Initializing random data.\n");
  std::vector<std::vector<int>> datavec;
  std::vector<int> targets;
  for (size_t i = 0; i < small; i++) {
    datavec.emplace_back(std::vector<int>(large));
    for (size_t z = 0; z < large; z++) {
      datavec[i][z] = splitmix64_stateless(i * large + z);
    }
    targets.push_back(datavec[i][0]);
    std::sort(datavec[i].begin(), datavec[i].end());
    if((i % (small / 10)) == 0) printf(".");
    fflush(NULL);
  }
  printf("\n");
  std::vector<size_t> solution(small);
  int ** data = new int*[small];
  for (size_t i = 0; i < small; i++) {
      data[i] = datavec[i].data();
  }
  int counter = 0;


  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start2 = std::chrono::high_resolution_clock::now();
  binsearch2(data, small, large, targets.data(), solution.data());
  auto finish2 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 2 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish2-start2).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start3 = std::chrono::high_resolution_clock::now();
  binsearch3(data, small, large, targets.data(), solution.data());
  auto finish3 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 3 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish3-start3).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start4 = std::chrono::high_resolution_clock::now();
  binsearch4(data, small, large, targets.data(), solution.data());
  auto finish4 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 4 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish4-start4).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start5 = std::chrono::high_resolution_clock::now();
  binsearch5(data, small, large, targets.data(), solution.data());
  auto finish5 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 5 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish5-start5).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start6 = std::chrono::high_resolution_clock::now();
  binsearch6(data, small, large, targets.data(), solution.data());
  auto finish6 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 6 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish6-start6).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start7 = std::chrono::high_resolution_clock::now();
  binsearch7(data, small, large, targets.data(), solution.data());
  auto finish7 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 7 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish7-start7).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start8 = std::chrono::high_resolution_clock::now();
  binsearch8(data, small, large, targets.data(), solution.data());
  auto finish8 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 8 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish8-start8).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start9 = std::chrono::high_resolution_clock::now();
  binsearch9(data, small, large, targets.data(), solution.data());
  auto finish9 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 9 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish9-start9).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start10 = std::chrono::high_resolution_clock::now();
  binsearch10(data, small, large, targets.data(), solution.data());
  auto finish10 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 10 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish10-start10).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start11 = std::chrono::high_resolution_clock::now();
  binsearch11(data, small, large, targets.data(), solution.data());
  auto finish11 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 11 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish11-start11).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start12 = std::chrono::high_resolution_clock::now();
  binsearch12(data, small, large, targets.data(), solution.data());
  auto finish12 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 12 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish12-start12).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start13 = std::chrono::high_resolution_clock::now();
  binsearch13(data, small, large, targets.data(), solution.data());
  auto finish13 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 13 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish13-start13).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start14 = std::chrono::high_resolution_clock::now();
  binsearch14(data, small, large, targets.data(), solution.data());
  auto finish14 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 14 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish14-start14).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start15 = std::chrono::high_resolution_clock::now();
  binsearch15(data, small, large, targets.data(), solution.data());
  auto finish15 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 15 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish15-start15).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start16 = std::chrono::high_resolution_clock::now();
  binsearch16(data, small, large, targets.data(), solution.data());
  auto finish16 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 16 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish16-start16).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start17 = std::chrono::high_resolution_clock::now();
  binsearch17(data, small, large, targets.data(), solution.data());
  auto finish17 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 17 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish17-start17).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start18 = std::chrono::high_resolution_clock::now();
  binsearch18(data, small, large, targets.data(), solution.data());
  auto finish18 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 18 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish18-start18).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start19 = std::chrono::high_resolution_clock::now();
  binsearch19(data, small, large, targets.data(), solution.data());
  auto finish19 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 19 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish19-start19).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start20 = std::chrono::high_resolution_clock::now();
  binsearch20(data, small, large, targets.data(), solution.data());
  auto finish20 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 20 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish20-start20).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start21 = std::chrono::high_resolution_clock::now();
  binsearch21(data, small, large, targets.data(), solution.data());
  auto finish21 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 21 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish21-start21).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start22 = std::chrono::high_resolution_clock::now();
  binsearch22(data, small, large, targets.data(), solution.data());
  auto finish22 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 22 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish22-start22).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start23 = std::chrono::high_resolution_clock::now();
  binsearch23(data, small, large, targets.data(), solution.data());
  auto finish23 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 23 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish23-start23).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start24 = std::chrono::high_resolution_clock::now();
  binsearch24(data, small, large, targets.data(), solution.data());
  auto finish24 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 24 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish24-start24).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start25 = std::chrono::high_resolution_clock::now();
  binsearch25(data, small, large, targets.data(), solution.data());
  auto finish25 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 25 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish25-start25).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start26 = std::chrono::high_resolution_clock::now();
  binsearch26(data, small, large, targets.data(), solution.data());
  auto finish26 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 26 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish26-start26).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start27 = std::chrono::high_resolution_clock::now();
  binsearch27(data, small, large, targets.data(), solution.data());
  auto finish27 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 27 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish27-start27).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start28 = std::chrono::high_resolution_clock::now();
  binsearch28(data, small, large, targets.data(), solution.data());
  auto finish28 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 28 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish28-start28).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start29 = std::chrono::high_resolution_clock::now();
  binsearch29(data, small, large, targets.data(), solution.data());
  auto finish29 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 29 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish29-start29).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start30 = std::chrono::high_resolution_clock::now();
  binsearch30(data, small, large, targets.data(), solution.data());
  auto finish30 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 30 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish30-start30).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start31 = std::chrono::high_resolution_clock::now();
  binsearch31(data, small, large, targets.data(), solution.data());
  auto finish31 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 31 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish31-start31).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start32 = std::chrono::high_resolution_clock::now();
  binsearch32(data, small, large, targets.data(), solution.data());
  auto finish32 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 32 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish32-start32).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start33 = std::chrono::high_resolution_clock::now();
  binsearch33(data, small, large, targets.data(), solution.data());
  auto finish33 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 33 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish33-start33).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start34 = std::chrono::high_resolution_clock::now();
  binsearch34(data, small, large, targets.data(), solution.data());
  auto finish34 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 34 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish34-start34).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start35 = std::chrono::high_resolution_clock::now();
  binsearch35(data, small, large, targets.data(), solution.data());
  auto finish35 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 35 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish35-start35).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start36 = std::chrono::high_resolution_clock::now();
  binsearch36(data, small, large, targets.data(), solution.data());
  auto finish36 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 36 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish36-start36).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start37 = std::chrono::high_resolution_clock::now();
  binsearch37(data, small, large, targets.data(), solution.data());
  auto finish37 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 37 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish37-start37).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start38 = std::chrono::high_resolution_clock::now();
  binsearch38(data, small, large, targets.data(), solution.data());
  auto finish38 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 38 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish38-start38).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start39 = std::chrono::high_resolution_clock::now();
  binsearch39(data, small, large, targets.data(), solution.data());
  auto finish39 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 39 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish39-start39).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start40 = std::chrono::high_resolution_clock::now();
  binsearch40(data, small, large, targets.data(), solution.data());
  auto finish40 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 40 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish40-start40).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start41 = std::chrono::high_resolution_clock::now();
  binsearch41(data, small, large, targets.data(), solution.data());
  auto finish41 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 41 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish41-start41).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start42 = std::chrono::high_resolution_clock::now();
  binsearch42(data, small, large, targets.data(), solution.data());
  auto finish42 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 42 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish42-start42).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start43 = std::chrono::high_resolution_clock::now();
  binsearch43(data, small, large, targets.data(), solution.data());
  auto finish43 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 43 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish43-start43).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start44 = std::chrono::high_resolution_clock::now();
  binsearch44(data, small, large, targets.data(), solution.data());
  auto finish44 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 44 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish44-start44).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start45 = std::chrono::high_resolution_clock::now();
  binsearch45(data, small, large, targets.data(), solution.data());
  auto finish45 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 45 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish45-start45).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start46 = std::chrono::high_resolution_clock::now();
  binsearch46(data, small, large, targets.data(), solution.data());
  auto finish46 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 46 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish46-start46).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start47 = std::chrono::high_resolution_clock::now();
  binsearch47(data, small, large, targets.data(), solution.data());
  auto finish47 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 47 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish47-start47).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start48 = std::chrono::high_resolution_clock::now();
  binsearch48(data, small, large, targets.data(), solution.data());
  auto finish48 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 48 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish48-start48).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start49 = std::chrono::high_resolution_clock::now();
  binsearch49(data, small, large, targets.data(), solution.data());
  auto finish49 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 49 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish49-start49).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start50 = std::chrono::high_resolution_clock::now();
  binsearch50(data, small, large, targets.data(), solution.data());
  auto finish50 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 50 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish50-start50).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start51 = std::chrono::high_resolution_clock::now();
  binsearch51(data, small, large, targets.data(), solution.data());
  auto finish51 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 51 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish51-start51).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start52 = std::chrono::high_resolution_clock::now();
  binsearch52(data, small, large, targets.data(), solution.data());
  auto finish52 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 52 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish52-start52).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start53 = std::chrono::high_resolution_clock::now();
  binsearch53(data, small, large, targets.data(), solution.data());
  auto finish53 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 53 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish53-start53).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start54 = std::chrono::high_resolution_clock::now();
  binsearch54(data, small, large, targets.data(), solution.data());
  auto finish54 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 54 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish54-start54).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start55 = std::chrono::high_resolution_clock::now();
  binsearch55(data, small, large, targets.data(), solution.data());
  auto finish55 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 55 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish55-start55).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start56 = std::chrono::high_resolution_clock::now();
  binsearch56(data, small, large, targets.data(), solution.data());
  auto finish56 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 56 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish56-start56).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start57 = std::chrono::high_resolution_clock::now();
  binsearch57(data, small, large, targets.data(), solution.data());
  auto finish57 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 57 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish57-start57).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start58 = std::chrono::high_resolution_clock::now();
  binsearch58(data, small, large, targets.data(), solution.data());
  auto finish58 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 58 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish58-start58).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start59 = std::chrono::high_resolution_clock::now();
  binsearch59(data, small, large, targets.data(), solution.data());
  auto finish59 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 59 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish59-start59).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start60 = std::chrono::high_resolution_clock::now();
  binsearch60(data, small, large, targets.data(), solution.data());
  auto finish60 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 60 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish60-start60).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start61 = std::chrono::high_resolution_clock::now();
  binsearch61(data, small, large, targets.data(), solution.data());
  auto finish61 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 61 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish61-start61).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start62 = std::chrono::high_resolution_clock::now();
  binsearch62(data, small, large, targets.data(), solution.data());
  auto finish62 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 62 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish62-start62).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start63 = std::chrono::high_resolution_clock::now();
  binsearch63(data, small, large, targets.data(), solution.data());
  auto finish63 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 63 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish63-start63).count() << std::endl;
  for(int z = 0; z < 5; z++) element_access(datavec, & counter);
  auto start64 = std::chrono::high_resolution_clock::now();
  binsearch64(data, small, large, targets.data(), solution.data());
  auto finish64 = std::chrono::high_resolution_clock::now();
  std::cout <<  " 64 : " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish64-start64).count() << std::endl;

  delete[] data;
  if(counter == 0) printf("counter is zero\n");
  return EXIT_SUCCESS;
}
