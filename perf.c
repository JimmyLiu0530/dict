#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "perf.h"

#define DICT_FILE "cities.txt"
#define WORDMAX 256


int no_bloom_filter(const tst_node *root, char *out_file)
{
    char word[WORDMAX] = "";
    FILE *fp = fopen(out_file, "w");
    FILE *dict = fopen(DICT_FILE, "r");
    int idx = 0;
    double t1, t2;
    tst_node *res = NULL;

    if (!fp || !dict) {
        if (fp) {
            fprintf(stderr, "error: file open failed in '%s'.\n", DICT_FILE);
            fclose(fp);
        }
        if (dict) {
            fprintf(stderr, "error: file open failed in '%s'.\n", out_file);
            fclose(dict);
        }
        return 1;
    }

    while (fscanf(dict, "%s", word) != EOF) {
        t1 = tvgetf();
        res = tst_search(root, word);
        t2 = tvgetf();
        fprintf(fp, "%d %f msec\n", idx, (t2 - t1) * 1000);
        idx++;
    }

    fclose(fp);
    fclose(dict);
    return 0;
}

int bloom_filter(const tst_node *root, char *out_file, bloom_t bloom)
{
    char word[WORDMAX] = "";
    FILE *fp = fopen(out_file, "w");
    FILE *dict = fopen(DICT_FILE, "r");
    int idx = 0;
    double t1, t2;
    tst_node *res = NULL;

    if (!fp || !dict) {
        if (fp) {
            fprintf(stderr, "error: file open failed in '%s'.\n", DICT_FILE);
            fclose(fp);
        }
        if (dict) {
            fprintf(stderr, "error: file open failed in '%s'.\n", out_file);
            fclose(dict);
        }
        return 1;
    }

    while (fscanf(dict, "%s", word) != EOF) {
        t1 = tvgetf();
        if (!bloom_test(bloom, word)) { /* check if word is in bloom filter
                                           in advance*/
            t2 = tvgetf();
            fprintf(fp, "%d %f msec\n", idx, (t2 - t1) * 1000);
            idx++;
            continue;
        }
        res = tst_search(root, word);
        t2 = tvgetf();
        fprintf(fp, "%d %f msec\n", idx, (t2 - t1) * 1000);
        idx++;
    }

    fclose(fp);
    fclose(dict);
    return 0;
}
