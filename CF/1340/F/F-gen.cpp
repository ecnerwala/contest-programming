#include <bits/stdc++.h>
using namespace std;

void main_random() {
    const int N = 100;
    const int K = 5;
    cout << N << ' ' << K << '\n';

    mt19937 mt(rand());
    auto genVal = [&]() -> int{
        int a = uniform_int_distribution<int>(1, K)(mt);
        if (mt() & 1) a = -a;
        return a;
    };

    for (int i = 0; i < N; i++) {
        int a = genVal();
        cout << a << " \n"[i+1==N];
    }

    const int Q = 10000;
    cout << Q << '\n';
    for (int q = 0; q < Q; q++) {
        int op = uniform_int_distribution<int>(1, 2)(mt);
        if (op == 1) {
            int i = uniform_int_distribution<int>(1, N)(mt);
            int t = genVal();
            cout << op << ' ' << i << ' ' << t << '\n';
        } else if (op == 2) {
            int l = uniform_int_distribution<int>(1, N)(mt);
            int r = min(N, l+uniform_int_distribution<int>(2, 20)(mt));
            assert(l <= r);
            cout << op << ' ' << l << ' ' << r << '\n';
        } else assert(false);
    }
}

void main_big() {
    int N = int(1e5);
    assert(N % 2 == 0);
    int K = N;
    cout << N << ' ' << K << '\n';
    for (int i = 0; i < N; i++) {
        cout << (i >= N/2 ? -(N-i) : i+1) << " \n"[i+1==N];
    }
    const int Q = int(1e5);
    cout << Q << '\n';
    for (int q = 0; q < Q; q++) {
        if (true || (q & 1)) {
            cout << 1 << ' ' << N/2+1 << ' ' << -(N/2) << '\n';
        } else {
            cout << 2 << ' ' << 1 << ' ' << N << '\n';
        }
    }
}

int main() {
    main_big();
}
