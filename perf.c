#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "perf.h"

#define DICT_FILE "cities.txt"
#define WORDMAX 256


int no_bloom_filter(const tst_node *root, char *out_file)
{
    FILE *fp = fopen(out_file, "w");
    FILE *dict = fopen(DICT_FILE, "r");
    int idx = 0;
    double t1, t2;

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

    char buf[WORDMAX];
    char str[WORDMAX];
    int stridx = 0;
    while (fgets(buf, WORDMAX, dict)) {
        for (int i = 0; i < strlen(buf); i++) {
            if (buf[i] == ',' || buf[i] == '\n') {
                str[stridx] = '\0';
                i++;
                stridx = 0;
                t1 = tvgetf();
                tst_search(root, str);
                t2 = tvgetf();
                fprintf(fp, "%d %f msec\n", idx, (t2 - t1) * 1000);
                idx++;
            } else
                str[stridx++] = buf[i];
        }
    }

    fclose(fp);
    fclose(dict);
    return 0;
}

int bloom_filter(const tst_node *root, char *out_file, bloom_t bloom)
{
    FILE *fp = fopen(out_file, "w");
    FILE *dict = fopen(DICT_FILE, "r");
    int idx = 0;
    double t1, t2;

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

    char buf[WORDMAX];
    char str[WORDMAX];
    int stridx = 0;
    while (fgets(buf, WORDMAX, dict)) {
        for (int i = 0; i < strlen(buf); i++) {
            if (buf[i] == ',' || buf[i] == '\n') {
                str[stridx] = '\0';
                i++;
                stridx = 0;
                t1 = tvgetf();
                if (!bloom_test(
                        bloom,
                        str)) { /* check if word is in bloom filter in advance*/
                    t2 = tvgetf();
                    fprintf(fp, "%d %f msec\n", idx, (t2 - t1) * 1000);
                    idx++;
                    continue;
                }
                tst_search(root, str);
                t2 = tvgetf();
                fprintf(fp, "%d %f msec\n", idx, (t2 - t1) * 1000);
                idx++;
            } else
                str[stridx++] = buf[i];
        }
    }

    fclose(fp);
    fclose(dict);
    return 0;
}
