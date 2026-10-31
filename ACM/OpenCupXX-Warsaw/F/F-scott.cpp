#pragma GCC optimize("Ofast", "unroll-loops")
#pragma GCC target("sse,sse2,sse3,ssse3,popcnt,abm,mmx")
#include <bits/stdc++.h>
 
using namespace std;
typedef long long ll;
const int NDIG = 5;
const int MAXN = 100100;

unordered_map <int, int> mm[MAXN];
int mv[MAXN][10];
int p10[NDIG];
int lx[110][110];

int dfs (int L, int R);

int memv (int L, int R) // start at L, contains neither L nor R
{
    if (L / 100 == R / 100)
        return lx[L % 100][R % 100];
    if (mm[L].find(R) != mm[L].end())
        return mm[L][R];
    //cout << L << " " << R << "\n";
    return mm[L][R] = dfs (L, R);
}

int gdist (int x, int y)
{
    int ans = 0;
    for (int i = 0; i < 5; i++)
    {
        if (x % 10 != y % 10) ans++;
        x /= 10;
        y /= 10;
    }
    return ans;
}

int gotry (int L, int R, int M, int ans)
{
    if (M <= min (L, R) || M >= max (L, R))
        return 1e9;
    int ccost = gdist (L, M);
    if (abs (M - L) >= abs (M - R))
    {
        int cv = memv (M, L);
        if (cv + 1 + ccost >= ans) return ans;
        int lv = memv (M, R);
        return 1 + ccost + max (cv, lv);        
    }
    else
    {
        int cv = memv (M, R);
        if (cv + 1 + ccost >= ans) return ans;
        int lv = memv (M, L);
        return 1 + ccost + max (cv, lv);        
    }
}

int dfs (int L, int R)
{
    if (abs (L - R) <= 2) return 0;
    int M = (L + R) / 2;

    int ans = 1e9;
    for (int i = 4; i >= 0; i--)
    {
        int cg = (M / p10[i]) * p10[i] + (L % p10[i]);
        int ng = cg;
        if (cg > M)
        {
            ng -= p10[i];
        }
        else
        {
            ng += p10[i];
        }

        ans = min (ans, gotry (L, R, cg, ans));
        ans = min (ans, gotry (L, R, ng, ans));
    }
    return ans;
}

int ggotry (int L, int R, int M, int ans)
{
    if (M <= min (L, R) || M >= max (L, R))
        return 1e9;
    int ccost = gdist (0, M);
    int cv = memv (M, L);
    if (cv + 1 + ccost >= ans) return ans;
    int lv = memv (M, R);
    return 1 + ccost + max (cv, lv);
}

int gfs (int L, int R)
{
    if (abs (L - R) <= 2) return 0;
    int M = (L + R) / 2;

    int ans = 1e9;
    for (int i = 4; i >= 0; i--)
    {
        int cg = (M / p10[i]) * p10[i];
        int ng = cg;
        if (cg > M)
        {
            ng -= p10[i];
        }
        else
        {
            ng += p10[i];
        }

        ans = min (ans, ggotry (L, R, cg, ans));
        ans = min (ans, ggotry (L, R, ng, ans));
    }
    return ans;
}

int main()
{
    ios_base::sync_with_stdio(0);

    p10[0] = 1;
    for (int i = 1; i < 5; i++)
        p10[i] = 10 * p10[i-1];

    for (int i = 0; i <= 99; i++)
        for (int j = 0; j <= 99; j++)
        {
            lx[i][j] = dfs (i, j);
        }

    int T;
    cin >> T;
    for (int t = 0; t < T; t++)
    {
        int L, R;
        cin >> L >> R;
        cout << gfs (L - 1, R + 1) << "\n";
    }
}