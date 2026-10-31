#include "lcs.h"
using namespace std;

void find_permutation(int n, int res[])
{
    int A[3];
    for (int i = 0; i < 3; ++i) A[i] = 1;
    get_lcs(3, A); // 询问数组 [1, 1, 1] 与镁团手中排列的最长公共子序列。
    for (int i = 0; i < n; ++i) res[i] = i + 1; // 返回 1, 2, ..., n
}
