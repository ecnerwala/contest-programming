#include<bits/stdc++.h>
using namespace std;

int main() {
    const int N = 90000;
    const int M = N;
    uint32_t sum = 0;
    cout << N << ' ' << M << '\n';
    for (int i = 0; i < N; i++) {
        sum += N-i;
        cout << N - i << " \n"[i+1==N];
    }
    //cerr << int32_t(sum) << '\n';
}
