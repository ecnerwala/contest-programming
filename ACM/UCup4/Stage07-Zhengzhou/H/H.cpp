#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;
	std::vector<int> C(N);
	for (auto& c : C) cin >> c;

	auto solve = [&]() -> double {
		std::vector<int> freq(N+1, 0);
		int tot = 0;
		int64_t tot_C = 0;
		double dp = 0;
		for (int i = N-1; i >= 0; i--) {
			int a = A[i];
			int c = C[i];
			tot_C += c;
			freq[a]++;
			tot++;
			dp += c;
			dp *= freq[a] / double(2 * tot - freq[a]);
		}
		return (double(tot_C) + dp) / 2;
	};

	cout << fixed << setprecision(10);
	cout << solve() << '\n';

	int Q; cin >> Q;
	while (Q--) {
		int k; cin >> k;
		while (k--) {
			int p, x, y; cin >> p >> x >> y; p--;
			A[p] = x;
			C[p] = y;
		}
		cout << solve() << '\n';
	}

	return 0;
}
