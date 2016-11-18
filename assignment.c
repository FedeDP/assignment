/**
 * Required for strptime
 */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include <limits.h>

/**
 * Useful macro to know number of languages/formats available
 */
#define SIZE(a) (sizeof(a) / sizeof(*a))

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

static void init_default_values(void);
static void check_cmd(int argc, char *argv[]);
static void show_helper(const char *str);
static int read_conf(void);
static int get_lang(const char *str);
static int get_format(const char *str);
static int read_input_file(void);
static int add_node(input_l **h, size_t ssize, double resistance, double reactance);
static double compute_impedance(void);
static void pretty_print(double impedance);
static void free_input(input_l *h);

static input_l *values;
struct options conf;
static int size;
static struct format formats[] = {
    {"it", "%d-%m-%Y", "DD-MM-YYYY"},
    {"us", "%m-%d-%Y", "MM-DD-YYYY"},
    {"as", "%Y%m%d", "YYYYMMDD"}
};

static struct lang languages[] = {
    {"it", "Impedenza media calcolata nel giorno"},
    {"en", "Mean impedence value during day"}
};

int main(int argc, char *argv[]) {
    int ret = EXIT_SUCCESS;
    
    init_default_values();
    check_cmd(argc, argv);
        
    if (!read_conf() && !read_input_file()) {
        double impedance = compute_impedance();
        pretty_print(impedance);
    } else {
        ret = EXIT_FAILURE;
    }
    
    if (values) {
        free_input(values);
    }
    return ret;
}

/**
 * Helper function to initialize default values.
 */
static void init_default_values(void) {
    strncpy(conf.options_file, "options.conf", PATH_MAX);
    strncpy(conf.input_file, "input.txt", PATH_MAX);
}

/**
 * Check cmdline args through GNU getopt.
 */
static void check_cmd(int argc, char *argv[]) {
    int idx = 0, opt;
    
    static struct option opts[] = {
        {"help", no_argument, 0, 'h'},
        {"options", required_argument, 0, 'o'},
        {"input", required_argument, 0, 'i'},
        {0, 0, 0, 0}
    };
    
    while ((opt = getopt_long(argc, argv, "hi:o:", opts, &idx)) != -1) {
        switch (opt) {
        case 'h':
            show_helper(argv[0]);
            exit(EXIT_SUCCESS);
        case 'o':
            strncpy(conf.options_file, optarg, PATH_MAX);
            break;
        case 'i':
            strncpy(conf.input_file, optarg, PATH_MAX);
            break;
        case '?':
            exit(EXIT_FAILURE);
        default:
            break;
        }
    }
}

/**
 * Helper function to print helper strings.
 */
static void show_helper(const char *str) {
    printf("Usage:\n");
    printf("\t%s --help/-h to show this message.\n", str);
    printf("\t%s --options/-o /path/to/option to change default option file. Defaults to ./options.conf\n", str);
    printf("\t%s --input/-i /path/to/input to change default input file. Defaults to ./input.txt\n\n", str);
}

/**
 * Reads options.conf and load in conf correct values.
 */
static int read_conf(void) {
    FILE *f = NULL;
    int ret = 0;
    char s[3];
    
   
    if (!(f = fopen(conf.options_file, "r"))) {
        ret = -1;
        perror("File");
        goto end;
    }
    
    if (fscanf(f, "language:=%s\n", s) != 1) {
        ret = -1;
        fprintf(stderr, "Failed to read language:= value from file %s. Please check it.\n", conf.options_file);
        goto end;
    }
    
    conf.lang = get_lang(s);
    if (conf.lang == -1) {
        ret = -1;
        fprintf(stderr, "Failed to find %s language.\n", s);
        goto end;
    }
    
    if (fscanf(f, "date_format:=%s", s) != 1) {
        ret = -1;
        fprintf(stderr, "Failed to read date_format:= value from file %s. Please check it.\n", conf.options_file);
        goto end;
    }
    
    conf.format = get_format(s);
    if (conf.format == -1) {
        ret = -1;
        fprintf(stderr, "Failed to find %s date_format.\n", s);
        goto end;
    }

end:
    if (f) {
        fclose(f);
    }
    return ret;
}

/**
 * Helper function to get correct indexes for our conf.lang.
 */
static int get_lang(const char *str) {
    for (int i = 0; i < SIZE(languages); i++) {
        if (strcmp(languages[i].language, str) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * Helper function to get correct indexes for our conf.format.
 */
static int get_format(const char *str) {
    for (int i = 0; i < SIZE(formats); i++) {
        if (strcmp(formats[i].language, str) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * Reads input file.
 * It stops when fscanf does not read resistance and reactance,
 * and checks if feof is true. Else an error happened.
 * 
 * Note that i'm using a double pointer to our list to
 * initialize every value (head too) of our list.
 */
static int read_input_file(void) {
    FILE *f = NULL;
    int ret = 0;
    char *date = NULL;
    size_t n = 0;
    double resistance, reactance;
        
    if (!(f = fopen(conf.input_file, "r"))) {
        ret = -1;
        perror("File");
        goto end;
    }
    
    input_l **tmp = &values;
    
    while (fscanf(f,"%lf %lf ", &resistance, &reactance) == 2) {
        if (add_node(tmp, sizeof(input_l), resistance, reactance) == -1) {
            ret = -1;
            break;
        }
                
        if (getline(&date, &n, f) == -1) {
            ret = -1;
            perror("Getline");
            break;
        }
        
        // cleanup memory
        memset(&((*tmp)->timedate), 0, sizeof(struct tm));
        strptime(date, "%d-%m-%Y %H:%M", &((*tmp)->timedate));
        
        tmp = &((*tmp)->next);
        size++;
    }
    
    if (!feof(f) || !values) {
        ret = -1;
        fprintf(stderr, "Failed to read proper values from file %s. Please check it.\n", conf.input_file);
    }
    
    free(date);
    fclose(f);
    
end:
    return ret;
}

/**
 * Given a double pointer, allocates needed memory for pointer pointed by it.
 */
static int add_node(input_l **h, size_t ssize, double resistance, double reactance) {
    *h = malloc(size);
    
    if (*h != NULL) {
        (*h)->next = NULL;
        (*h)->resistance = resistance;
        (*h)->reactance = reactance;
        return 0;
    }
    return -1;
}

/**
 * Simple function to compute required impedance.
 */
static double compute_impedance(void) {
    input_l *tmp;
    double impedance = 0;
    
    for (tmp = values; tmp; tmp = tmp->next) {
        impedance += sqrt(pow(tmp->resistance, 2) + pow(tmp->reactance, 2));
    }
    
    return impedance / size;
}

/**
 * Simple function to print our result with required format.
 */
static void pretty_print(double impedance) {
    char time_str[15];
    strftime(time_str, sizeof(time_str), formats[conf.format].format, &(values->timedate));
    
    printf("%s %s [%s]\n", languages[conf.lang].string, time_str, formats[conf.format].printable_format);
    printf("|Z| = %.2lf\n", impedance);
}

/**
 * Helper recursive free function.
 * Please, note that while this is surely the most elegant way,
 * it is quite heavy in case of lots of nodes. It can lead to stack overflows too.
 * An iterative free would be something like:
 * 
 * input_l *tmp;
 * while (h) {
 *    tmp = h
 *    h = tmp->next;
 *    free(tmp);
 * }
 */
static void free_input(input_l *h) {
    if (h->next) {
        free_input(h->next);
    }
    free(h);
}
