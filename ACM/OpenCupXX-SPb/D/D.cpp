#pragma GCC optimize("Ofast", "unroll-loops")
#pragma GCC target("sse,sse2,sse3,ssse3,popcnt,abm,mmx")
#include <bits/stdc++.h>
 
using namespace std;
typedef long long ll;
const int MAXN = 100100;
const int BSIZE = 201;
const int P = (1 << 17);
const int MAXP = 2 * P + 100;
const ll D = (1LL << 44);

int N, Q;
int arr[MAXN];
int L1[MAXN], R1[MAXN], L2[MAXN], R2[MAXN];

pair <int, int> ord[MAXN];
int oloc[MAXN];
vector <pair <pair <int, int>, pair <int, int> > > nn;

inline bool cmp2 (pair <int, int> left, pair <int, int> right)
{
    int lb = left.first / BSIZE, rb = right.first / BSIZE;
    if (lb != rb) return lb < rb;
    //if (left.second == right.second) return left.first < right.first;
    if (lb % 2 == 0)
        return left.second < right.second;
    return left.second > right.second;
}

inline bool cmp (pair <pair <int, int>, pair <int, int> > left, pair <pair <int, int>, pair <int, int> > right)
{
    return cmp2 (left.first, right.first);
}

ll ctot;
ll seg[2][MAXP];

ll csum (int which, int ind)
{
    ll res = 0;

    for (int i = ind; i > 0; i -= i & (-i)) {
        res += seg[which][i];
    }
    return res;
}

void cadd (int which, int ind, int v)
{
    ll nx = v * (D + ord[ind].first);
    for (int i = ind+1; i <= P; i += i & (-i)) {
        seg[which][i] += nx;
    }
}

ll sscore (int which, int ind)
{
    int ntot = seg[which][P] / D;
    ll stot = seg[which][P] & (D - 1);

    ll rr = csum (which, ind);

    int nlo = rr / D;
    ll slo = rr & (D - 1);
    int nhi = ntot - nlo;
    ll shi = stot - slo;
    ll nx = ord[ind].first;
    return (shi - (nhi * nx)) + ((nlo * nx) - slo);
}

void sput (int which, int ind, int nadd) // 0/1, ind, 1/-1. ops and adds the diff
{
    ind = oloc[ind];
    ll cc = sscore (1 - which, ind);
    ctot += nadd * cc;
    cadd (which, ind, nadd);
}

void ent (int x, int y, int a, int b)
{
    if (x > y) swap (x, y);
    nn.push_back(make_pair (make_pair(x, y), make_pair (a, b)));
}

ll ans[MAXN];

int main()
{
    ios_base::sync_with_stdio(0);

    cin >> N >> Q;
    for (int i = 0; i < N; i++)
    {
        cin >> arr[i];
    }
    for (int i = 0; i < Q; i++)
    {
        cin >> L1[i] >> R1[i] >> L2[i] >> R2[i];
        L1[i]--;
        L2[i]--;

        ent (L1[i], L2[i], i, 1);
        ent (R1[i], L2[i], i, -1);
        ent (L1[i], R2[i], i, -1);
        ent (R1[i], R2[i], i, 1);
    }

    for (int i = 0; i < N; i++)
    {
        ord[i] = make_pair (arr[i], i);
    }
    sort (ord, ord + N);
    for (int i = 0; i < N; i++)
        oloc[ord[i].second] = i;

    sort (nn.begin(), nn.end(), cmp);
    /*for (int i = 0; i < 5; i++)
    {
        for (int j = 1; j + 2 < nn.size(); j++)
        {
            if (gd (nn[j-1], nn[j]) + gd (nn[j+1], nn[j+2]) > gd (nn[j-1], nn[j+1]) + gd (nn[j], nn[j+2]))
                swap (nn[j], nn[j+1]);
        }
    }*/

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < MAXP; j++)
            seg[i][j] = 0;

    int lv = 0, rv = 0;
    ctot = 0;
    for (int i = 0; i < nn.size(); i++)
    {
        int nl = nn[i].first.first, nr = nn[i].first.second;
        while (lv < nl)
        {
            sput (0, lv, 1);
            lv++;
        }
        while (lv > nl)
        {
            sput (0, lv - 1, -1);
            lv--;
        }
        while (rv < nr)
        {
            sput (1, rv, 1);
            rv++;
        }
        while (rv > nr)
        {
            sput (1, rv - 1, -1);
            rv--;
        }

        int a = nn[i].second.first, b = nn[i].second.second;
        ans[a] += b * ctot;
    }

    for (int i = 0; i < Q; i++)
    {
        cout << ans[i] << "\n";
    }
}
