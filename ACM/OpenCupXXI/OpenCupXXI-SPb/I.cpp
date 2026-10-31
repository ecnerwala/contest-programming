#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

int N;
int ans[200];

int ev[15];

void gogo()
{
    cin >> N;

    for (int i = 0; i < N; i++)
        ev[i] = i;

    do {
        bool good = true;
        for (int i = 0; i < N; i++)
            if (ev[ev[i]] != i) good = false;
        if (!good) continue;

        int np = 0;
        for (int i = 0; i < N; i++)
            for (int j = i + 1; j < N; j++)
                if (ev[i] > ev[j])
                    np++;
        ans[np]++;
    } while (next_permutation (ev, ev + N));

    for (int i = 0; i <= N * (N - 1) / 2; i++)
        cout << i << " " << ans[i] % 2 << "\n";
}

int main()
{
    ios_base::sync_with_stdio(0);

    int T = 1;
    for (int t = 0; t < T; t++)
        gogo();
}