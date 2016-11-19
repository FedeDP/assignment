#include "../inc/assignment.h"
#include <assert.h>

static void test_not_existent_options_file(void);
static void test_empty_options(void);
static void test_wrong_options(void);
static void test_options_file(void);
static void test_not_existent_input_file(void);
static void test_empty_input(void);
static void test_input_file(void);
static void test_impedance(void);

static struct options conf;
static input_l *h;
static int size;

int main(void) {
    /**
     * Test options file
     */
    test_not_existent_options_file();
    test_empty_options();
    test_wrong_options();
    test_options_file();
    
    /**
     * Test input file
     */
    test_not_existent_input_file();
    test_empty_input();
    test_input_file();
    
    /**
     * Test expected impedance
     */
    test_impedance();
    
    /**
     * Finally free input
     */
    free_input(h);
    
    return EXIT_SUCCESS;
}

/**
 * Test that a not-existent options file returns -1
 */
static void test_not_existent_options_file(void) {
    printf("***Testing not existent options file.***\n");
    strcpy(conf.options_file, "?"); 
    
    assert(read_conf(&conf) == -1);
}

/**
 * Test existent empty options file
 */
static void test_empty_options(void) {
    printf("***Testing empty options file.***\n");
    strcpy(conf.options_file, "test/test_options_blank.conf");
    
    assert(read_conf(&conf) == -1);
}

/**
 * Test existent wrong options file
 */
static void test_wrong_options(void) {
    printf("***Testing wrong options file.***\n");
    strcpy(conf.options_file, "test/test_options_wrong.txt"); 
    
    assert(read_conf(&conf) == -1);
    assert(conf.lang == -1);
    assert(h == NULL);
}

/**
 * Test existent options file
 */
static void test_options_file(void) {
    printf("***Testing existent options file.***\n");
    strcpy(conf.options_file, "test/test_options.conf");
        
    assert(read_conf(&conf) == 0);
    assert(conf.lang == 0);
    assert(conf.format == 0);
}

/**
 * Test not existent input file
 */
static void test_not_existent_input_file(void) {
    printf("***Testing not existent input file.***\n");
    strcpy(conf.input_file, "?"); 
    
    assert(read_input_file(conf, &h, &size) == -1);
    assert(h == NULL);
}

/**
 * Test existent empty input file
 */
static void test_empty_input(void) {
    printf("***Testing empty input file.***\n");
    strcpy(conf.input_file, "test/test_input_blank.conf"); 
    
    assert(read_input_file(conf, &h, &size) == -1);
    assert(h == NULL);
}

/**
 * Test existent input file
 */
static void test_input_file(void) {
    printf("***Testing existent input file.***\n");
    strcpy(conf.input_file, "test/test_input.txt"); 
    
    assert(read_input_file(conf, &h, &size) == 0);
    assert(h != NULL);
    assert(size == 1);
}

/**
 * Test expected impedance. test_input has only one line: {4.00, 3.00}, so we expect 5.00
 */
static void test_impedance(void) {
    printf("***Testing expected impedance value of 5.00.***\n");
    
    assert(compute_impedance(h, size) == 5.00);
}

