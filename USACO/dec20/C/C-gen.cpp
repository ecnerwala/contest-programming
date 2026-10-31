#include <bits/stdc++.h>
int main() {
    using namespace std;
    int N = int(1e4);
    int K = 809399;
    mt19937 mt(48);
    vector<int> endpoints(2*N);
    for (auto& v : endpoints) {
        v = mt() % int(1e9);
    }
    sort(endpoints.begin(), endpoints.end());

    cout << N << ' ' << K << '\n';
    for (int i = 0; i < N; i++) {
        cout << endpoints[2*i] << ' ' << endpoints[2*i+1] << '\n';
    }
}
