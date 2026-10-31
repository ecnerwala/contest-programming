#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t N, K; cin >> N >> K;
		N += 2;

		std::vector<std::pair<int64_t, int>> pfacs;
		for (int64_t p = 2, v = N; v > 1; p++) {
			if (p * p > v) p = v;
			if (v % p == 0) {
				int e = 0;
				while (v % p == 0) {
					v /= p;
					e++;
				}
				pfacs.push_back({p, e});
			}
		}

		std::vector<std::pair<int64_t, int>> mob_divs;
		mob_divs.reserve(1 << int(pfacs.size()));
		mob_divs.push_back({1, 1});
		for (auto [p, _] : pfacs) {
			int z = int(mob_divs.size());
			for (int i = 0; i < z; i++) {
				mob_divs.push_back({mob_divs[i].first * p, mob_divs[i].second * -1});
			}
		}

		auto num_leq = [&](int64_t a) -> int64_t {
			int64_t r = 0;
			for (auto [d, v] : mob_divs) {
				r += (a / d) * v;
			}
			return r;
		};

		int64_t mi = 0, ma = N;
		while (ma - mi > 1) {
			int64_t md = (mi + ma) / 2;
			if (num_leq(md) < K) {
				mi = md;
			} else {
				ma = md;
			}
		}
		if (ma == N) {
			cout << -1 << '\n';
		} else {
			int64_t v1 = ma, v0 = N - ma;
			assert(std::gcd(v1, v0) == 1);
			int first_char = v1 > v0 ? 1 : 0;
			if (first_char == 0) std::swap(v0, v1);
			assert(v0 < v1);
			std::vector<int64_t> blocks;
			while (v0) {
				blocks.push_back(v1 / v0);
				v1 %= v0;
				std::swap(v0, v1);
			}
			// backtrack by 1
			blocks.back() -= 1;

			cout << blocks.size() << ' '<< first_char << '\n';
			for (int z = 0; z < int(blocks.size()); z++) {
				cout << blocks[z] << " \n"[z+1==int(blocks.size())];
			}
		}
	}

	return 0;
}
