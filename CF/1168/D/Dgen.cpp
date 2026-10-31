#include<bits/stdc++.h>
using namespace std;
int main() {
    const int D = 500;
    const int N = D * (D+1) / 2;
    const int Q = 1.5e4;
    cout << N << ' ' << Q << '\n';
    int curNode = 1;
    for (int i = 2; i <= D; i++) {
        curNode ++;
        cout << i-1 << ' ' << '?' << '\n';
    }
    for (int i = D-1; i >= 1; i--) {
        curNode++;
        cout << i << ' ' << '?' << '\n';
        for (int j = i+2; j <= D; j++) {
            curNode++;
            cout << curNode-1 << ' ' << '?' << '\n';
        }
    }
    assert(curNode == N);
    for (int q = 0; q < Q; q++) {
        cout << D << ' ' << '?' << '\n';
    }
}
