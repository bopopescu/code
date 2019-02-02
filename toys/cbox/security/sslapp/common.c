#include "common.h"
#include "thread.h"

void handle_error(const char *file, int lineno, const char *msg)
{
    fprintf(stderr, "** %s:%i %s\n", file, lineno, msg);
    ERR_print_errors_fp(stderr);
    exit(-1);
}

void init_OpenSSL(void)
{
    if (!THREAD_setup() || !SSL_library_init())
    {
        fprintf(stderr, "** OpenSSL initialization failed!\n");
    }
    SSL_load_error_strings();
}


