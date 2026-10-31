#include<bits/stdc++.h>
using namespace std;

int main() {
    int N = int(1e5);
    cout << N << '\n';
    for (int i = 0; i < N; i++) {
        cout << (1ll << (39 * (N-i) / N)) << " \n"[i+1==N];
    }
    for (int i = 0; i+1 < N; i++) {
        cout << i+1 << ' ' << i+2 << '\n';
    }
}
