#include<bits/stdc++.h>
using namespace std;

int main() {
    mt19937 mt(28835);
    int N = 1e5;
    int M = 1e5;
    cout << N << ' ' << M << '\n';
    for (int z = 0; z < 2; z++) {
        for (int i = 0; i < N; i++) {
            cout << char((mt() % 26) + 'a');
        }
        cout << '\n';
    }
}
