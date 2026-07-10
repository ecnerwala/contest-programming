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
	auto solve = [&](int N, int M, const std::vector<bs>& S) -> int64_t {
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
		int64_t tot_ans = 0;
		for (const bool side : {0, 1}) {
			int64_t ans = 0;
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
			std::vector<edge_t> nxt(L, {L, 0});
			for (bool l_type : {false, true}) {
				std::vector<int> extra_pts; extra_pts.reserve(N+1);
				int k = 0;
				std::vector<int> stk; stk.reserve(L);
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
							//std::cerr << "match ";
							//std::cerr << pts[j].is_need << '@' << pts[j].loc << ' ';
							//std::cerr << pts[i].is_need << '@' << pts[i].loc << ' ';
							//std::cerr << cost << '\n';
							prv[i] = {j, cost};
							nxt[j] = {i, cost};
						}
					}
				}
			}

			std::vector<int> pt_vals(L);
			for (int i = 0; i < L; i++) {
				int j = prv[i].dest;
				pt_vals[i] = j == -1 ? 0 : prv[i].cost - pt_vals[j];
			}

			std::vector<int> ways_lo(K);
			std::vector<int> ways_hi(K);
			std::vector<int> tie_pair(K, -1);

			std::vector<pt_t> chain_pts; chain_pts.reserve(L);
			// number of below starts
			std::vector<int> pref_ends; pref_ends.reserve(L+1);
			// number of above ends
			std::vector<int> suff_ends; suff_ends.reserve(L+1);
			std::vector<int> want_vals; want_vals.reserve(L);
			for (int st = 0; st < L; st++) {
				if (prv[st].dest != -1) continue;
				chain_pts.clear();
				pref_ends.clear();
				suff_ends.clear();
				want_vals.clear();

				pref_ends.push_back(0);
				for (int i = st; i != L; i = nxt[i].dest) {
					if (!pts[i].is_need) {
						chain_pts.push_back(pts[i]);
						pref_ends.push_back(pref_valid_ends[i+1] - pref_valid_ends[prv[i].dest+1]);
						suff_ends.push_back(pref_valid_ends[nxt[i].dest+1] - pref_valid_ends[i+1]);
						want_vals.push_back(-pt_vals[i]);
					}
				}
				suff_ends.push_back(0);

				int C = int(chain_pts.size());
				for (int i = 0; i < C; i++) pref_ends[i+1] += pref_ends[i];
				for (int i = C; i > 0; i--) suff_ends[i-1] += suff_ends[i];

				//std::cerr << "have chain " << C << '\n';
				//std::cerr << ' '; for (int i : chain) { std::cerr << pts[i].is_need << '@' << pts[i].loc << '/' << pts[i].is_lo_ex.value_or(-1) << '/' << pts[i].is_hi_ex.value_or(-1) << ' '; } std::cerr << '\n';

				int64_t chain_ans = 0;

				auto ct = CartesianTree::build_min_tree(want_vals);
				if (!want_vals.empty()) {
					for (int z = 0; z < C; z ++) {
						assert(!chain_pts[z].is_need);
						assert(ct[2*z+1].m == z);
						int l = ct[2*z+1].l;
						int r = ct[2*z+1].r;
						assert(0 <= l && l <= z && z <= r && r < C);
						int64_t used_times = 1;
						used_times *= pref_ends[z+1] - pref_ends[l];
						used_times *= suff_ends[z] - suff_ends[r+1];
						//std::cerr << "remove " << z << ' ' << chain_vals[z] << ' ' << l << ' ' << r << ' ' << used_times << '\n';
						chain_ans += used_times * int64_t(want_vals[z]);

						// Now, let's do the bad pairs.
						{
							// First, detect a tie to the right and tighten our interval if so
							if (ct[2*z+1].c[1] & 1) {
								int cnd = ct[2*z+1].c[1] / 2;
								if (want_vals[z] == want_vals[cnd]) {
									if (chain_pts[z].is_hi_ex && chain_pts[cnd].is_lo_ex) {
										tie_pair[*chain_pts[z].is_hi_ex] = *chain_pts[cnd].is_lo_ex;
									}
									r = cnd - 1;
								}
							}
							if (chain_pts[z].is_lo_ex) {
								ways_lo[*chain_pts[z].is_lo_ex] = pref_ends[z+1] - pref_ends[l];
							}
							if (chain_pts[z].is_hi_ex) {
								ways_hi[*chain_pts[z].is_hi_ex] = suff_ends[z] - suff_ends[r+1];
							}
						}
					}
				}
				//std::cerr << "chain_ans " << chain_ans << '\n';
				ans += chain_ans;
			}

			// greedy match forwards and backwards
			{
				std::vector<pairnum<int, int64_t>> freqs(max_balance + 1);
				int cur_balance = pref_balances[0];
				pairnum<int, int64_t> pref_sum{0, 0};

				int64_t cur_weight = 0;
				//int64_t tot_pairs = 0;
				int idx = 0;
				for (int i = 0; i <= N; i++) {
					for (; idx < int(pts.size()) && pts[idx].loc < i; idx++) {
						if (pts[idx].is_need) {
							pref_sum -= freqs[cur_balance--];
						} else {
							pref_sum += freqs[++cur_balance];
						}
						cur_weight += pt_vals[idx];
					}
					if (pref[i] != side) {
						cur_weight++;
						continue;
					}
					ans += cur_weight * pref_sum.u - pref_sum.v;
					pairnum<int, int64_t> upd{1, cur_weight};
					pref_sum += upd, freqs[cur_balance] += upd;
				}
				//std::cerr << "tot_pairs " << tot_pairs << '\n';
			}

			//std::cerr << "baseline side " << ans << '\n';

			int64_t horiz_ans = 0;
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
		std::cout << solve(N, M, S) % 998244353 << '\n';
	}


	return 0;
}
