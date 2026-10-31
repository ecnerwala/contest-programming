#include<bits/stdc++.h>

int main(int argc, char* argv[]) {
    using namespace std;
    mt19937 mt(48);
    int N;
    if (argc >= 2) {
        sscanf(argv[1], "%d", &N);
    } else {
        N = 10;
    }
    assert(N % 2 == 0);

    int T = 1;
    cout << T << '\n';
    for (int t = 0; t < T; t++) {
        string A;
        int pref = 0;
        for (int z = 0; z < N; z++) {
            if (pref == 0) {
                A += '(';
                pref++;
            } else if (pref == N-z) {
                A += ')';
                pref--;
            } else {
                if (mt() & 1) {
                    A += '(';
                    pref++;
                } else {
                    A += ')';
                    pref--;
                }
            }
        }
        vector<int> L(N, 1);
        for (int i = 0; i < N; i++) L[i] = mt() % 10 + 1;
        vector<int> R(N, 1);
        for (int i = 0; i < N; i++) R[i] = mt() % 10 + 1;
        vector<int> P(N, 1);
        for (int i = 0; i < N; i++) P[i] = mt() % 10 + 1;

        const int Q = N * N;
        vector<int> S(Q);
        vector<int> E(Q);

        for (int i = 0; i < Q; i++) S[i] = i / N + 1, E[i] = i % N + 1;

        cout << N << ' ' << Q << '\n';
        cout << A << '\n';
        for (int i = 0; i < N; i++) cout << L[i] << " \n"[i+1==N];
        for (int i = 0; i < N; i++) cout << R[i] << " \n"[i+1==N];
        for (int i = 0; i < N; i++) cout << P[i] << " \n"[i+1==N];
        for (int i = 0; i < Q; i++) cout << S[i] << " \n"[i+1==Q];
        for (int i = 0; i < Q; i++) cout << E[i] << " \n"[i+1==Q];
    }
}
