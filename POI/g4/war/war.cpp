#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(1e6) + 200;
int N, M, K;
int C[MAXN];
int A[MAXN], B[MAXN];
int R[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M >>  K;
	for (int i = 0; i < N; i++) {
		cin >> C[i];
	}
	for (int i = 0; i < M; i++) {
		cin >> A[i] >> B[i];
	}
	for (int i = 1; i <= K; i++) {
		R[i] = i;
	}
	for (int i = M-1; i >= 0; i--) {
		R[A[i]] = R[B[i]];
	}
	for (int i = 0; i < N; i++) {
		if(i) cout << ' ';
		cout << R[C[i]];
	}
	cout << '\n';

	return 0;
}
