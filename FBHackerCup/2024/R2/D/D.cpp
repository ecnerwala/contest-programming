#include<bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }
template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		const int INF = 1e9;
		int N; cin >> N;
		std::vector<int> W(N);
		for (auto& w : W) { cin >> w; }
		std::vector<int> C(N);
		for (auto& c : C) { cin >> c; c--; assert(0 <= c && c < N); }
		auto has_solution = [&]() -> bool {
			struct color_t {
				int lo;
				int hi;
				int cnt = 0;
				std::vector<int> knowns;
				std::vector<int> unknowns;
			};
			std::vector<color_t> by_color(N);
			for (int c = 0; c < N; c++) {
				by_color[c].lo = INF, by_color[c].hi = -1;
			}
			for (int i = 0; i < N; i++) {
				by_color[C[i]].cnt++;
				if (W[i] == -1) {
					by_color[C[i]].unknowns.push_back(i);
				} else {
					by_color[C[i]].knowns.push_back(W[i]);
					setmin(by_color[C[i]].lo, W[i]);
					setmax(by_color[C[i]].hi, W[i]);
				}
			}
			for (auto& c : by_color) {
				if (!(c.cnt == 2 || c.cnt == 3)) return false;
				std::sort(c.knowns.begin(), c.knowns.end());
			}

			std::sort(by_color.begin(), by_color.end(), [&](const color_t& a, const color_t& b) { return a.lo < b.lo; });
			int F = 0;
			while (F < N && by_color[F].lo != INF) F++;

			std::vector<std::array<std::pair<int, std::pair<int, int>>, 3>> dp(F+1);
			for (int z = 0; z <= F; z++) {
				dp[z].fill({-INF, {-1, -1}});
			}
			dp[0][0] = {INF, {0, -1}};
			for (int idx = 0; idx < F; idx++) {
				const auto& color = by_color[idx];
				assert(color.lo != INF);
				assert(color.cnt == 2 || color.cnt == 3);
				for (int pz = 0; pz < 3; pz++) {
					if (dp[idx][pz].first == -INF) continue;
					int prv_hi = idx ? 0 : by_color[idx-1].hi + pz;

					int num_fixed = color.cnt - int(color.unknowns.size());
					if (color.hi - color.lo >= color.cnt - 1) {
						assert(num_fixed >= 2);
						// we just evaluate it
						assert(color.unknowns.size() <= 1);
						int nlo = color.lo;
						int nhi = color.hi;
						int nz = 0;

						int val = dp[idx][pz].first;
						if (nlo <= prv_hi) continue;
						val += nlo - prv_hi;
						// 0 -> 1
						if (val < 0) continue;
						val = -val;
						int nmi;
						// now, either we pick the midpoint
						if (color.cnt == 3) {
							if (num_fixed == 3) {
								nmi = color.knowns[1];
							} else {
								assert(val <= 0);
								if (val == 0) continue;
								assert(val < 0);
								nmi = std::min(nlo + (-val), nhi - 1);
							}
							assert(nmi > nlo);
							val += nmi - nlo;
							if (val > 0) continue;
							assert(val <= 0);
							val = 0;
						} else {
							nmi = nlo;
						}
						assert(nhi > nmi);
						val += nhi - nmi;
						val = -std::max(val, 0);
						assert(val <= 0);
						setmax(dp[idx+1][nz], {val, {pz, nmi}});
					} else {
						assert(color.hi - color.lo == num_fixed - 1);
						for (int nz = 0; nz < int(color.unknowns.size()); nz++) {
							int nhi = color.hi + nz;
							int nlo = nhi - color.cnt + 1;
							assert(nhi - nlo + 1 == color.cnt);
							if (nlo <= prv_hi) continue;

							int val = dp[idx][pz].first;
							val += nlo - prv_hi;
							// step by 1 each time
							if (color.cnt == 2) {
								if (val < 0) continue;
								val = -val;
								val ++;
								val = -std::max(val, 0);
								assert(val == -1 || val == 0);
							} else if (color.cnt == 3) {
								if (val < 0) continue;
								val = -val;
								val++;
								if (val > 0) continue;
								val = 0;
								val++;
								val = -std::max(val, 0);
								assert(val == -1);
							}
							setmax(dp[idx+1][nz], {val, {pz, -1}});
						}
					}
				}
			}

			int final_z = 0;
			while (final_z < 3 && dp[F][final_z].first == -INF) final_z++;
			if (final_z == 3) return false;

			// otherwise we're fine
			{
				int z = final_z;
				for (int idx = F-1; idx >= 0; idx--) {
					int nmi = dp[idx+1][z].second.second;
					const auto& color = by_color[idx];
					if (color.hi - color.lo >= color.cnt - 1) {
						assert(color.lo <= nmi && nmi < color.hi);
						if (color.cnt == 3) assert(color.lo < nmi);
						else assert(color.lo == nmi);

						assert(int(color.unknowns.size()) <= 1);
						if (color.unknowns.size()) {
							assert(color.cnt == 3);
							assert(color.unknowns.size() == 1);
							W[color.unknowns[0]] = nmi;
						}
					} else {
						assert(nmi == -1);
						int nhi = color.hi + z;
						int nlo = nhi - color.cnt + 1;
						assert(color.hi - color.lo + 1 == int(color.knowns.size()));
						int tmp = 0;
						for (int a = nlo; a <= nhi; a++) {
							if (a == color.lo || a == color.hi) {
								continue;
							}
							W[color.unknowns[tmp++]] = a;
						}
						assert(tmp == int(color.unknowns.size()));
					}
					z = dp[idx+1][z].second.first;
				}
				assert(z == 0);
			}

			for (int idx = F; idx < N; idx++) {
				// now just sprinkle these in at the end
				const auto& color = by_color[idx];
				assert(color.lo == INF);
			}

			return true;
		}();

		cout << "Case #" << case_num << ": " << (has_solution ? "Yes" : "No") << '\n';
		if (has_solution) {
			for (int i = 0; i < N; i++) {
				assert(W[i] != -1);
				cout << W[i] << " \n"[i+1==N];
			}
		}
	}

	return 0;
}
