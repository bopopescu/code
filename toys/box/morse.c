#include <stdio.h>
#include <string.h>


struct letter {
  int n;
  char a[8];
};

struct letter alphabet[256] = {
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},

  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},

  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},

  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},

  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {6, {'-','-','.','.','-','-'}},   // ,
  {1, {'.'}},
  {6, {'.','-','.','-','.','-'}},   // .
  {5, {'-','.','.','-','.'}},       // /
  {5, {'-','-','-','-','-'}},       // 0
  {5, {'.','-','-','-','-'}},       // 1
  {5, {'.','.','-','-','-'}},       // 2
  {5, {'.','.','.','-','-'}},       // 3
  {5, {'.','.','.','.','-'}},       // 4
  {5, {'.','.','.','.','.'}},       // 5
  {5, {'-','.','.','.','.'}},       // 6
  {5, {'-','-','.','.','.'}},       // 7
  {5, {'-','-','-','.','.'}},       // 8
  {5, {'-','-','-','-','.'}},       // 9
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {6, {'.','.','-','-','.','.'}},   // ?
  {6, {'.','-','-','.','-','.'}},   // @
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},
  {1, {'.'}},

  {2, {'.','-'}},                   // a
  {4, {'-','.','.','.'}},           // b
  {4, {'-','.','-','.'}},           // c
  {3, {'-','.','-','.'}},           // d
  {1, {'.'}},                       // e
  {4, {'.','.','-','.'}},           // f
  {3, {'-','-','.'}},               // g
  {4, {'.','.','.','.'}},           // h

  {2, {'.','.'}},                   // i
  {4, {'.','-','-','-'}},           // j
  {3, {'-','.','-'}},               // k
  {4, {'.','-','.','.'}},           // l
  {2, {'-','-'}},                   // m
  {2, {'-','.'}},                   // n
  {3, {'-','-','-'}},               // o
  {4, {'.','-','-','.'}},           // p

  {4, {'-','-','.','-'}},           // q
  {3, {'.','-','.'}},               // r
  {3, {'.','.','.'}},               // s
  {1, {'-'}},                       // t
  {3, {'.','.','-'}},               // u
  {4, {'.','.','.','-'}},           // v
  {3, {'.','-','-'}},               // w
  {4, {'-','.','.','-'}},           // x
  {4, {'-','.','-','-'}},           // y
  {4, {'-','-','.','.'}},           // z


};

void dot()
{
    printf(".");
}

void dash()
{
    printf("-");
}

void letter(char a)
{
    int i;
    //int idx = a-'a';
    int idx = a;
    struct letter l = alphabet[idx];
    printf("%c: ", a);
    for (i = 0; i < l.n; ++i) {
        if (l.a[i] == '.') dot();
        else dash();
    }
    printf("\n");
}

void word(char *str)
{
    int i;
    int len = strlen(str);
    for (i = 0; i < len; i++) {
        letter(str[i]);
    }
}

void list(char *str)
{
    int i;
    int len = strlen(str);
    for (i = 0; i < len; i++) {
        printf("%c => %d\n", str[i], str[i]);
    }
}

int main()
{
    char str[] = "abcdefghijklmnopqrstuvwxyz.,?/@1234567890";
    //char str[] = "abcdefghijklmnopqrstuvwxyz";
    //word(str);
    //list(str);
    word(str);
    //word("bart.");
    //word("jp");
}
