#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
const int MAXN = 100100;

string res = "ACDEHIJKMORST";
int N;
int child[2][MAXN];
set <int> eset[MAXN];
vector <int> edge[MAXN];

bool seen[MAXN];
int nleft[MAXN];
int ans[MAXN];

bool nbad[MAXN];

void gogo()
{
    for (int i = 0; i < MAXN; i++)
    {
        eset[i].clear();
        edge[i].clear();
        child[0][i] = child[1][i] = 0;

        seen[i] = false;
        ans[i] = -1;
    }

    cin >> N;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cin >> child[i][j];
            child[i][j]--;
        }
    }

    bool bad = false;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int nv = child[j%2][i];
            int nn = child[j/2][nv];

            eset[i].insert(nv);
            eset[i].insert(nn);
            eset[nv].insert(i);
            eset[nn].insert(i);
            if (nv == i || nn == i)
                bad = true;
        }
    }

    if (bad)
    {
        cout << "IMPOSSIBLE\n";
        return;
    }

    vector <int> q;
    for (int i = 0; i < N; i++)
    {
        for (int x : eset[i])
            edge[i].push_back(x);

        nleft[i] = edge[i].size();
        if (edge[i].size() <= 12)
        {
            seen[i] = true;
            q.push_back(i);
        }

        /*cout << edge[i].size();
        for (int x : edge[i])
            cout << " " << x;
        cout << "\n";*/
    }

    for (int i = 0; i < q.size(); i++)
    {
        int cloc = q[i];
        for (int j : edge[cloc])
        {
            nleft[j]--;
            if (!seen[j] && nleft[j] <= 12)
            {
                seen[j] = true;
                q.push_back(j);
            }
        }
    }

    for (auto x : q)
    {
        //cout << x << "\n";
        for (int i = 0; i < 13; i++)
            nbad[i] = false;

        for (int y : edge[x])
        {
            if (ans[y] == -1) continue;
            nbad[ans[y]] = true;
        }

        ans[x] = 0;
        while (nbad[ans[x]]) ans[x]++;
    }

    for (int i = 0; i < N; i++)
        cout << res[ans[i]];
    cout << "\n";
}

int main()
{
    int T; cin >> T;

    for (int t = 0; t < T; t++)
    {
        cout << "Case #" << t + 1 << ": ";
        gogo();
    }
}