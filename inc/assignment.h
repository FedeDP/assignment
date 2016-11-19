#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

/**
 * List that holds data read from input file.
 */
typedef struct list {
    double  resistance;
    double reactance;
    struct tm timedate;
    struct list *next;
} input_l;

/**
 * Struct that holds our options
 */
struct options {
    char input_file[PATH_MAX + 1];
    char options_file[PATH_MAX + 1];
    int lang;
    int format;
};

/**
 * Struct needed to map lang string as read from options.conf,
 * to its correct string.
 */
struct lang {
    char *language;
    char *string;
};

/**
 * Struct needed to map format string as read from options.conf,
 * to its correct format (to be passed to strftime)
 */
struct format {
    char *language;
    char *format;
    char *printable_format;
};

int read_conf(struct options *opt);
int read_input_file(struct options opt, input_l **h, int *ssize);
double compute_impedance(input_l *h, int ssize);
void free_input(input_l *h);
