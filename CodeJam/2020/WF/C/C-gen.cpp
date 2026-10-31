#include <bits/stdc++.h>
using namespace std;

char int_to_hex(int a) {
	if (0 <= a && a <= 9) return char('0' + a);
	else if (10 <= a && a <= 15) return char('A' + (a - 10));
	else assert(false);
}

int main() {
    int T = 100; cout << T << '\n';
    mt19937 mt(48);
    for (int t = 0; t < T; t++) {
        int N = 450;
        int D = 3;
        cout << N << ' ' << D << '\n';
        string S(D, '\0');
        for (int i = 0; i < D; i++) S[i] = int_to_hex(mt() & 15);
        string E(D, '\0');
        for (int i = 0; i < D; i++) E[i] = int_to_hex(mt() & 15);
        if (S > E) swap(S, E);
        cout << S << ' ' << E << '\n';
        for (int z = 0; z < N; z++) {
            string L(D, '\0');
            for (int i = 0; i < D; i++) L[i] = int_to_hex(mt() & 15);
            cout << L << " \n"[z+1==N];
        }
    }
}
