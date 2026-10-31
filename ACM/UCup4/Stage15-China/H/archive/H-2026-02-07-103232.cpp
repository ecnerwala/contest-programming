#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int K, N; cin >> K >> N;
	std::vector<int> A_(K);
	for (auto& a : A_) cin >> a;
	int A = *std::max_element(A_.begin(), A_.end());
	std::vector<int> B(N);
	for (auto& b : B) cin >> b;
	std::sort(B.begin(), B.end(), std::greater<>());

	K--;

	// If something is bigger than A in the first K-1, then we just take the max.
	// probability that the biggest is exactly the ith is (i-1 choose K-2) / (N choose K-1)
	// If everything is smaller than A, 
	int64_t tot = std::accumulate(B.begin(), B.begin() + (N-K), int64_t(0));
	double ans = 0;
	if (K == 0) {
		ans = std::max(double(A), double(tot) / N);
	} else {
		for (int i = N-K; i >= 0; i--, tot -= B[i]) {
			ans *= double(N-K-i) / double(N-1-i);
			int64_t cnd = tot + (N-K-i) * B[i];
			ans += std::max(double(std::max(A, B[i])), double(cnd) / (N-K));
		}
		ans *= double(K) / double(N);
	}

	cout << fixed << setprecision(20) << ans << '\n';

	return 0;
}
