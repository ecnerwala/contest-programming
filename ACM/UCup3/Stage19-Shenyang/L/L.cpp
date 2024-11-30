#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int CASES; cin >> CASES;
	while (CASES--) {
		int N; int64_t P; cin >> N >> P;

		// team 0 has slightly less penalty
		int64_t p0_minus_p1 = -1;

		std::vector<int64_t> times; times.reserve(N+1);
		times.push_back(0);
		std::vector<int> weights; weights.reserve(N);

		int s0_minus_s1 = 0;

		{
			std::array<std::map<int, int>, 2> status;
			for (int i = 0; i < N; i++) {
				int a, b; int64_t c; int d; cin >> a >> b >> c >> d;
				a--;
				auto& v = status[a][b];
				if (v == -1) {
					// already solved, do nothing
				} else if (d == 1) {
					// solve the problem
					int64_t penalty = v * P + c;
					v = -1;
					p0_minus_p1 += 2 * (a ? -penalty : +penalty);
					times.push_back(c);
					weights.push_back(a ? -2 : +2);
					s0_minus_s1 += (a ? -1 : +1);
				} else if (d == 0) {
					v++;
				} else assert(false);
			}
		}

		if (p0_minus_p1 < 0) {
			p0_minus_p1 = -p0_minus_p1;
			for (auto& w : weights) w = -w;
		}
		assert(p0_minus_p1 > 0);
		int L = int(weights.size());
		assert(int(times.size()) == L+1);
		for (int i = L-1; i > 0; i--) {
			weights[i-1] += weights[i];
		}

		int64_t T = times.back() - times.front();
		std::tuple<int64_t, int64_t, int64_t> best{T+1, -1, -1};

		for (int z = 0; z < 2; z++) {
			std::reverse(times.begin(), times.end());
			for (auto& t : times) t = -t;
			std::reverse(weights.begin(), weights.end());

			//for (int i = 0; i < L; i++) { cerr << '@' << times[i] << ' ' << 'x' << weights[i] << ' '; } cerr << '@' << times[L] << '\n';

			// we need to find an interval which is > p0_minus_p1
			std::vector<std::pair<int64_t, int>> stk; stk.reserve(L);
			int64_t pref = 0;
			for (int i = 0; i <= L; i++) {
				// insert i
				{
					while (!stk.empty() && pref <= stk.back().first) {
						stk.pop_back();
					}
					stk.push_back({pref, i});
				}
				{
					int mi = -1;
					int ma = int(stk.size());
					while (ma - mi > 1) {
						int md = mi + (ma - mi) / 2;
						if (pref - stk[md].first > p0_minus_p1) {
							mi = md;
						} else {
							ma = md;
						}
					}
					if (mi >= 0) {
						int j = stk[mi].second;
						assert(j < i);
						int64_t t0 = times[j];
						int64_t excess = pref - stk[mi].first - p0_minus_p1;
						if (weights[j] > 0) {
							int64_t dt = excess / weights[j];
							t0 += dt;
							excess -= dt * weights[j];
							assert(t0 <= times[j+1]);
						}
						int64_t t1 = times[i];
						// t0 .. t1
						if (z == 0) {
							t0 = -t0;
							t1 = -t1;
							std::swap(t0, t1);
						}
						std::tuple<int64_t, int64_t, int64_t> cnd{t1 - t0, t0, excess};
						if (s0_minus_s1 == 0) {
							best = std::min(best, cnd);
						}
					}
				}
				if (i+1 < L) {
					pref += weights[i] * (times[i+1] - times[i]);
				}
			}
		}
		{
			auto [l, t0, excess] = best;
			if (t0 == -1) {
				cout << -1 << '\n';
			} else {
				int64_t t1 = t0 + l;
				int i = int(std::lower_bound(times.begin(), times.end(), t0) - times.begin());
				int j = int(std::lower_bound(times.begin(), times.end(), t1) - times.begin());
				assert(j >= i);
				if (i > 0) {
					int delta_weight = weights[j-1] - weights[i-1];
					if (delta_weight > 0) {
						// shift it left
						int64_t dt = excess / delta_weight;
						t0 -= dt;
						t1 -= dt;
					}
				}

				cout << t0 << ' ' << t1 << '\n';
			}
		}
	}

	return 0;
}
