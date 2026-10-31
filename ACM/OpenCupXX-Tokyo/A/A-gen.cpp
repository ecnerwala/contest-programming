#include<bits/stdc++.h>
using namespace std;

int main() {
    mt19937 mt(48);
    auto dist = uniform_int_distribution<int>(0, int(1e9)-1);
    const int N = 2e5;
    cout << N << '\n';
    for (int i = 0; i < N; i++) {
        cout << dist(mt) << " \n"[i+1==N];
    }
    const int M = 2e5;
    cout << M << '\n';
    for (int i = 0; i < M; i++) {
        cout << dist(mt) << " \n"[i+1==M];
    }
    for (int i = 0; i < M; i++) {
        cout << "BS"[mt()&1];
    }
    cout << '\n';
}
