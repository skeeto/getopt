/* A minimal POSIX getopt() implementation in ANSI C
 *
 * This is free and unencumbered software released into the public domain.
 *
 * This implementation supports the convention of resetting the option
 * parser by assigning optind to 0. This resets the internal state
 * appropriately.
 *
 * Ref: http://pubs.opengroup.org/onlinepubs/9699919799/functions/getopt.html
 */
#ifndef GETOPT_H
#define GETOPT_H

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static int optind = 0;
static int opterr = 1;
static int optopt;
static char *optarg;

/*
 * Returns 1 if the string at argv[index] follows a valid option flag that
 * requires an argument, 0 otherwise
 */
static
int is_optarg(char *argv[], int index, const char *optstring)
{
    if (index == 1) {
        return 0;
    }

    if (argv[index - 1][0] != '-') {
        return 0;
    }

    char opt_test = argv[index - 1][1];
    if (opt_test == '\0') {
        return 0;
    }

    char *opt = strchr(optstring, opt_test);
    if (opt == NULL) {
        return 0;
    }

    return opt[1] == ':';
}

/*
 * Find any arguments that do not follow an option flag requiring an argument,
 * and shift them to the end of the argument list
 */
static
void shift_nonopt_args(int argc, char *argv[], const char *optstring)
{
    for (int i = 1; i < (argc - 1); i++) {
        if (argv[i][0] != '-') {
            if (is_optarg(argv, i, optstring)) {
                continue;
            }

            char *temp = argv[i];

            for (int j = i + 1; j < argc; j++) {
                argv[j - 1] = argv[j];
            }

            argv[argc - 1] = temp;
        }
    }
}

static int
getopt(int argc, char * argv[], const char *optstring)
{
    static int optpos = 1;
    char *arg;
    (void)argc;

    /* Reset? */
    if (optind == 0) {
        optind = 1;
        optpos = 1;

        /* Move all non-option arguments to the end */
        shift_nonopt_args(argc, argv, optstring);
    }

    arg = argv[optind];
    if (arg && strcmp(arg, "--") == 0) {
        optind++;
        return -1;
    } else if (!arg || arg[0] != '-' || !isalnum(arg[1])) {
        return -1;
    } else {
        char *opt = strchr(optstring, arg[optpos]);
        optopt = arg[optpos];
        if (!opt) {
            if (opterr && *optstring != ':')
                fprintf(stderr, "%s: illegal option: %c\n", argv[0], optopt);
            return '?';
        } else if (opt[1] == ':') {
            if (arg[optpos + 1]) {
                optarg = (char *)arg + optpos + 1;
                optind++;
                optpos = 1;
                return optopt;
            } else if (argv[optind + 1]) {
                optarg = (char *)argv[optind + 1];
                optind += 2;
                optpos = 1;
                return optopt;
            } else {
                if (opterr && *optstring != ':')
                    fprintf(stderr, 
                            "%s: option requires an argument: %c\n", 
                            argv[0], optopt);
                return *optstring == ':' ? ':' : '?';
            }
        } else {
            if (!arg[++optpos]) {
                optind++;
                optpos = 1;
            }
            return optopt;
        }
    }
}

#endif
