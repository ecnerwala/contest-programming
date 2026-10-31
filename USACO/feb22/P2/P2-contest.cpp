#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int64_t> A(N);
	for (auto& a : A) cin >> a;

	std::vector<int64_t> pref(N+1);
	for (int i = 0; i < N; i++) pref[i+1] = pref[i] + A[i];

	const int64_t V = pref[N];
	// factorization of V into relatively prime pieces
	std::vector<std::pair<int64_t, int>> pfacs;
	if (V > 1) {
		pfacs.push_back({V, 1});
	}

	// find a minimal nontrivial factor of p
	auto get_new_pfac = [&](int64_t p, int64_t o) -> int64_t {
		assert(p > 1);
		while (true) {
			while (o % p == 0) o /= p;
			int64_t g = std::gcd(o, p);
			assert(g != p);
			if (g == 1) return p;

			p /= g;
			o /= g;

			if (std::gcd(p, g) == 1) {
				p = g;
			} else {
				o = g;
			}
		}
	};

	auto update_pfacs = [&](int64_t o) {
		int sz = int(pfacs.size());
		for (int z = 0; z < sz; z++) {
			auto [p, c] = pfacs[z];
			while (true) {
				int64_t np = get_new_pfac(p, o);
				assert(np > 1);
				assert(p % np == 0);
				if (np == p) {
					// we got nothing
					break;
				}
				int e = 0;
				while (p % np == 0) {
					p /= np;
					e++;
				}
				assert(std::gcd(p, np) == 1);
				if (p == 1) {
					p = np;
					c *= e;
					pfacs[z] = {p, c};
				} else {
					pfacs.push_back({np, e * c});
					pfacs[z] = {p, c};
				}
			}
		}
	};

	for (int i = 1; i < N; i++) {
		update_pfacs(pref[i]);
	}

	int Q; cin >> Q;
	std::vector<int64_t> queries(Q);
	for (auto& q : queries) cin >> q;

	for (auto& q : queries) {
		update_pfacs(q);
	}

	const int P = int(pfacs.size());
	std::vector<int> num_facs(P+1);
	num_facs[0] = 1;
	for (int z = 0; z < P; z++) {
		num_facs[z+1] = num_facs[z] * (pfacs[z].second + 1);
	}
	const int F = num_facs[P];
	std::vector<int> cnt(F);
	for (int i = 1; i < N; i++) {
		int idx = 0;
		int64_t v = std::gcd(pref[i], V);
		for (int z = 0; z < P; z++) {
			int64_t p = pfacs[z].first;
			while (v % p == 0) {
				v /= p;
				idx += num_facs[z];
			}
		}
		cnt[idx]++;
	}

	for (int z = 0; z < P; z++) {
		for (int i = F-1; i >= 0; i--) {
			if (i % num_facs[z+1] < num_facs[z]) continue;
			cnt[i - num_facs[z]] += cnt[i];
		}
	}

	for (int q = 0; q < Q; q++) {
		int64_t v = queries[q];
		if (V % v != 0) {
			cout << -1 << '\n';
		} else {
			int64_t ans = V / v + N - 2;

			int idx = 0;
			for (int z = 0; z < P; z++) {
				int64_t p = pfacs[z].first;
				while (v % p == 0) {
					v /= p;
					idx += num_facs[z];
				}
			}
			ans -= 2 * cnt[idx];

			cout << ans << '\n';
		}
	}

	return 0;
}
