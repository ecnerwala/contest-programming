#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t A, B; cin >> A >> B;

	int64_t num_ops = 0;
	if (A == B) {
		cout << 1 << '\n';
		exit(0);
	}

	std::vector<std::pair<int64_t, int>> pfac;
	for (int64_t v = std::abs(A-B), p = 2; v > 1; p ++) {
		if (p * p > v) p = v;
		if (v % p == 0) {
			int e = 0;
			while (v % p == 0) {
				e++;
				v /= p;
			}
			pfac.push_back({p, e});
		}
	}
	while (A >= 1 && B >= 1) {
		{
			int64_t g = std::gcd(A, B);
			A /= g, B /= g;
		}
		{
			std::vector<std::pair<int64_t, int>> n_pfac;
			n_pfac.reserve(pfac.size());
			int64_t v = std::abs(A-B);
			for (auto [p, e] : pfac) {
				if (v % p == 0) {
					int ne = 0;
					while (v % p == 0) {
						v /= p;
						ne++;
					}
					n_pfac.push_back({p, ne});
				}
			}
			assert(v == 1);
			pfac = std::move(n_pfac);
		}

		int64_t min_dist = std::min(A, B);
		for (auto [p, e] : pfac) {
			assert(A % p == B % p);
			min_dist = std::min(min_dist, A % p);
		}
		num_ops += min_dist;
		A -= min_dist;
		B -= min_dist;
	}
	cout << num_ops << '\n';

	return 0;
}
