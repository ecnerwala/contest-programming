#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; int64_t H; cin >> N >> H;

	struct spell_t {
		int64_t t, d;
	};
	std::vector<spell_t> spells(N);
	for (auto& [t, d] : spells) cin >> t >> d;
	std::sort(spells.begin(), spells.end(), [&](auto a, auto b) { return a.t < b.t; });
	std::vector<int64_t> short_best(N+1);
	for (int i = N-1; i >= 0; i--) {
		short_best[i] = std::max(short_best[i+1], spells[i].d);
	}
	std::vector<int64_t> long_best(N+1);
	for (int i = 0; i < N; i++) {
		long_best[i+1] = std::max(long_best[i], spells[i].d * spells[i].t);
	}

	int64_t cur_t = 0;
	for (int i = 0; H > 0; i++) {
		if (i == N) {
			assert(long_best[N]);
			cur_t += (H + long_best[N] - 1) / long_best[N];
			H = 0;
			break;
		}
		int64_t nxt_t = spells[i].t;
		int64_t best_long = long_best[i];
		int64_t best_short = short_best[i];
		// cur_t .. nxt_t
		int64_t mid_t = best_short ? std::clamp(best_long / best_short, cur_t, nxt_t) : nxt_t;
		// mid_t * best_short <= best_long
		//cerr << cur_t << '-' << mid_t << "long " << best_long << '\n';
		//cerr << mid_t << '-' << nxt_t << "short " << best_short << "*t" << '\n';
		if (mid_t > cur_t) {
			if ((mid_t - cur_t) * best_long >= H) {
				cur_t += (H + best_long - 1) / best_long;
				H = 0;
				break;
			} else {
				H -= best_long * (mid_t - cur_t);
				cur_t = mid_t;
			}
		}
		if (nxt_t > cur_t) {
			assert(best_short);
			if ((nxt_t - cur_t) * (nxt_t + cur_t + 1) / 2 >= (H + best_short - 1) / best_short) {
				int64_t tgt = (H + best_short - 1) / best_short;
				int64_t mi = cur_t;
				int64_t ma = nxt_t;
				while (ma - mi > 1) {
					int64_t md = (mi + ma) / 2;
					if ((md - cur_t) * (cur_t + md + 1) / 2 >= tgt) {
						ma = md;
					} else {
						mi = md;
					}
				}
				cur_t = ma;
				H = 0;
				break;
			} else {
				H -= best_short * (nxt_t - cur_t) * (nxt_t + cur_t + 1) / 2;
				cur_t = nxt_t;
			}
		}
	}

	cout << cur_t << '\n';

	return 0;
}
