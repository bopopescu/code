#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpc.h"

// linux
// gcc -std=c99 -Wall lisp.c mpc.c -ledit -lreadline -o lisp
// windows
// gcc -std=c99 -Wall lisp.c mpc.c -o lisp

/* If we are compiling on Windows compile these functions */
#ifdef _WIN32
#include <string.h>

static char buffer[2048];

/* Fake readline function */
char* readline(char* prompt) {
    fputs(prompt, stdout);
    fgets(buffer, 2048, stdin);
    char* cpy = malloc(strlen(buffer)+1);
    strcpy(cpy, buffer);
    cpy[strlen(cpy)-1] = '\0';
    return cpy;
}

/* Fake add_history function */
void add_history(char* unused) {
}

/* Otherwise include the editline headers */
#else
#include <editline/readline.h>
//#include <editline/history.h>
#endif

typedef struct lval {
    int type;
    long num;

    char *err;
    char *sym;

    int count;
    struct lval** cell;

} lval;

enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_SEXPR };
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

lval *lval_num(long x)
{
    lval *v = malloc(sizeof(lval));
    v->type = LVAL_NUM;
    v->num = x;
    return v;
}

lval *lval_err(char *m)
{
    lval *v = malloc(sizeof(lval));
    v->type = LVAL_ERR;
    v->err = malloc(strlen(m)+1);
    strcpy(v->err, m);
    return v;
}

lval *lval_sym(char *s)
{
    lval *v = malloc(sizeof(lval));
    v->type = LVAL_SYM;
    v->sym = malloc(strlen(s)+1);
    strcpy(v->sym, s);
    return v;
}

lval *lval_sexpr(void)
{
    lval *v = malloc(sizeof(lval));
    v->type = LVAL_SEXPR;
    v->count = 0;
    v->cell = NULL;
    return v;
}

void lval_del(lval *v)
{
    switch (v->type) {
        case LVAL_NUM: break;
        case LVAL_ERR: free(v->err); break;
        case LVAL_SYM: free(v->sym); break;
        case LVAL_SEXPR: 
            for (int i = 0; i < v->count; i++) {
                lval_del(v->cell[i]);
            }
            free(v->cell);
            break;
    }
    free(v);
}

void lval_print(lval v)
{
    switch (v.type) {
        case LVAL_NUM: printf("%li", v.num); break;
        case LVAL_ERR: printf("%s", v.err); break;
        case LVAL_SYM: printf("%s", v.sym); break;
#if 0
            if (v.err == LERR_DIV_ZERO) {
                printf("Error: division by zero");
            }
            if (v.err == LERR_BAD_OP) {
                printf("Error: invalid operator");
            }
            if (v.err == LERR_BAD_NUM) {
                printf("Error: invalid number");
            }
            break;
#endif
    }
}

int main(int argc, char** argv) {

    puts("Lisp Version 0.0.0.0.1");
    puts("Press Ctrl+c to Exit\n");

    mpc_parser_t* Number    = mpc_new("number");
    mpc_parser_t* Symbol    = mpc_new("symbol");
    mpc_parser_t* Sexpr     = mpc_new("sexpr");
    mpc_parser_t* Expr      = mpc_new("expr");
    mpc_parser_t* Lispy     = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
        "                                                   \
            number      : /-?[0-9]+/ ;                      \
            symbol      : '+' | '-' | '*' | '/' ;           \
            sexpr       : '(' <expr>* ')' ;                 \
            expr        : <number> | <symbol> | <sexpr> ;   \
            lispy       : /^/ <expr>* /$/ ;                 \
        ",
        Number, Symbol, Sexpr, Expr, Lispy);

    while (1) {

        /* Now in either case readline will be correctly defined */
        char* input = readline("lisp> ");

        add_history(input);

        //printf("%s\n", input);
        if (strcmp(input, "quit") == 0) {
            free(input);
            break;
        }

        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Lispy, &r)) {
            // on success print the ast
            //lval result = eval(r.output);
            //lval_println(result);
            mpc_ast_print(r.output);
            mpc_ast_delete(r.output);
        } else {
            // on error print the error
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        free(input);

    }

    mpc_cleanup(5, Number, Symbol, Sexpr, Expr, Lispy);

    return 0;
}

