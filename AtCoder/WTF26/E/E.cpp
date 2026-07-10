#include <bits/stdc++.h>
#include <cassert>

template <typename F> struct reverse_comparator_t {
	F f;
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) & {
		return f(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) const& {
		return f(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) && {
		return std::move(f)(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) const&& {
		return std::move(f)(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
};

template <typename F> constexpr reverse_comparator_t<std::decay_t<F>> reverse_comparator(F&& f) {
	return { std::forward<F>(f) };
}

class CartesianTree {
public:
	struct Node {
		int l, m, r; // inclusive ranges
		std::array<int, 2> c;
	};
	std::vector<Node> nodes;
	int root = -1;

	CartesianTree() {}

	Node& operator [] (int idx) { return nodes[idx]; }
	const Node& operator [] (int idx) const { return nodes[idx]; }

	int size() const { return int(nodes.size()); }

private:
	CartesianTree(std::vector<Node>&& nodes_, int root_) : nodes(std::move(nodes_)), root(root_) {}

public:

	// min-cartesian-tree, with earlier cells tiebroken earlier
	template <typename T, typename Comp = std::less<T>>
	static CartesianTree build_min_tree(const std::vector<T>& v, Comp comp = Comp()) {
		std::vector<Node> nodes(v.size()*2+1);
		std::vector<int> stk; stk.reserve(v.size());
		int root = -1;
		for (int i = 0; i <= int(v.size()); i++) {
			int cur = 2*i;
			nodes[cur].l = i;
			nodes[cur].r = i-1;
			nodes[cur].m = i-1;
			nodes[cur].c = {-1, -1};
			while (!stk.empty() && (i == int(v.size()) || comp(v[i], v[nodes[stk.back()].m]))) {
				int nxt = stk.back(); stk.pop_back();
				nodes[nxt].c[1] = cur;
				nodes[nxt].r = nodes[cur].r;
				cur = nxt;
			}
			if (i == int(v.size())) {
				root = cur;
				break;
			}
			nodes[2*i+1].l = nodes[cur].l;
			nodes[2*i+1].m = i;
			nodes[2*i+1].c[0] = cur;
			stk.push_back(2*i+1);
		}
		return {std::move(nodes), root};
	}

	// max-cartesian-tree, with earlier cells tiebroken earlier
	template <typename T, typename Comp = std::less<T>>
	static CartesianTree build_max_tree(const std::vector<T>& v, Comp comp = Comp()) {
		return build_min_tree(v, reverse_comparator(comp));
	}
};

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// abs coeff of a in x and y (they're always opposite sign)
	T vx = 1, vy = 0;
	bool swap = false;
	while (x) {
		T k = y / x;
		y %= x;
		vy += k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
		swap ^= 1;
	}
	assert(y == 1);
	return swap ? vy : m - vy;
}

template <int MOD_> struct modnum {
	// Uses subtraction to support MOD up to 2^31 - 1
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");
	struct is_reduced_tag {};

	int v;
	modnum() : v(0) {}
	modnum(int v_, is_reduced_tag) : v(v_) { assert(0 <= v && v < MOD); }
	static modnum from_reduced(int v) { return modnum(v, is_reduced_tag{}); }

	modnum(int v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	modnum(unsigned v_) : v(int(v_ % MOD)) { }
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	modnum(uint64_t v_) : v(int(v_ % MOD)) { }

	explicit operator int() const { return v; }
	int as_signed() const { return MOD-v > v ? v : v - MOD; }
	friend std::ostream& operator << (std::ostream& out, modnum n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (modnum a, modnum b) { return a.v == b.v; }
	friend bool operator != (modnum a, modnum b) { return a.v != b.v; }

	modnum inv() const { return from_reduced(mod_inv_in_range(v, MOD)); }
	friend modnum inv(modnum m) { return m.inv(); }
	modnum neg() const { return from_reduced(v ? MOD-v : 0); }
	friend modnum neg(modnum m) { return m.neg(); }

	modnum operator- () const {
		return neg();
	}
	modnum operator+ () const {
		return modnum(*this);
	}

	modnum& operator ++ () {
		v ++;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum& operator -- () {
		if (v == 0) v = MOD;
		v --;
		return *this;
	}
	modnum& operator += (modnum o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (modnum o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (modnum o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (modnum o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (modnum a, modnum b) { return modnum(a) += b; }
	friend modnum operator - (modnum a, modnum b) { return modnum(a) -= b; }
	friend modnum operator * (modnum a, modnum b) { return modnum(a) *= b; }
	friend modnum operator / (modnum a, modnum b) { return modnum(a) /= b; }
};

template <typename U, typename V> struct pairnum {
	U u;
	V v;

	pairnum() : u(0), v(0) {}
	pairnum(long long val) : u(val), v(val) {}
	pairnum(const U& u_, const V& v_) : u(u_), v(v_) {}

	friend std::ostream& operator << (std::ostream& out, const pairnum& n) { return out << '(' << n.u << ',' << ' ' << n.v << ')'; }
	friend std::istream& operator >> (std::istream& in, pairnum& n) { long long val; in >> val; n = pairnum(val); return in; }

	friend bool operator == (const pairnum& a, const pairnum& b) { return a.u == b.u && a.v == b.v; }
	friend bool operator != (const pairnum& a, const pairnum& b) { return a.u != b.u || a.v != b.v; }

	pairnum inv() const {
		return pairnum(u.inv(), v.inv());
	}
	pairnum neg() const {
		return pairnum(u.neg(), v.neg());
	}
	pairnum operator- () const {
		return pairnum(-u, -v);
	}
	pairnum operator+ () const {
		return pairnum(+u, +v);
	}

	pairnum& operator ++ () {
		++u, ++v;
		return *this;
	}
	pairnum& operator -- () {
		--u, --v;
		return *this;
	}

	pairnum& operator += (const pairnum& o) {
		u += o.u;
		v += o.v;
		return *this;
	}
	pairnum& operator -= (const pairnum& o) {
		u -= o.u;
		v -= o.v;
		return *this;
	}
	pairnum& operator *= (const pairnum& o) {
		u *= o.u;
		v *= o.v;
		return *this;
	}
	pairnum& operator /= (const pairnum& o) {
		u /= o.u;
		v /= o.v;
		return *this;
	}

	friend pairnum operator ++ (pairnum& a, int) { pairnum r = a; ++a; return r; }
	friend pairnum operator -- (pairnum& a, int) { pairnum r = a; --a; return r; }
	friend pairnum operator + (const pairnum& a, const pairnum& b) { return pairnum(a) += b; }
	friend pairnum operator - (const pairnum& a, const pairnum& b) { return pairnum(a) -= b; }
	friend pairnum operator * (const pairnum& a, const pairnum& b) { return pairnum(a) *= b; }
	friend pairnum operator / (const pairnum& a, const pairnum& b) { return pairnum(a) /= b; }
};

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
	using bs = std::bitset<10>;
	using num = modnum<998244353>;
	auto solve = [&](int N, int M, const std::vector<bs>& S) -> num {
		assert(int(S.size()) == N);
		auto popcount = [](bs a) -> int {
			return int(a.count());
		};
		std::vector<int> pref(N+1);
		for (int i = 0; i < N; i++) {
			pref[i+1] = (pref[i] + popcount(S[i])) & 1;
		}

		std::vector<int> odds; odds.reserve(N);
		for (int i = 0; i < N; i++) {
			if (pref[i+1] != pref[i]) {
				odds.push_back(i);
			}
		}
		int K = int(odds.size());
		if (K == 0) return 0; // everything's even already
		assert(K >= 1);

		std::vector<std::optional<int>> extra_cost_lo(N, std::nullopt);
		std::vector<std::optional<int>> extra_cost_hi(N, std::nullopt);
		std::vector<bool> extra_cost(K-1);
		for (int i = 0; i+1 < K; i++) {
			int a = odds[i], b = odds[i+1];
			bool has_extra = S[a] == S[b] && (popcount(S[a]) < M);
			for (int m = a+1; m < b; m++) {
				int v = popcount(S[m] ^ S[a]);
				assert(v & 1);
				if (v > 1) has_extra = false;
			}
			extra_cost[i] = has_extra;
			if (has_extra) {
				extra_cost_lo[a] = i;
				extra_cost_hi[b] = i;
			}
		}

		// Now, it's time to do the funny matching thing
		//for (int i = 0; i+1 < K; i++) { std::cerr << extra_cost[i]; } std::cerr << '\n';

		int M_cap = M - (M & 1);
		std::vector<int> balances(N);
		std::vector<int> pref_balances(N+1);
		for (int i = 0; i < N; i++) {
			balances[i] = M_cap - popcount(S[i]);
			pref_balances[i+1] = pref_balances[i] + balances[i];
		}
		{
			// shrink them down
			std::vector<int> pref_max = pref_balances;
			std::vector<int> suff_min = pref_balances;
			for (int i = 0; i < N; i++) {
				pref_max[i+1] = std::max(pref_max[i+1], pref_max[i]);
			}
			for (int i = N; i > 0; i--) {
				suff_min[i-1] = std::min(suff_min[i-1], suff_min[i]);
			}
			for (int i = 0; i < N; i++) {
				int gap = suff_min[i+1] - pref_max[i];
				assert(balances[i] >= gap);
				if (gap >= 0) {
					// only remove an even count
					balances[i] -= gap / 2 * 2;
				}
			}
			for (int i = 0; i < N; i++) {
				pref_balances[i+1] = pref_balances[i] + balances[i];
			}
			int a = *std::min_element(pref_balances.begin(), pref_balances.end());
			for (auto& v : pref_balances) v -= a;
			// now they're all positive
		}
		int max_balance = *std::max_element(pref_balances.begin(), pref_balances.end());
		int L = 0;
		for (int i = 0; i < N; i++) {
			L += std::abs(balances[i]);
		}
		assert(L <= 2*N);
		num tot_ans = 0;
		for (const bool side : {0, 1}) {
			num ans = 0;
			//std::cerr << '\n' << "doing side " << side << '\n';
			// we're going to find the matching for lo..hi
			struct pt_t {
				bool is_need;
				int loc;
				std::optional<int> is_lo_ex;
				std::optional<int> is_hi_ex;
			};
			std::vector<pt_t> pts; pts.reserve(L);
			std::vector<int> valid_ends; valid_ends.reserve(L + 1);
			valid_ends.push_back(0);
			for (int i = 0; i < N; i++) {
				if (pref[i] == side) {
					valid_ends.back()++;
				}
				if (balances[i] < 0) {
					// need 1 here
					pts.push_back({true, i, std::nullopt, std::nullopt});
					valid_ends.push_back(0);
				} else {
					int extras = balances[i];
					for (int z = 0; z < extras; z++) {
						auto is_lo_ex = pref[i] == side && z == 0 ? extra_cost_lo[i] : std::nullopt;
						auto is_hi_ex = pref[i] != side && z == extras-1 ? extra_cost_hi[i] : std::nullopt;
						if (is_hi_ex) {
							// we mark them on the same parity cause it's simpler
							assert(!pts.empty());
							pts.back().is_hi_ex = is_hi_ex;
						}
						pts.push_back({false, i, is_lo_ex, std::nullopt});
						valid_ends.push_back(0);
					}
				}
			}
			if (pref[N] == side) valid_ends.back()++;

			assert(int(pts.size()) == L);
			assert(int(valid_ends.size()) == L+1);
			std::vector<int> pref_valid_ends(L+2);
			for (int i = 0; i <= L; i++) pref_valid_ends[i+1] = pref_valid_ends[i] + valid_ends[i];
			//std::cerr << "valid "; for (int i = 0; i <= L; i++) { std::cerr << valid_ends[i] << ' '; } std::cerr << '\n';

			struct edge_t {
				int dest;
				int cost;
			};
			std::vector<edge_t> prv(L, {-1, 0});
			std::vector<edge_t> nxt(L, {-1, 0});
			for (bool l_type : {false, true}) {
				std::vector<int> extra_pts;
				int k = 0;
				std::vector<int> stk;
				for (int i = 0; i < L; i++) {
					for (; k <= pts[i].loc; k++) {
						if (pref[k] != side) {
							//std::cerr << "save at " << k << '\n';
							extra_pts.push_back(k);
						}
					}
					if (pts[i].is_need == l_type) {
						stk.push_back(i);
					} else {
						if (!stk.empty()) {
							int j = stk.back();
							stk.pop_back();
							int cost = pts[i].loc - pts[j].loc;
							while (!extra_pts.empty() && extra_pts.back() > pts[j].loc) {
								cost--;
								extra_pts.pop_back();
							}
							int cost2 = (i-j==1 && pref[pts[i].loc] != side) ? 0 :
								(pref[pts[i].loc] != side ? pts[i-1].loc : pts[i].loc)
								- (pref[pts[j].loc+1] != side ? pts[j+1].loc : pts[j].loc);
							std::cerr << "match ";
							std::cerr << pts[j].is_need << '@' << pts[j].loc << ' ';
							std::cerr << pts[i].is_need << '@' << pts[i].loc << ' ';
							std::cerr << cost << '\n';
							std::cerr << cost2 << ' ' << cost << '\n';
							assert(cost2 == cost);
							prv[i] = {j, cost};
							nxt[j] = {i, cost};
						}
					}
				}
			}

			std::vector<int> all_chain_vals(L);

			std::vector<num> ways_lo(K);
			std::vector<num> ways_hi(K);
			std::vector<int> tie_pair(K, -1);
			std::vector<int> chain; chain.reserve(L);
			for (int st = 0; st < L; st++) {
				if (prv[st].dest != -1) continue;
				chain.clear();
				for (int i = st; i != -1; i = nxt[i].dest) {
					chain.push_back(i);
				}
				int C = int(chain.size());
				//std::cerr << "have chain " << C << '\n';
				//std::cerr << ' '; for (int i : chain) { std::cerr << pts[i].is_need << '@' << pts[i].loc << '/' << pts[i].is_lo_ex.value_or(-1) << '/' << pts[i].is_hi_ex.value_or(-1) << ' '; } std::cerr << '\n';

				std::vector<int> chain_vals(C); // TODO: p sure this should be int
				chain_vals[0] = 0;
				for (int z = 0; z+1 < C; z++) {
					assert(pts[chain[z]].is_need != pts[chain[z+1]].is_need);
					chain_vals[z+1] = chain_vals[z] +
						(pts[chain[z]].is_need ? -1 : +1) * nxt[chain[z]].cost;
				}
				//std::cerr << ' '; for (auto v : chain_vals) { std::cerr << v << ' '; } std::cerr << '\n';
				for (int z = 0; z < C; z++) {
					all_chain_vals[chain[z]] = chain_vals[z] * ((pts[chain[z]].is_need) ? +1 : -1);
				}
				std::vector<std::array<int, 2>> ends(C+1);
				for (int z = 0; z <= C; z++) {
					int cur_ends = pref_valid_ends[z == C ? L+1 : chain[z] + 1]
						- pref_valid_ends[z == 0 ? 0 : chain[z-1] + 1];
					if (z == C ? !pts[chain[z-1]].is_need : pts[chain[z]].is_need) {
						// we're a peak
						ends[z][true] += cur_ends;
					} else {
						ends[z][false] += cur_ends;
					}
				}
				//std::cerr << ' '; for (auto v : ends) { std::cerr << v[1] << ' '; } std::cerr << '\n';
				//std::cerr << ' '; for (auto v : ends) { std::cerr << v[0] << ' '; } std::cerr << '\n';
				std::vector<std::array<int, 2>> pref_ends = ends;
				std::vector<std::array<int, 2>> suff_ends = ends;
				for (int z = 0; z+1 <= C; z++) {
					pref_ends[z+1][0] += pref_ends[z][0];
					pref_ends[z+1][1] += pref_ends[z][1];
				}
				for (int z = C; z-1 >= 0; z--) {
					suff_ends[z-1][0] += suff_ends[z][0];
					suff_ends[z-1][1] += suff_ends[z][1];
				}
				assert(pref_ends[C][0] + pref_ends[C][1] == pref_valid_ends.back());
				num chain_ans = 0;
				//std::cerr << "chain_ans all " << chain_ans << '\n';
				std::vector<int> want_vals; want_vals.reserve((C+1)/2);
				for (int z = 0; z < C; z++) {
					if (!pts[chain[z]].is_need) {
						assert(int(want_vals.size()) == (z/2));
						// we prefer not to pick the extra ones
						want_vals.push_back(chain_vals[z]);
					}
				}
				auto ct = CartesianTree::build_min_tree(want_vals);
				if (!want_vals.empty()) {
					bool x = !pts[chain[0]].is_need ? 0 : 1;
					assert(!pts[chain[x]].is_need);
					for (int z = x; z < C; z += 2) {
						assert(!pts[chain[z]].is_need);
						int wz = z / 2;
						assert(ct[2*wz+1].m == wz);
						int l = ct[2*wz+1].l * 2 + x;
						int r = ct[2*wz+1].r * 2 + x;
						assert(0 <= l && l <= z && z <= r && r < C);
						num used_times = 1;
						used_times *= pref_ends[z][0] - (l == 0 ? 0 : pref_ends[l-1][0]);
						used_times *= suff_ends[z+1][1] - (r+1 == C ? 0 : suff_ends[r+2][1]);
						//std::cerr << "remove " << z << ' ' << chain_vals[z] << ' ' << l << ' ' << r << ' ' << used_times << '\n';
						chain_ans += used_times * num(want_vals[wz]);

						// Now, let's do the bad pairs.
						{
							// First, detect a tie to the right and tighten our interval if so
							if (ct[2*wz+1].c[1] & 1) {
								int cnd = (ct[2*wz+1].c[1] / 2) * 2 + x;
								if (want_vals[z/2] == want_vals[cnd/2]) {
									if (pts[chain[z]].is_hi_ex && pts[chain[cnd]].is_lo_ex) {
										tie_pair[*pts[chain[z]].is_hi_ex] = *pts[chain[cnd]].is_lo_ex;
									}
									r = cnd - 1;
								}
							}
							if (pts[chain[z]].is_lo_ex) {
								ways_lo[*pts[chain[z]].is_lo_ex] = pref_ends[z][0] - (l == 0 ? 0 : pref_ends[l-1][0]);
							}
							if (pts[chain[z]].is_hi_ex) {
								ways_hi[*pts[chain[z]].is_hi_ex] = suff_ends[z+1][1] - (r+1 == C ? 0 : suff_ends[r+2][1]);
							}
						}
					}
				}
				//std::cerr << "chain_ans " << chain_ans << '\n';
				ans += chain_ans;
			}

			// greedy match forwards and backwards
			{
				std::vector<pairnum<int, num>> freqs(max_balance + 1);
				int cur_balance = pref_balances[0];
				pairnum<int, num> pref_sum{0, 0};

				num cur_weight = 0;
				//int64_t tot_pairs = 0;
				int idx = 0;
				for (int i = 0; i <= N; i++) {
					for (; idx < int(pts.size()) && pts[idx].loc < i; idx++) {
						if (pts[idx].is_need) {
							pref_sum -= freqs[cur_balance--];
						} else {
							pref_sum += freqs[++cur_balance];
						}
						cur_weight += all_chain_vals[idx];
					}
					if (pref[i] != side) {
						cur_weight++;
						continue;
					}
					ans += num(cur_weight) * num(pref_sum.u) - pref_sum.v;
					pairnum<int, num> upd{1, cur_weight};
					pref_sum += upd, freqs[cur_balance] += upd;
				}
				//std::cerr << "tot_pairs " << tot_pairs << '\n';
			}

			//std::cerr << "baseline side " << ans << '\n';

			num horiz_ans = 0;
			for (int k = 0; k < K; k++) {
				//std::cerr << "extra " << k << ' ' << ways_lo[k] << ' ' << ways_hi[k] << '\n';
				horiz_ans += ways_lo[k] * ways_hi[k];
				if (tie_pair[k] != -1) {
					//std::cerr << "tie_pair " << k << ' ' << tie_pair[k] << '\n';
					ways_lo[tie_pair[k]] += ways_lo[k];
				}
			}
			//std::cerr << "horiz_ans " << horiz_ans << '\n';
			tot_ans += horiz_ans;
			//std::cerr << "side ans " << ans << '\n';
			tot_ans += ans;
		}

		return tot_ans;
	};

	int T; std::cin >> T;
	while (T--) {
		int N, M; std::cin >> N >> M;
		assert(M <= int(bs().size()));
		std::vector<bs> S(N);
		for (int i = 0; i < N; i++) {
			std::string r; std::cin >> r;
			assert(int(r.size()) == M);
			for (int j = 0; j < M; j++) {
				assert(r[j] == 'O' || r[j] == '.');
				S[i][j] = r[j] == 'O';
			}
		}
		std::cout << solve(N, M, S) << '\n';
	}


	return 0;
}
