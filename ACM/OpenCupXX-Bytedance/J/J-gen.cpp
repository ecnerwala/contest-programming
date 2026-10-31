#include <bits/stdc++.h>

int main() {
    using namespace std;

    int N = int(2e5);
    int Q = int(1e7);
    int K = 100;
    int S = 48;
    cout << N << ' ' << Q << ' ' << K << ' ' << S << '\n';
    deque<int> dq;
    for (int i = 0; i < N; i++) {
        if (i & 1) dq.push_back(i);
        else dq.push_front(i);
    }
    vector<int> A(dq.begin(), dq.end());
    for (int i = 0; i < N; i++) {
        cout << A[i] << " \n"[i+1==N];
    }

}
