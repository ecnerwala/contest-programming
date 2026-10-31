#include <bits/stdc++.h>
int main() {
    using namespace std;
    mt19937 mt(48);

    int T = 100;
    cout << T << '\n';
    for (int t = 0; t < T; t++) {
        int N = (t >= T-6) ? 1687 : 100;
        int D = 1e7;
        cout << N << ' ' << D << '\n';
        for (int z = 0; z < N; z++) {
            cout << mt() % (5 * D) << ' ' << mt() % (5 * D) << '\n';
        }
    }
}
