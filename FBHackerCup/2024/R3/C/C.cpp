#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	cerr << fixed << setprecision(20);

	using ld = long double;
	int MX = int(1e8);
	std::vector<ld> suff(MX+1);
	for (int i = MX; i >= 1; i--) {
		suff[i-1] = suff[i] + ld(1) / ld(i);
	}

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int64_t N; cin >> N;
		int P; cin >> P;
		cerr << "Case #" << case_num << " " << "N = " << N << ' ' << "P = " << P << '\n';

		int64_t use_D_cap = N * P / (P + 100);
		// sum of k = use_D_cap+1..N of N / k
		assert(use_D_cap <= N);

		auto get_optimal_D = [&](int64_t k) {
			std::pair<ld, int> best(ld(N) / k, 1);
			for (int d = 1; true; d++) {
				int p_guaranteed = std::min(100, (d-1) * P);
				std::pair<ld, int> cnd(ld(d) / (
					p_guaranteed / ld(100) + (100 - p_guaranteed) / ld(100) * ld(k) / ld(N)
				), d);
				best = std::min(best, cnd);
				if (p_guaranteed == 100 || P == 0) break;
			}
			cerr << "  optimal " << k << ' ' << best.second << ' ' << best.first << '\n';
			return best;
		};

		auto get_harmonic = [&](int64_t lo, int64_t hi) -> ld {
			cerr << "harmonic " << lo+1 << ' ' << hi << '\n';
			if (hi <= MX) {
				return suff[lo] - suff[hi];
			}
			ld ans = 0;
			if (lo <= MX) {
				ans += suff[lo];
				lo = MX;
			}
			ans += std::log(ld(hi) / ld(lo)) +
				(ld(0.5) / hi - ld(0.5) / lo);
			return ans;
		};

		ld ans = 0;
		ans += N * get_harmonic(use_D_cap, N);
		cerr << ' ' << ans << '\n';
		get_optimal_D(use_D_cap + 1);
		get_optimal_D(N);
		
		if (use_D_cap > 0) {
			assert(P > 0);
			int D_normal = 1 + 99 / P;
			int P_guaranteed = (D_normal - 1) * P;
			assert(P_guaranteed <= 99);
			assert(D_normal >= 1);
			assert(100 - P_guaranteed > 0);

			auto get_sum_full = [&](int64_t lo, int64_t hi) -> ld {
				cerr << "full " << lo << ' ' << hi << '\n';
				get_optimal_D(lo);
				get_optimal_D(hi);
				assert(lo <= hi);
				ld res = ld(hi - lo + 1) * ld(D_normal + 1);
				cerr << ' ' << res << '\n';
				return res;
			};
			auto get_sum_most = [&](int64_t lo, int64_t hi) -> ld {
				cerr << "most " << lo << ' ' << hi << '\n';
				cerr << P_guaranteed << '\n';
				get_optimal_D(lo);
				get_optimal_D(hi);
				assert(lo <= hi);
				// we need the sum from k = lo .. hi of (100 * N) / (k * (100 - P_guaranteed) + N * P_guaranteed)
				ld res = 0;
				for (int i = 0; i <= int(1e8) && lo <= hi; i++) {
					res += ld(100 * N) / ld(lo * (100 - P_guaranteed) + N * P_guaranteed);
					lo++;
				}
				if (lo <= hi) {
					// half-open
					lo--;
					res +=
						ld(100 * N) * std::log(ld(hi * (100 - P_guaranteed) + N * P_guaranteed) / ld(lo * (100 - P_guaranteed) + N * P_guaranteed)) / ld(100 - P_guaranteed)
						+ (ld(50 * N) / ld(hi * (100 - P_guaranteed) + N * P_guaranteed)
						- ld(50 * N) / ld(lo * (100 - P_guaranteed) + N * P_guaranteed))
					;
				}
				res *= D_normal;
				cerr << ' ' << res << '\n';
				return res;
			};
			auto get_sum = [&](bool use_full, int64_t lo, int64_t hi) -> ld{
				return use_full ? get_sum_full(lo, hi) : get_sum_most(lo, hi);
			};

			auto use_full = [&](int64_t k) -> bool {
				int64_t prob_D = k * (100 - P_guaranteed) + N * P_guaranteed;
				int64_t prob_full = 100 * N;
				return __int128(prob_full) * D_normal >= __int128(prob_D) * (D_normal + 1);
			};
			int64_t mi = 1;
			int64_t ma = use_D_cap;
			bool use_full_mi = use_full(mi);
			bool use_full_ma = use_full(ma);
			if (use_full_mi != use_full_ma) {
				while (ma - mi > 1) {
					int64_t md = mi + (ma - mi) / 2;
					bool use_full_md = use_full(md);
					if (use_full_md == use_full_ma) ma = md;
					else mi = md;
				}
				ans += get_sum(use_full_mi, 1, mi);
				ans += get_sum(use_full_ma, ma, use_D_cap);
			} else {
				ans += get_sum(use_full_mi, 1, use_D_cap);
			}
			// We need to compare this to (N-k) / N * P - ((N-k) / N * P - k / N) / D_normal vs 1 / (D_normal+1)
			// We need to compare this to (N-k) / N * P - ((N-k) / N * P - k / N) / D_normal vs 1 / (D_normal+1)
			// Let a = k/N
			// (a + (1-a) * (D-1) * P) / D vs 1 / (D+1)
			// (D+1) * a + (1-a) * (D-1) * (D+1) * P vs D
			// (D+1) * a + (1-a) * (D^2 - 1) * P vs D
			// (D+1) * a
		}

		cout << fixed << setprecision(20);
		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}

// If we insert D bills, we get P(win) = k / N + (N-k) / N * (D-1) * P = (N-k) / N * P * D - (N-k) / N * P + k / N
// We want to minimize D / P(win), so we want to maximize P(win) / D = (N-k) / N * P - ((N-k) / N * P - k / N) / D
// There's a cutoff when (N-k) * P - k > 0 or < 0
// if (N - k) * P > k * 100, then we'd like to use as many D's as possible
// if (N - k) * P < k * 100, we'd like to use 0 D's
// N * P / (P + 100) <
