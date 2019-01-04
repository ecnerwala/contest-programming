#include<bits/stdc++.h>
using namespace std;
using ll = long long;
ll cb (ll a) {
	return a * a * a;
}

ll slow(int N, int K) {
	ll res = 0;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			for (int k  = 1; k <= N; k++) {
				if ((i + j) % K == 0 && (j + k) % K == 0 && (k + i) % K == 0) {
					res ++;
				}
			}
		}
	}
	return res;
}

ll go(int N, int K) {
	ll res = cb(N / K);
	if (K % 2 == 0) {
		res += cb((N + K / 2) / K);
	}
	return res;
}

int main() {
	for (int i = 1; i <= 20; i++) {
		for (int j = 1; j <= 20; j++) {
			assert(go(i, j) == slow(i, j));
		}
	}
	int N, K; cin >> N >> K;
	cout << go(N, K) << '\n';

	return 0;
}
