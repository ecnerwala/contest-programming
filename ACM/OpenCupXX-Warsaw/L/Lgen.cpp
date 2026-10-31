#include<bits/stdc++.h>
using namespace std;

int main() {
    mt19937_64 mt(48);
    int N = 150;
    cout << N << '\n';
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << mt() << " \n"[i+1==N];
        }
    }
}
