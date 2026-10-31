#include<bits/stdc++.h>
using namespace std;

int main() {
    int R = 500, C = 500, Q = 50000;
    mt19937 mt(48);
    cout << R << ' ' << C << ' ' << Q << '\n';
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            cout << char(mt() % 26 + 'A');
        }
        cout << '\n';
    }
    for (int q = 0; q < Q; q++) {
        for (int z = 0; z < 4; z++) {
            cout << char(mt() % 26 + 'A');
        }
        cout << '\n';
    }
}
