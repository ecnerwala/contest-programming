#include<bits/stdc++.h>
using namespace std;

int main() {
    mt19937 mt(48);
    int N = 150000;
    cout << N << '\n';
    cout << 2 << '\n';
    for (int i = 0; i < N; i++) {
        int x = (mt() % int(2e9)) - int(1e9);
        int y = (mt() % int(2e9)) - int(1e9);
        cout << x << ' ' << y << '\n';
    }
}
