#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	int64_t vleaf; cin >> vleaf;
	K--;
	std::vector<int64_t> A(K+1);
	for (int i = 1; i <= K; i++) {
		cin >> A[i];
		A[i] += vleaf * (i-1);
	}

	N -= 2;

	const int64_t INF = int64_t(1e18);
	A[0] = INF;

	std::vector<int64_t> tmp(2*K+1);
	auto mul_eq = [&](std::vector<int64_t>& a, const std::vector<int64_t>& b) {
		std::fill(tmp.begin(), tmp.end(), INF);
		for (int i = 1; i <= K; i++) {
			for (int j = 1; j <= K; j++) {
				tmp[i+j] = std::min(tmp[i+j], a[i] + b[j]);
			}
		}
		for (int i = 2; i <= 2*K; i++) {
			for (int j = 1; j <= K && i+j <= 2*K; j++) {
				tmp[i+j] = std::min(tmp[i+j], tmp[i] + A[j]);
			}
		}
		for (int i = 1; i <= K; i++) {
			a[i] = tmp[i+K];
		}
	};

	std::vector<int64_t> base = A;
	std::vector<int64_t> resp(K+1, INF);
	resp[K] = 0;

	for (int e = (N-1+K)/K; e; e >>= 1) {
		if (e & 1) mul_eq(resp, base);
		mul_eq(base, base);
	}

	int64_t ans = resp[(N-1)%K+1];
	ans += vleaf * 2;
	cout << ans << '\n';

	return 0;
}
