#include <bits/stdc++.h>

int main() {
    using namespace std;
    mt19937 mt(48);

    int T = 10000;
    cout << T << '\n';
    for (int z = 0; z < T; z++) {
        string A;
        int NA = 1 + (mt() % 60);
        for (int i = 0; i < NA; i++) {
            A += char('A' + (mt() % 26));
        }
        string B;
        int NB = 1 + (mt() % 60);
        for (int i = 0; i < NB; i++) {
            B += char('A' + (mt() % 26));
        }
        cout << A << ' ' << B << '\n';
    }
}
