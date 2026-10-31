#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, MOD, G; cin >> N >> MOD >> G;
	vector<int> A(N); for (auto& a : A) { cin >> a; a %= MOD; }

	auto fma = [&](int v, int a, int b) -> int {
		return int((int64_t(v) + int64_t(a) * int64_t(b)) % MOD);
	};
	auto add = [&](int a, int b) -> int {
		a += b;
		return a >= MOD ? a - MOD : a;
	};

	int neg1 = G;
	for (int k = N >> 1; k > 1; k >>= 1) neg1 = fma(0, neg1, neg1);

	for (int k = N >> 1; k >= 1; k >>= 1, G = fma(0, G, G)) {
		for (int i = 0; i < N; i += 2 * k) {
			int v = 1;
			for (int j = 0; j < k; j++, v = fma(0, v, G)) {
				int x = add(A[i+j], A[i+j+k]);
				int y = fma(0, fma(A[i+j], A[i+j+k], neg1), v);
				A[i+j] = x;
				A[i+j+k] = y;
			}
		}
	}

	int tot = 0; for (auto a : A) tot = add(tot, a);
	cout << tot << '\n';

	vector<int> rev(N);
	for (int i = 1; i < N; i++) {
		rev[i] = (rev[i>>1] | ((i&1)*N)) >> 1;
		cout << A[rev[i]] << ' ';
	}
	cout << A[0] << '\n';

	return 0;
}
