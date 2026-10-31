#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A(N);
		for (auto& a : A) cin >> a;
		cout << ([&]() -> bool {
			std::sort(A.begin(), A.end());
			if (std::unique(A.begin(), A.end()) != A.end()) return false;
			auto is_prime = [&](int p) -> bool {
				assert(p >= 2);
				for (int i = 2; i * i <= p; i++) if (p % i == 0) return false;
				return true;
			};
			for (int p = 2; 2 * p <= N; p++) {
				if (!is_prime(p)) continue;
				std::vector<int> seen(p);
				for (auto a : A) seen[a%p]++;
				if (*std::min_element(seen.begin(), seen.end()) >= 2) return false;
			}
			return true;
		}() ? "YES" : "NO") << '\n';
	}

	return 0;
}
