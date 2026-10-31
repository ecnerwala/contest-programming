#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int64_t> A(N);
	for (auto& a : A) cin >> a;

	std::vector<int64_t> pref(N+1);
	for (int i = 0; i < N; i++) pref[i+1] = pref[i] + A[i];

	int Q; cin >> Q;
	std::vector<int64_t> queries(Q);
	for (auto& q : queries) cin >> q;

	const int64_t V = pref[N];
	for (auto& p : pref) { p = std::gcd(p, V); }
	for (auto& q : queries) {
		if (V % q) q = -1;
	}

	// factorization of V into relatively prime pieces
	std::vector<std::pair<int64_t, int>> pfacs;
	{
		int64_t v = V;
		for (int64_t p = 2; p * p * p <= v; p++) {
			if (v % p == 0) {
				int e = 0;
				while (v % p == 0) {
					v /= p;
					e++;
				}
				pfacs.push_back({p, e});
			}
		}
		if (v > 1) {
			int64_t a = int64_t(std::sqrt(v));
			a = (a + v/a) / 2;
			a = (a + v/a) / 2;
			a = (a + v/a) / 2;
			a = (a + v/a) / 2;
			if (a * a == v) {
				pfacs.push_back({a, 2});
			} else {
				pfacs.push_back({v, 1});
			}
		}
	}

	auto update_pfacs = [&](int64_t o) {
		if (!pfacs.empty() && pfacs.back().second == 1) {
			while (o % pfacs.back().first == 0) o /= pfacs.back().first;
			int64_t g = std::gcd(o, pfacs.back().first);
			if (g != 1) {
				int64_t a = g, b = pfacs.back().first / g;
				if (a > b) std::swap(a, b);
				pfacs.pop_back();
				pfacs.push_back({a, 1});
				pfacs.push_back({b, 1});
			}
		}
	};

	for (int i = 1; i < N; i++) {
		update_pfacs(pref[i]);
	}

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
		int64_t v = pref[i];
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
		if (v == -1) {
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
