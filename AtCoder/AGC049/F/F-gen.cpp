#include <bits/stdc++.h>

int main() {
    using namespace std;

    mt19937 mt(48);

    int N = int(2e5);
    cout << N << '\n';
    for (int i = 0; i < N; i++) {
        cout << mt() % (int(2e5)+1) << '\n';
    }
    for (int i = 0; i < N; i++) {
        cout << mt() % (int(2e5)+1) << '\n';
    }
    for (int i = 0; i < N; i++) {
        cout << mt() % 5 + 1 << '\n';
    }
}
