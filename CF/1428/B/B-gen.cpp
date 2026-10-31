#include <bits/stdc++.h>
int main() {
    using namespace std;
    ios_base::sync_with_stdio(false), cin.tie(nullptr);
    int T = 1;
    cout << T << '\n';
    const int N = 3e5;
    cout << N << '\n';
    mt19937 mt(48);
    for (int i = 0; i < N; i++) {
        if (mt() % 10 < 9) {
            cout << '-';
        } else {
            cout << ((mt() & 1) ? '<' : '>');
        }
    }
    cout << '\n';
}
