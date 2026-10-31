#include<bits/stdc++.h>
using namespace std;

int main() {
    const int T = 13;
    cout << T << '\n';
    for (int t = 1; t <= T; t++) {
        const int R = 1e8;
        const int N = 1e5;
        cout << N << '\n';
        for (int i = 0; i < N; i++) {
            double ang = M_PI * i / N;
            int x = int(cos(ang) * R);
            int y = int(sin(ang) * R);
            cout << x << ' ' << y << ' ' << -x << ' ' << -y << '\n';
        }
    }
}
