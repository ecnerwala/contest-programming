#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = int(5e5);
    const int Q = int(5e5);
    cout << N << ' ' << Q << '\n';
    mt19937 mt(48);
    uniform_int_distribution<int> x_distribution(1, int(1e9));
    for (int i = 0; i < N; i++) {
        cout << x_distribution(mt) << " \n"[i+1==N];
    }

    uniform_int_distribution<int> i_distribution(1, N);

    for (int q = 0; q < Q; q++) {
        int op = (q % 2) + 1;
        if (op == 1) {
            cout << op << ' ' << i_distribution(mt) << ' ' << x_distribution(mt) << '\n';
        } else if (op == 2) {
            cout << op << ' ' << i_distribution(mt) << '\n';
        } else assert(false);
    }
}
