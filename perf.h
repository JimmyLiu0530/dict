#ifndef PERF_H
#define PERF_H
#include "bench.h"
#include "bloom.h"
#include "tst.h"

int no_bloom_filter(const tst_node *root, char *out_file);
int bloom_filter(const tst_node *root, char *out_file, bloom_t filter);

#endif
