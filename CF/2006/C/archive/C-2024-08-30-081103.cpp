#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N); for (auto& a : A) cin >> a;
		std::vector<int> V(N-1);
		for (int i = 0; i+1 < N; i++) {
			int v = std::abs(A[i] - A[i+1]);
			if (v) v >>= __builtin_ctz(v);
			V[i] = v;
		}
		N--;

		for (int v : V) { cerr << v << ' '; } cerr << '\n';

		int64_t ans = N+1;
		// we want the number of (possibly empty) intervals of V whose gcd is 0 or 1
		std::vector<std::pair<int, int>> freq; freq.reserve(32);
		for (int v : V) {
			cerr << "add " << v << '\n';
			{
				// add a 0
				if (freq.empty() || freq.back().first != 0) freq.push_back({0, 1});
				else freq.back().second++;
			}
			// adjust to v
			for (auto& [a, c] : freq) {
				a = std::gcd(a, v);
				if (a == 1 || a == 0) {
					ans += c;
				}
			}
			// run length encode
			int L = 0;
			for (int i = 0; i < int(freq.size()); i++) {
				if (L && freq[i].first == freq[L-1].first) {
					freq[L-1].second += freq[i].first;
				} else {
					freq[L++] = freq[i];
				}
			}
			freq.resize(L);
			for (auto [a, c] : freq) { cerr << ' ' << a << ' ' << c << '\n'; }
		}
		cout << ans << '\n';
	}

	return 0;
}
