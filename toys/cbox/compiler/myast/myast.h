#ifndef _MY_AST_H_
#define _MY_AST_H_

#include <stdbool.h>
#include <string.h>
#include <ctype.h>


typedef enum { EOF_T, EOL_T, CONSTANT_T, OPERATOR_T, VARIABLE_T, RIGHTPAREN_T } inputtype;
typedef struct {
    inputtype tokentype;
    char tokenvalue;
} tokendata;

bool isoperator(char c);
tokendata *gettoken();


#endif // _MY_AST_H_
