#include <bits/stdc++.h>
using namespace std;

int main() {
    int N = int(1e5), Q = int(1e5);
    mt19937 mt(48);

    cout << N << ' ' << Q << '\n';
    cout << string(100, 'a') << '\n';
    cout << string(N, 'a') << '\n';
    for (int q = 0; q < Q; q++) {
        if (q == 0) {
            cout << N << ' ' << string(int(1e6), 'a') << '\n';
        } else {
            string w; for (int z = 0; z < 10; z++) w += char(mt() % 26 + 'a');
            cout << N << ' ' << w << '\n';
        }
    }
}
