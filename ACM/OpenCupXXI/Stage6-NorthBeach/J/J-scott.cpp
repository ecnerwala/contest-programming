#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
const int MAXN = 1000100;

int N, M;
int arr[MAXN];
bool seen[MAXN];
vector <int> lx, hx, ly, hy, v;

int lxcur, hxcur, lycur, hycur;

ll e1[MAXN], e2[MAXN];

int gloc (int x, int y)
{
    return M * x + y;
}

int xc[4] = {-1, 1, 0, 0};
int yc[4] = {0, 0, -1, 1};

void flood (int x, int y)
{
    int cp = gloc (x, y);
    if (seen[cp]) return;

    lxcur = min (lxcur, x);
    hxcur = max (hxcur, x);
    lycur = min (lycur, y);
    hycur = max (hycur, y);

    seen[cp] = true;
    for (int i = 0; i < 4; i++)
    {
        int nx = x + xc[i];
        int ny = y + yc[i];
        if (nx < 0 || nx >= N) continue;
        if (ny < 0 || ny >= M) continue;

        int ncp = gloc (nx, ny);
        if (!seen[ncp] && (arr[ncp] == arr[cp]))
            flood (nx, ny);
    }
}

int main()
{
    ios_base::sync_with_stdio(0);

    while (cin >> N)
    {
        cin >> M;

        for (int i = 0; i < N * M; i++)
        {
            cin >> arr[i];
            seen[i] = false;
            e1[i] = e2[i] = 0;
        }
        lx.clear();
        ly.clear();
        hx.clear();
        hy.clear();
        v.clear();

        set <int> s;
        for (int i = 0; i < N * M; i++)
        {
            if (s.find(arr[i]) == s.end())
            {
                s.insert(arr[i]);

                lxcur = 1e9;
                lycur = 1e9;
                hxcur = -1e9;
                hycur = -1e9;
                flood (i / M, i % M);

                lx.push_back(lxcur);
                ly.push_back(lycur);
                hx.push_back(hxcur);
                hy.push_back(hycur);
                v.push_back(arr[i]);
            }
        }

        for (int i = 0; i < v.size(); i++)
        {
            int x1 = lx[i], x2 = hx[i], y1 = ly[i], y2 = hy[i];
            ll cv = v[i];
            if (x1)
            {
                for (int j = y1; j <= y2; j++)
                    e1[gloc (x1 - 1, j)] += cv;
            }
            for (int j = y1; j <= y2; j++)
                e1[gloc (x2, j)] += cv;

            if (y1)
            {
                for (int j = x1; j <= x2; j++)
                    e2[gloc (j, y1 - 1)] += cv;
            }
            for (int j = x1; j <= x2; j++)
                e2[gloc (j, y2)] += cv;
        }

        ll ans = 0;
        for (int i = 0; i < v.size(); i++)
        {
            int x1 = lx[i], x2 = hx[i], y1 = ly[i], y2 = hy[i];
            ll cv = v[i];
            ll ctot = 0;
            for (int j = x1; j < x2; j++)
                ctot += e1[gloc (j, y1)] + e1[gloc (j, y2)];
            for (int j = y1; j < y2; j++)
                ctot += e2[gloc (x1, j)] + e2[gloc (x2, j)];
            ans += (cv ^ (ctot / 2));

            cout << cv << " " << ctot << " " << ctot / 2 << "\n";
        }
        cout << ans << "\n";

        for (int i = 0; i < N * M; i++)
        {
            seen[i] = false;
            e1[i] = e2[i] = 0;
        }
    }
}