#include <bits/stdc++.h>
using namespace std;

int main() {
    mt19937 mt(48);
    int T = 100; cout << T << '\n';
    for (int t = 0; t < T; t++) {
        int K = 2; cout << K << '\n';
        int N = 10;
        for (int i = 0; i < N; i++) {
            cout << char(mt() % 26 + 'a');
        }
        cout << '\n';
    }
}
