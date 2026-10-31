#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr);

    mt19937 mt(48);
    std::uniform_int_distribution<int> coord_dist(0, int(1e5));

    const int N = 5;
    //const int N = int(1e5);
    cout << N << '\n';
    for (int i = 0; i < N; i++) {
        int x = coord_dist(mt);
        int y = coord_dist(mt);
        cout << x << ' ' << y << '\n';
    }
    const int M = 5;
    //const int M = int(1e5);
    cout << M << '\n';
    for (int j = 0; j < M; j++) {
        int x = coord_dist(mt);
        int y = coord_dist(mt);
        cout << x << ' ' << y << '\n';
    }
}
