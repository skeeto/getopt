#include <stdio.h>
#include "getopt.h"

#define TEST(s, x) \
    do { \
        if (x) { \
            printf("\033[32;1mPASS\033[0m %s %s\n", argv[0], s); \
            count_pass++; \
        } else { \
            printf("\033[31;1mFAIL\033[0m %s %s\n", argv[0], s); \
            count_fail++; \
        } \
    } while (0)

#define ARGC(argv) (sizeof(argv) / sizeof(*argv) - 1)

static int count_pass;
static int count_fail;

static void
test0(void)
{
    char *argv[] = {
        "test0",
        "-a", "alpha",
        "-bbeta",
        "-c", "gamma",
        "--",
        "file",
        0
    };
    int argc = ARGC(argv);

    optind = 0;
    TEST("a", getopt(argc, argv, "a:b:c:") == 'a');
    TEST("a optarg", optarg == argv[2]);
    TEST("b", getopt(argc, argv, "a:b:c:") == 'b');
    TEST("b optarg", optarg == argv[3] + 2);
    TEST("c", getopt(argc, argv, "a:b:c:") == 'c');
    TEST("c optarg", optarg == argv[5]);
    TEST("finish", getopt(argc, argv, "a:b:c:") == -1);
    TEST("optind", argv[optind] == argv[argc - 1]);
}

static void
test1(void)
{
    char *argv[] = {
        "test1",
        "-abcfoo",
        0
    };
    int argc = ARGC(argv);

    optind = 0;
    TEST("a", getopt(argc, argv, "abc:") == 'a');
    TEST("b", getopt(argc, argv, "abc:") == 'b');
    TEST("c", getopt(argc, argv, "abc:") == 'c');
    TEST("a", getopt(argc, argv, "abc:") == -1);
    TEST("c optarg", optarg == argv[1] + 4);
    TEST("optind", !argv[optind]);
    TEST("stop 1", getopt(argc, argv, "abc:") == -1);
    TEST("c optarg", optind == 2);
    TEST("stop 2", getopt(argc, argv, "abc:") == -1);
    TEST("c optarg", optind == 2);
}

static void
test2(void)
{
    char *argv[] = {
        "test2",
        "-a",
        0
    };
    int argc = ARGC(argv);

    optind = 0;
    TEST("missing argument", getopt(argc, argv, ":a:bc") == ':');
    optind = 0;
    TEST("missing argument", getopt(argc, argv, "a:bc") == '?');
    TEST("optopt", optopt == 'a');
}

static void
test3(void)
{
    char *argv[] = {
        "test3",
        "-x",
        0
    };
    int argc = ARGC(argv);

    optind = 0;
    TEST("unknown option 1", getopt(argc, argv, "a:bc") == '?');
    optind = 0;
    TEST("unknown option 2", getopt(argc, argv, ":a:bc") == '?');
    TEST("optopt", optopt == 'x');
}

static void
test4(void)
{
    char *argv[] = {
        "test4",
        "-ayfoo",
        0
    };
    int argc = ARGC(argv);

    optind = 0;
    TEST("a", getopt(argc, argv, "abc") == 'a');
    TEST("unknown option", getopt(argc, argv, ":abc") == '?');
    TEST("optopt", optopt == 'y');
    TEST("unknown option stick", getopt(argc, argv, ":abc") == '?');
}

static void
test5(void)
{
    char *argv[] = {
        "test5",
        "-acb",
        "-c",
        0
    };
    int argc = ARGC(argv);

    optind = 0;
    TEST("a", getopt(argc, argv, "ab") == 'a');
    TEST("unknown option", getopt(argc, argv, "ab") == '?');
    TEST("b", getopt(argc, argv, "ab") == 'b');
    TEST("unknown option", getopt(argc, argv, "ab") == '?');
    optind = 2;
    TEST("missing argument", getopt(argc, argv, ":abc:") == ':');
    TEST("stop", getopt(argc, argv, "ab") == -1);
}

int
main(void)
{
    opterr = 0;
    test0();
    test1();
    test2();
    test3();
    test4();
    test5();
    printf("%d fail, %d pass\n", count_fail, count_pass);
    return count_fail != 0;
}
