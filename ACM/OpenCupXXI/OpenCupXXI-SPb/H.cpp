#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
const int MAXN = 125;

int N;

using bs = bitset<240>;

namespace std {
inline bool operator < (bs left, bs right)
{
    for (int i = 0; i < 240; i++)
        if (left[i] != right[i])
            return left[i] < right[i];
    return false;
}
}

map <pair <pair <bs, int>, int>, ll> mm;

bs clean (bs s, int l, int m)
{
    if (m == N || l == 1 || m * 2 <= N) return s;
    if (s[0] != 0 || s[1] != 0)
    {
        int a = s[0], b = s[1];
        for (int i = 0; i < 2 * l; i += 2)
        {
            if (s[i] == a && s[i+1] == b)
                s[i] = 0, s[i+1] = 0;
            else if (s[i] == 0 && s[i+1] == 0)
                s[i] = a, s[i+1] = b;
        }
    }
    int nleft = N - (m - l);
    bool bad = false;
    for (int i = l / 2 + 1; i * 2 <= nleft; i++)
    {
        bs v = s ^ (s >> 2 * i);
        if (v._Find_first() >= 2 * (l - i))
        {
            bad = true;
            break;
        }
    }
    if (bad) return s;
    return clean (s >> 2, l - 1, m);
}

ll figure (bs s, int l, int m);

ll flood (bs s, int l, int m)
{
    return 1;
    s = clean (s, l, m);
    if (mm.find (make_pair (make_pair (s, l), m)) != mm.end())
        return mm[make_pair (make_pair (s, l), m)];
    ll v = figure (s, l, m);
    mm[make_pair (make_pair (s, l), m)] = v;
    return v;
}

ll figure (bs s, int l, int m)
{
    if (m == N) return 1;
    ll ans = 0;
    for (int i = 0; i < 3; i++)
    {
        if (s[2*l-2] == i % 2 && s[2-l-1] == i / 2)
            continue;

        bs t = s;
        t[2*l] = i % 2;
        t[2*l+1] = i / 2;

        int k = l + 1;
        bool bad = false;
        for (int j = 1; j * 2 <= k; j++)
            if (((t >> (2 * (k - 2 * j))) ^ (t >> 2 * (k - j)))._Find_first() >= 2 * j)
            {
                bad = true;
                break;
            }
        if (bad) continue;

        ans += flood (t, l + 1, m + 1);
    }
    return ans;
}

void gogo()
{
    cin >> N;

    //cout << flood ("a", 1) * 3 << "\n";
}

int main()
{
    ios_base::sync_with_stdio(0);

    int T = 1;
    for (int t = 0; t < T; t++)
        gogo();
}
