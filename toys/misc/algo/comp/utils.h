#ifndef _UTILS_H_
#define _UTILS_H_

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>
#include <map>
#include <cstring>


using namespace std;

typedef long long ll; 
typedef pair<int, int> ii; 
typedef vector<ii> vii;
typedef vector<int> vi;
typedef map<string,int> msi;

const int INF = 0x3f3f3f3f;

/*
 *
 * Setting up values in array:
 * initialize DP memoization table with -1
 *  memset(memo, -1, sizeof memo); 
 *
 * to clear array of integers
 *  memset(arr, 0, sizeof arr); 
 *
 * Use a vector to create a dynamic array 
 * and initialize it in one statement.
 * Creates a vector of size N and values as 0.
 *  vector<int> v(N, 0); 
 *  OR vi v(N, 0);  *
 *
 *
 * to simplify: if (a) ans = b; else ans = c;
 *  ans = a ? b : c; 
 *
 * to simplify: ans = ans + val; and its variants
 *  ans += val; 
 *
 * index++; if (index >= n) index = 0;
 *  index = (index + 1) % n; 
 *
 * index--; if (index < 0) index = n - 1;
 *  index = (index + n - 1) % n; 
 *
 * for rounding to nearest integer
 *  int ans = (int)((double)d + 0.5); 
 *
 * min/max shortcut to update max/min
 *  ans = min(ans, new_computation); 
 *
 * To return true if a value matches a given number, else return false
 *  if (val == given_no) return true; 
 *  else return false;
 *   
 *  return (val == given_no) 
 * 
 *
 */

#endif // _UTILS_H_

