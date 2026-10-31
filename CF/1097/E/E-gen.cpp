#include<bits/stdc++.h>
using namespace std;

int main() {
    mt19937 mt(58);
    const int T = 1000;
    cout << T << '\n';
    for (int t = 0; t < T; t++) {
        //const int N = int(1e5);
        const int N = 5;
        vector<int> A(N);
        for (int i = 0; i < N; i++) A[i] = i+1;
        shuffle(A.begin(), A.end(), mt);

        cout << N << '\n';
        for (int i = 0; i < N; i++) {
            cout << A[i] << " \n"[i+1==N];
        }
    }
}
