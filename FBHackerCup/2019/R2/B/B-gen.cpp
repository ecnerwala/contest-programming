#include<bits/stdc++.h>
using namespace std;

int main() {
    mt19937 mt(48);
    int T = 350; cout << T << '\n';
    for (int t = 0; t < T; t++) {
        int N = 4000;
        int M = 10000;
        cout << N << ' ' << M << '\n';
        for (int i = 0; i < M; i++) {
            int x = mt() % N;
            int y = min(x + (mt() % 5 < 1 ? 1 : 0), N-1);
            cout << x+1 << ' ' << y+1 << '\n';
        }
    }
}
