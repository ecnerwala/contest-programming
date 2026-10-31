#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2.1e5;
int N;
int A[MAXN];

const int MAXS = 1.1e6;
int S;
int prv[MAXS];

const int MAXQ = 1.1e5;
int Q;
int ans[MAXS];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}
	S = 0;
	for (int i = 0; i < N; i++) {
		for (int z = 0; z < A[i]; z++) {
			prv[S+z] = S;
		}
		S += A[i];
	}

	int maxA = *max_element(A, A + N);

	cin >> Q;
	for (int q = 0; q < Q; q++) {
		int t; cin >> t;
		if (t < maxA) {
			cout << "Impossible" << '\n';
			continue;
		}

		if (ans[t]) {
			cout << ans[t] << '\n';
			continue;
		}

		int cnt = 0;
		int cur = 0;
		while (cur < S) {
			cnt++;
			cur = prv[cur] + t;
		}
		cout << (ans[t] = cnt) << '\n';
	}

	return 0;
}
