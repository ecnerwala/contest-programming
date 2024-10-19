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
		cerr << "Case #" << case_num << ' ' << "N = " << N << '\n';
		auto has_solution = [&]() -> bool {
			return true;
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
				//cerr << c.cnt << ' ' << c.lo << '-' << c.hi << '\n';
				if (!(c.cnt == 0 || c.cnt == 2 || c.cnt == 3)) return false;
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
					int prv_hi = idx == 0 ? 0 : by_color[idx-1].hi + pz;
					cerr << "possible " << idx << ' ' << pz << ' ' << dp[idx][pz].first << '\n';
					cerr << "processing " << color.lo << '-' << color.hi << ' ' << color.cnt << ' ' << color.unknowns.size() << '\n';

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
							cerr << "  next " << idx+1 << ' ' << nz << ' ' << val << '\n';
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

			int cur_x = F == 0 ? 0 : by_color[F-1].hi + final_z;
			int cur_val = std::min(dp[F][final_z].first, 0);

			for (int idx = F; idx < N; idx++) {
				// now just sprinkle these in at the end
				const auto& color = by_color[idx];
				assert(color.lo == INF);
				if (color.cnt == 0) continue;
				assert(color.cnt == 2 || color.cnt == 3);
				assert(cur_val <= 0);
				if (color.cnt == 3) {
					// hit at least 1, then we set it to -1
					cur_x += std::max(1, 1 - cur_val);
					cur_val = -1;
					W[color.unknowns[0]] = cur_x+0;
					W[color.unknowns[1]] = cur_x+1;
					W[color.unknowns[2]] = cur_x+2;
					cur_x += 2;
				} else if (color.cnt == 2) {
					cur_x += std::max(1, 0 - cur_val);
					cur_val = -1;
					W[color.unknowns[0]] = cur_x+0;
					W[color.unknowns[1]] = cur_x+1;
					cur_x += 1;
				}
			}

			return true;
		}();

		cerr << "Case #" << case_num << ": " << (has_solution ? "Yes" : "No") << '\n';
		cout << "Case #" << case_num << ": " << (has_solution ? "Yes" : "No") << '\n';
		if (has_solution) {
			assert(int(std::set<int>(W.begin(), W.end()).size()) == N);
			for (int i = 0; i < N; i++) {
				assert(W[i] != -1);
				assert(1 <= W[i] && W[i] <= 10000);
				cout << W[i] << " \n"[i+1==N];
			}
			std::vector<int> w_sorted = W;
			std::sort(w_sorted.begin(), w_sorted.end());
			std::array<int, 2> dp{INF, 0};
			for (int i = 1; i < N; i++) {
				assert(w_sorted[i] > w_sorted[i-1]);
				dp[1] += w_sorted[i] - w_sorted[i-1];
				dp = {dp[1], std::min(dp[0], dp[1])};
			}
			int best = dp[0];
			std::vector<std::vector<int>> by_color(N);
			for (int i = 0; i < N; i++) {
				by_color[C[i]].push_back(W[i]);
			}
			int actual = 0;
			for (const auto& c : by_color) {
				if (c.empty()) continue;
				actual += (*std::max_element(c.begin(), c.end())) - (*std::min_element(c.begin(), c.end()));
			}
			assert(best == actual);
		}
	}

	return 0;
}
