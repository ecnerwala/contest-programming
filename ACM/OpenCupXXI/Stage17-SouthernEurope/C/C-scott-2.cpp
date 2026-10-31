#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
const int MAXN = 30;
const int B = 2;

int N;
vector <int> ans;
int loc[3];

void figure (int X)
{
    loc[0] = 0;
    loc[1] = 1;
    loc[2] = 2;

    vector <int> goal;

    for (int i = 0; i < B; i++)
    {
        int xloc = 2 * i + 3;
        if (X & (1 << i))
        {
            if (i)
            {
                goal.push_back(xloc);
                goal.push_back(loc[1]);
            }
            int na = loc[2], nb = loc[0], nc = loc[1];
            loc[0] = na;
            loc[1] = nb;
            loc[2] = nc;
        }
        else
        {
            goal.push_back(xloc);
            goal.push_back(loc[2]);
            swap (loc[0], loc[1]);
        }

        goal.push_back(xloc + 1);
        goal.push_back(loc[1]);
        swap (loc[0], loc[2]);
    }

    if (X & (1 << B))
    {

    }
    else
    {
        goal.push_back(2 * B + 1);
        goal.push_back(loc[1]);
    }

    cout << goal.size() / 2 << "\n";
    for (int i = 0; i < goal.size(); i += 2)
    {
        cout << goal[i] + 1 << " " << goal[i+1] + 1 << "\n";
    }
}

void gogo()
{
    N = 3;
    for (int i = 0; i < 3; i++)
        for (int j = i + 1; j < 3; j++)
        {
            ans.push_back(i);
            ans.push_back(j);
        }

    int cloc = 1;
    loc[0] = 0;
    loc[1] = 1;
    loc[2] = 2;

    for (int i = 0; i < B; i++)
    {
        int nloc = N;
        ans.push_back(cloc);
        ans.push_back(nloc);
        cloc = nloc;
        N++;

        nloc = N;
        ans.push_back(cloc);
        ans.push_back(nloc);
        N++;
    }

    cout << N << " " << ans.size() / 2 << "\n";
    for (int i = 0; i < ans.size(); i += 2)
    {
        cout << ans[i] + 1 << " " << ans[i+1] + 1 << "\n";
    }

    for (int x = 1; x <= 500; x++)
        figure (x);
}

int main()
{
    ios_base::sync_with_stdio(0);

    int T = 1;
    for (int t = 0; t < T; t++)
        gogo();
}