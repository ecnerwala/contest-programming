#include<bits/stdc++.h>
using namespace std;

int main() {
    int N = 20;
    cout << N << '\n';
    for (int i = 0; i < 2 * N; i++) {
        for (int j = 0; j < 2 * N; j++) {
            cout << (i!=j);
        }
        cout << '\n';
    }
}
