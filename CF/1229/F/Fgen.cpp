#include<bits/stdc++.h>
using namespace std;

const int MAXN = 6e4;
int A[MAXN];
int L[MAXN];
int R[MAXN];
int main() {
    const int N = 35000;
    cout << N << '\n';
    for (int i = 0; i < N; i++) {
        A[i] = i;
        L[i] = (i+N/2)%N;
        R[i] = (i+N/2)%N;
    }
    for (int i = 0; i < N; i++) {
        cout << A[i] << ' ' << L[i] << ' ' << R[i] << '\n';
    }
}
