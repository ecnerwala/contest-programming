#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t S, K; cin >> S >> K;
		int64_t q = 1;
		while (S % q == 0) q++;
		// q is the first number we include
		std::vector<int64_t> first_vals(q, -1);
		first_vals[0] = q;

		std::vector<int64_t> lo_bound(q, 0);
		lo_bound[S % q] = S + 1;

		for (int z = 0; z < q; z++) {
			int64_t best_val = -1;
			for (int v = 0; v < q; v++) {
				if (first_vals[v] != -1) continue;
				int64_t mi = 0;
				for (int64_t it = 1; it < q; it++) {
					int64_t cur = v * it % q;
					mi = std::max(mi, (lo_bound[cur % q] + it - 1) / it);
				}
				while (mi % q != v) mi++;
				if (best_val == -1 || mi < best_val) best_val = mi;
			}
			if (best_val == -1) break;
			int64_t d = best_val % q;
			first_vals[d] = best_val;
			for (int iter = 0; iter < q; iter++) {
				for (int i = 0; i < q; i++) {
					lo_bound[i] = std::max(lo_bound[i], lo_bound[i + d >= q ? i + d - q : i + d] - best_val);
				}
			}
		}

		//for (int i = 0; i < q; i++) { cerr << first_vals[i] << " "; } cerr << '\n';

		auto get_leq = [&](int64_t a) -> int64_t {
			int64_t cnt = 0;
			for (int i = 0; i < q; i++) {
				if (first_vals[i] > a) continue;
				cnt += (a - first_vals[i] + q) / q;
			}
			return cnt;
		};

		int64_t mi = 0;
		int64_t ma = S;
		while (ma - mi > 1) {
			int64_t md = (mi + ma) / 2;
			if (get_leq(md) >= K) ma = md;
			else mi = md;
		}
		if (ma >= S) {
			cout << -1 << '\n';
		} else {
			cout << ma << '\n';
		}
	}

	return 0;
}
