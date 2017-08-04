#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

ll N, M, K;

ll go() {
	if (N > M) {
		swap(N, M);
	}
	assert(N <= M);
	if (K >= N) return -1;
	assert (N > K);
	if (!(2 * K + 3 <= M)) {
		return -1;
	}
	if (N <= 2 * K) return 2;
	if (N <= 3 * K) return 3;
	if (K >= 2) return 4;
	assert(K == 1);
	assert(N >= 4);
	if (N == 4) return 4;
	if (N >= 5) return 5;
	assert(false);
	return -1;
}

int main() {
	int __T; cin >> __T;
	for (int T = 1; T <= __T; T++) {
		cin >> N >> M >> K;
		ll res = go();
		cout << "Case #" << T << ": ";
		cout << res << '\n';
	}

	return 0;
}
