#include<bits/stdc++.h>
using namespace std;

int main() {
    mt19937 mt(48);
    int N = 2000;
    int K = max(1, N / 2);
    cout << N << ' ' << K << '\n';
    int MAXX = int(10);
    for (int i = 0; i < N; i++) {
        int x = int(mt() % (2*MAXX)) - MAXX;
        int y = int(mt() % (2*MAXX)) - MAXX;
        cout << x << ' ' << y << '\n';
    }
}
