#include <bits/stdc++.h>
using namespace std;

int main() {
    random_device rd;
    mt19937 mt(rd());

    const int N = 100;
    const int Q = int(1e5);
    cout << N << ' ' << Q << '\n';

    uniform_int_distribution<int> pt_dist(1, N);
    uniform_int_distribution<int> value_dist(1, 1000);
    uniform_int_distribution<int> op_dist(0, 11);

    for (int i = 2; i <= N; i++) {
        cout << i << ' ' << i/2 << '\n';
    }
    for (int i = 1; i <= N; i++) {
        cout << value_dist(mt) << '\n';
    }
    cout << pt_dist(mt) << '\n';
    for (int q = 0; q < Q; q++) {
        int op = op_dist(mt);
        cout << op << ' ';
        if (op == 0) {
            cout << pt_dist(mt) << ' ' << value_dist(mt) << '\n';
        } else if (op == 1) {
            cout << pt_dist(mt) << '\n';
        } else if (op == 2) {
            cout << pt_dist(mt) << ' ' << pt_dist(mt) << ' ' << value_dist(mt) << '\n';
        } else if (op == 3) {
            cout << pt_dist(mt) << '\n';
        } else if (op == 4) {
            cout << pt_dist(mt) << '\n';
        } else if (op == 5) {
            cout << pt_dist(mt) << ' ' << value_dist(mt) << '\n';
        } else if (op == 6) {
            cout << pt_dist(mt) << ' ' << pt_dist(mt) << ' ' << value_dist(mt) << '\n';
        } else if (op == 7) {
            cout << pt_dist(mt) << ' ' << pt_dist(mt) << '\n';
        } else if (op == 8) {
            cout << pt_dist(mt) << ' ' << pt_dist(mt) << '\n';
        } else if (op == 9) {
            cout << pt_dist(mt) << ' ' << pt_dist(mt) << '\n';
        } else if (op == 10) {
            cout << pt_dist(mt) << ' ' << pt_dist(mt) << '\n';
        } else if (op == 11) {
            cout << pt_dist(mt) << '\n';
        } else assert(false);
    }
}
