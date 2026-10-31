#include <bits/stdc++.h>
int main() {
    using namespace std;
    int L = 100;
    int T = (L-1) * (L-1) * (L-1) * 2;
    cout << T << '\n';
    for (int i = 2; i <= L; i++) {
        for (int j = 2; j <= L; j++) {
            for (int k = 2; k <= L; k++) {
                cout << i << ' ' << j << ' ' << k << ' ' << 0 << '\n';
                cout << i << ' ' << j << ' ' << k << ' ' << 1 << '\n';
            }
        }
    }
}
