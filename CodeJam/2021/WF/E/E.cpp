#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

const int64_t INF = int64_t(1e18) + 100;
struct sat_num {
	int64_t v = 0;
	sat_num() {}
	sat_num(int64_t a) : v(std::min(a, INF)) {}
	explicit operator int64_t() const { return v; }
	friend bool operator < (sat_num a, sat_num b) { return a.v < b.v; }
	friend bool operator <= (sat_num a, sat_num b) { return a.v <= b.v; }
	friend bool operator >= (sat_num a, sat_num b) { return a.v >= b.v; }
	friend bool operator > (sat_num a, sat_num b) { return a.v > b.v; }

	sat_num& operator += (sat_num o) { v += o.v; if (v > INF) v = INF; return *this; }
	sat_num& operator *= (sat_num o) { if (v > 0 && o.v > INF / v) v = INF; else v *= o.v; return *this; }
	friend sat_num operator + (sat_num a, sat_num b) { return sat_num(a) += b; }
	friend sat_num operator * (sat_num a, sat_num b) { return sat_num(a) *= b; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		//cerr << "Case #" << case_num << '\n';

		int N; cin >> N;
		int64_t A, B; cin >> A >> B; A--, B--;

		int D = N+2;
		using mat = vector<sat_num>; // size D*D

		auto mult = [&](const mat& a, const mat& b) -> mat {
			mat r(D*D);
			for (int i = 0; i < D; i++) {
				for (int j = 0; j < D; j++) {
					for (int k = 0; k < D; k++) {
						r[i*D+k] += a[i*D+j] * b[j*D+k];
					}
				}
			}
			return r;
		};

		vector<array<int, 2>> L(N);
		for (int z = 0; z < 2; z++) {
			for (int i = 0; i < N; i++) {
				int l; cin >> l;
				if (l == 0) l = N;
				else l--;

				L[i][z] = l;
			}
		}

		vector<mat> p2; p2.reserve(65);
		{
			mat layer(D*D);
			for (int i = 0; i < N; i++) {
				layer[i*D+L[i][0]] += 1;
				layer[i*D+L[i][1]] += 1;
			}
			for (int i = 0; i < D; i++) {
				layer[i*D+(N+1)] += 1;
			}
			p2.emplace_back(std::move(layer));
		}

		auto get_depth = [&](int64_t v) -> pair<int64_t, int64_t> {
			assert(v < INF);
			int l = 0;
			while (v >= p2[l][N+1]) {
				l++;
				if (l == int(p2.size())) p2.push_back(mult(p2.back(), p2.back()));
			}
			if (l == 0) return {0, 0};
			mat cur = p2[l-1];
			int64_t ans = (int64_t(1) << (l-1));
			l--;
			while (l > 0) {
				l--;
				mat cnd = mult(cur, p2[l]);
				if (v >= cnd[N+1]) {
					ans += (int64_t(1) << l);
					cur = cnd;
				}
			}
			return {ans, v - int64_t(cur[N+1])};
		};

		auto rA = get_depth(A);
		int64_t dA = rA.first, xA = rA.second;
		auto rB = get_depth(B);
		int64_t dB = rB.first, xB = rB.second;

		assert(dB >= dA);

		{
			int num_sccs = 0;
			std::vector<int> scc_idx(N+1, -1);

			{ // Run Tarjan's SCC
				std::vector<int> idx(N+1, -1);
				std::vector<int> lowval(N+1);
				int nxt_idx = 0;
				std::vector<int> stk; stk.reserve(N+1);
				std::vector<bool> in_stack(N+1, false);
				std::y_combinator([&](auto self, int cur) -> void {
					assert(idx[cur] == -1);
					lowval[cur] = idx[cur] = nxt_idx++;
					stk.push_back(cur);
					in_stack[cur] = true;
					if (cur < N) {
						for (int nxt : L[cur]) {
							if (idx[nxt] == -1) {
								self(nxt);
								lowval[cur] = min(lowval[cur], lowval[nxt]);
							} else if (in_stack[nxt]) {
								lowval[cur] = min(lowval[cur], idx[nxt]);
							}
						}
					}
					if (lowval[cur] == idx[cur]) {
						while (in_stack[cur]) {
							int v = stk.back(); stk.pop_back();
							in_stack[v] = false;
							scc_idx[v] = num_sccs;
						}
						num_sccs++;
					}
				})(0);
			}

			std::vector<int> scc_size(num_sccs);
			std::vector<int> scc_edges(num_sccs);
			int tot_deg = 0;
			for (int i = 0; i <= N; i++) {
				if (scc_idx[i] == -1) continue;
				tot_deg++;
				scc_size[scc_idx[i]] ++;
				if (i < N) {
					for (int v : L[i]) {
						scc_edges[scc_idx[i]] += scc_idx[i] == scc_idx[v];
					}
				}
			}
			assert(tot_deg >= 1);

			std::vector<uint64_t> char_poly(tot_deg+1);
			char_poly[0] = 1;
			int cur_deg = 0;
			for (int c = 0; c < num_sccs; c++) {
				assert(scc_size[c] >= 1);
				if (scc_edges[c] > scc_size[c]) {
					goto is_branching;
				}
				if (scc_edges[c] == scc_size[c]) {
					// char_poly *= (1 - x^{scc_size[c]));
					for (int i = cur_deg; i >= 0; i--) {
						char_poly[i+scc_size[c]] -= char_poly[i];
					}
					cur_deg += scc_size[c];
				} else if (scc_edges[c] == 0 && scc_size[c] == 1) {
					// Do nothing
					cur_deg++;
				} else assert(false);
			}
			assert(cur_deg == tot_deg);

			using poly = std::vector<uint64_t>;
			poly POLY_1 = poly(tot_deg); POLY_1.back() = 1;
			poly POLY_X = poly(tot_deg);
			if (tot_deg == 1) POLY_X.back() = -char_poly[1];
			else POLY_X[tot_deg-2] = 1;

			auto mul_mod = [&](const poly& a, const poly& b) -> poly {
				assert(int(a.size()) == tot_deg);
				assert(int(b.size()) == tot_deg);
				poly r(tot_deg*2-1);
				for (int i = 0; i < int(a.size()); i++) {
					for (int j = 0; j < int(b.size()); j++) {
						r[i+j] += a[i] * b[j];
					}
				}
				for (int i = 0; i < tot_deg-1; i++) {
					for (int j = tot_deg; j > 0; j--) {
						r[i+j] -= r[i] * char_poly[j];
					}
				}
				r.erase(r.begin(), r.begin()+tot_deg-1);
				return r;
			};

			std::vector<poly> pow_x_pow_2;
			pow_x_pow_2.push_back(POLY_X);

			for (int l = 1; (int64_t(1) << l) <= dB; l++) {
				pow_x_pow_2.emplace_back(mul_mod(pow_x_pow_2.back(), pow_x_pow_2.back()));
			}

			auto pow_x = [&](int64_t a) -> poly {
				poly res = POLY_1;
				for (int i = 0; (a >> i); i++) {
					assert(i < int(pow_x_pow_2.size()));
					if ((a >> i) & 1) {
						res = mul_mod(res, pow_x_pow_2[i]);
					}
				}
				return res;
			};

			auto extrapolate = [&](const std::vector<uint64_t>& initial_vals, const poly& px) -> int64_t {
				assert(int(initial_vals.size()) == tot_deg);
				assert(int(px.size()) == tot_deg);
				uint64_t r = 0;
				for (int i = 0; i < tot_deg; i++) {
					r += px[i] * initial_vals[tot_deg-1-i];
				}
				return int64_t(r);
			};

			std::vector<std::vector<uint64_t>> tree_size(N+1, std::vector<uint64_t>(tot_deg));
			for (int i = 0; i <= N; i++) {
				tree_size[i][0] = 1;
			}
			for (int l = 1; l < tot_deg; l++) {
				for (int i = 0; i < N; i++) {
					tree_size[i][l] = tree_size[L[i][0]][l-1] + tree_size[L[i][1]][l-1];
				}
			}

			poly pow_delta_d = pow_x(dB-dA);

			int cur_color = 0;
			while (dA > 0) {
				//cerr << cur_color << ' ' << dA << ',' << xA << ' ' << dB << ',' << xB << '\n';
				int c = scc_idx[cur_color];
				if (scc_edges[c] > 0) {
					// Must be exactly 1 cycle
					for (int i = 0; i < N; i++) {
						if (scc_idx[i] == c) {
							assert((scc_idx[L[i][0]] == c) + (scc_idx[L[i][1]] == c) == 1);
						}
					}

					// We need to simulate the cycle
					std::vector<std::vector<uint64_t>> tree_cnt(N+1, std::vector<uint64_t>(tot_deg));
					tree_cnt[cur_color][0] = 1;
					for (int l = 1; l < tot_deg; l++) {
						for (int i = 0; i < N; i++) {
							uint64_t v = tree_cnt[i][l-1];
							if (!v) continue;
							for (int n : L[i]) {
								tree_cnt[n][l] += v;
								if (scc_idx[n] == c) break;
							}
						}
					}

					int n_color = cur_color;
					int64_t n_xA = xA;
					int64_t n_xB = xB;
					auto eval_bounds = [&](const poly& top_poly, const poly& bot_A_poly) -> bool {
						int64_t lo_A = 0;
						int64_t hi_A = 0;
						int64_t lo_B = 0;
						int64_t hi_B = 0;
						int mid_idx = -1;
						poly bot_B_poly = mul_mod(bot_A_poly, pow_delta_d);
						for (int i = 0; i <= N; i++) {
							int64_t cnt = extrapolate(tree_cnt[i], top_poly);
							if (!cnt) continue;
							int64_t sz_A = cnt * extrapolate(tree_size[i], bot_A_poly);
							int64_t sz_B = cnt * extrapolate(tree_size[i], bot_B_poly);
							if (scc_idx[i] == c) {
								assert(mid_idx == -1);
								mid_idx = i;
								hi_A += sz_A;
								hi_B += sz_B;
							} else {
								lo_A += sz_A;
								lo_B += sz_B;
							}
						}
						hi_A += lo_A;
						hi_B += lo_B;
						assert(mid_idx != -1);
						if (lo_A <= xA && xA < hi_A && lo_B <= xB && xB < hi_B) {
							n_color = mid_idx;
							n_xA = xA - lo_A;
							n_xB = xB - lo_B;
							return true;
						} else {
							return false;
						}
					};

					std::vector<std::pair<int, poly>> bots; bots.reserve(__builtin_popcountll(dA)+1);
					bots.push_back({-1, POLY_1});
					for (int64_t v = dA; v > 0; v &= v-1) {
						int i = __builtin_ctzll(v);
						bots.push_back({i, mul_mod(pow_x_pow_2[i], bots.back().second)});
					}
					poly cur_top = POLY_1;
					int64_t cur_d = 0;
					while (true) {
						if (bots.size() == 1) {
							assert(cur_d == dA);
							break;
						}
						int i = bots.back().first;
						bots.pop_back();

						poly cnd_top = mul_mod(cur_top, pow_x_pow_2[i]);
						if (eval_bounds(cnd_top, bots.back().second)) {
							cur_top = cnd_top;
							cur_d += int64_t(1) << i;
							continue;
						} else {
							poly cur_bot = bots.back().second;
							while (i > 0) {
								i--;
								cnd_top = mul_mod(cur_top, pow_x_pow_2[i]);
								poly cnd_bot = mul_mod(cur_bot, pow_x_pow_2[i]);
								if (eval_bounds(cnd_top, cnd_bot)) {
									cur_top = cnd_top;
									cur_d += int64_t(1) << i;
								} else {
									cur_bot = cnd_bot;
								}
							}
							break;
						}
					}
					dA -= cur_d;
					dB -= cur_d;
					cur_color = n_color;
					xA = n_xA;
					xB = n_xB;
				} else if (scc_edges[c] == 0) {
					// Do nothing, just run the naive version
				} else assert(false);

				//cerr << cur_color << ' ' << dA << ',' << xA << ' ' << dB << ',' << xB << '\n';

				if (dA == 0) break;

				assert(cur_color != N);
				int cnd_color = L[cur_color][0];

				poly pow_dA = pow_x(dA-1);
				int64_t cnd_dA = extrapolate(tree_size[cnd_color], pow_dA);
				bool zA = xA >= cnd_dA;
				poly pow_dB = mul_mod(pow_dA, pow_delta_d);
				int64_t cnd_dB = extrapolate(tree_size[cnd_color], pow_dB);
				bool zB = xB >= cnd_dB;
				if (zA != zB) break;
				dA --, dB --;
				if (zA) xA -= cnd_dA;
				if (zB) xB -= cnd_dB;
				cur_color = L[cur_color][zA];
			}
		}

		if (false) {
		is_branching:
			vector<vector<int64_t>> tree_size(dB+1);
			tree_size[0] = vector<int64_t>(N+1, int64_t(1));
			for (int l = 1; l <= dB; l++) {
				tree_size[l] = vector<int64_t>(N+1);
				for (int i = 0; i < N; i++) {
					tree_size[l][i] = tree_size[l-1][L[i][0]] + tree_size[l-1][L[i][1]];
				}
			}
			int cur_color = 0;
			while (dA > 0) {
				assert(0 <= xA && xA < tree_size[dA][cur_color]);
				bool zA = xA >= tree_size[dA-1][L[cur_color][0]];
				if (zA) {
					xA -= tree_size[dA-1][L[cur_color][0]];
				}
				assert(0 <= xB && xB < tree_size[dB][cur_color]);
				bool zB = xB >= tree_size[dB-1][L[cur_color][0]];
				if (zB) {
					xB -= tree_size[dB-1][L[cur_color][0]];
				}
				if (zA != zB) break;
				dA--, dB--;
				cur_color = L[cur_color][zA];
			}
		}

		cout << "Case #" << case_num << ": " << (dA + dB) << '\n';
	}

	return 0;
}
