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

template <typename T> struct extended_gcd_result {
	T gcd;
	T coeff_a, coeff_b;
};
template <typename T> extended_gcd_result<T> extended_gcd(T a, T b) {
	T x = a, y = b;
	// coeff of a and b in x and y
	T ax = 1, ay = 0;
	T bx = 0, by = 1;
	while (x) {
		T k = y / x;
		y %= x;
		ay -= k * ax;
		by -= k * bx;
		std::swap(x, y);
		std::swap(ax, ay);
		std::swap(bx, by);
	}
	return {y, ay, by};
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
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

struct mod_goldilocks {
	static constexpr uint64_t MOD = 0xffffffff00000001ull;
	static constexpr uint64_t EPS = -MOD;
	// We have 2^32 is a primitive 6th root of unity.
	// Note that omega_8 + omega_8^7 == 2^24 - 2^72 == sqrt(2)
	// We'll pick the root so that 2^24 - 2^72 is our primitive 384th root of unity.
	static constexpr uint64_t PRIMITIVE_ROOT = 2717;
	struct is_reduced_tag {};
	uint64_t v;
	mod_goldilocks() : v(0) {}
	mod_goldilocks(uint64_t v_, is_reduced_tag) : v(v_) { assert(v < MOD); }
	mod_goldilocks(int64_t a) : v(a < 0 ? a+MOD : a) {}
	mod_goldilocks(int a) : mod_goldilocks(int64_t(a)) {}
	mod_goldilocks(uint64_t a) : v(a >= MOD ? a-MOD : a) {}
	mod_goldilocks(unsigned a) : mod_goldilocks(uint64_t(a)) {}
	mod_goldilocks(__int128_t a) : v(a % MOD < 0 ? uint64_t(MOD - a % MOD) : uint64_t(a % MOD)) {}
	mod_goldilocks(__uint128_t a) : v(uint64_t(a % MOD)) {}

	static mod_goldilocks from_reduced(uint64_t v) {
		return mod_goldilocks(v, is_reduced_tag{});
	}

	explicit operator uint64_t () const { return v; }
	int64_t as_signed() const { return MOD-v > v ? v : int64_t(v - MOD); }
	friend std::ostream& operator << (std::ostream& out, mod_goldilocks n) { return out << uint64_t(n); }

	friend bool operator == (mod_goldilocks a, mod_goldilocks b) { return a.v == b.v; }
	friend bool operator != (mod_goldilocks a, mod_goldilocks b) { return a.v != b.v; }

	mod_goldilocks operator+ () const { return *this; }

	// returns a-b, assuming -MOD <= a-b, e.g. b <= MOD
	static uint64_t sub_mod_raw(uint64_t a, uint64_t b) {
#if defined(__x86_64__)
		// TODO: We could try to write this using intrinsics, but GCC sometimes produces the wrong code.
		uint64_t res_wrapped = a;
		uint64_t adjustment = b;
		asm (
			// AT&T syntax: SRC DST
			"sub %[y], %[x]\n\t"
			// Trick from plonky2 implementation:
			// After sub, flag CF is set iff we underflowed. We want to correct by EPS == 2^32 - 1 iff C is set.
			// sbb (subtract with borrow) computes DST <- DST - SRC - CF
			// Thus, we can use the 32-bit form of sbb on a dummy register to load CF ? EPS : 0.
			// Here, we'll just reuse the original register holding b.
			"sbb %k[y], %k[y]\n\t"
			: [x] "+r"(res_wrapped),
			[y] "+r"(adjustment)
			:
			: "cc"
		);
#else
		uint64_t res_wrapped = a - b;
		uint64_t adjustment = (res_wrapped > a) ? EPS : 0;
#endif
		return res_wrapped - adjustment;
	}

	// Reduce lo + 2^64 * mi + 2^96 * hi, where hi <= MOD
	static uint64_t reduce_u160_raw(uint64_t lo, uint32_t mi, uint64_t hi) {
		// result = lo - hi + EPS * mi
		// 0 <= lo <= 2^64 - 1 = MOD + EPS - 1
		// 0 <= EPS * mi <= (2^32 - 1) * EPS = MOD - 1 - EPS
		// 0 <= hi <= MOD
		// -MOD <= lo - hi + EPS * mi <= 2*MOD-2
		// so we do have some leeway
		return sub_mod_raw(sub_mod_raw(lo, hi), MOD-(uint64_t(mi)<<32)+mi);
	}

	static uint64_t reduce_u128_raw(__uint128_t v) {
		uint64_t hi = uint64_t(v >> 64);
		uint64_t lo = uint64_t(v);
		uint32_t hi_hi = uint32_t(hi >> 32);
		uint32_t hi_lo = uint32_t(hi);
		return reduce_u160_raw(lo, hi_lo, hi_hi);
	}

	mod_goldilocks neg() const { return from_reduced(v ? MOD-v : 0); }
	friend mod_goldilocks neg(const mod_goldilocks& m) { return m.neg(); }
	mod_goldilocks operator- () const { return neg(); }

	mod_goldilocks& operator ++ () {
		++ v;
		if (v == MOD) v = 0;
		return *this;
	}
	mod_goldilocks& operator -- () {
		if (v == 0) v = MOD;
		-- v;
		return *this;
	}
	mod_goldilocks& operator += (mod_goldilocks o) {
		v = sub_mod_raw(v, MOD-o.v);
		return *this;
	}
	mod_goldilocks& operator -= (mod_goldilocks o) {
		v = sub_mod_raw(v, o.v);
		return *this;
	}
	mod_goldilocks& operator *= (mod_goldilocks o) {
		v = reduce_u128_raw(__uint128_t(v) * __uint128_t(o.v));
		return *this;
	}

	friend mod_goldilocks operator ++ (mod_goldilocks& a, int) { mod_goldilocks r = a; ++a; return r; }
	friend mod_goldilocks operator -- (mod_goldilocks& a, int) { mod_goldilocks r = a; --a; return r; }
	friend mod_goldilocks operator + (mod_goldilocks a, mod_goldilocks b) { return mod_goldilocks(a) += b; }
	friend mod_goldilocks operator - (mod_goldilocks a, mod_goldilocks b) { return mod_goldilocks(a) -= b; }
	friend mod_goldilocks operator * (mod_goldilocks a, mod_goldilocks b) { return mod_goldilocks(a) *= b; }

	mod_goldilocks inv() const { return from_reduced(mod_inv_in_range(v, MOD)); }
	friend mod_goldilocks inv(mod_goldilocks m) { return m.inv(); }
	mod_goldilocks& operator /= (mod_goldilocks o) {
		return *this *= o.inv();
	}
	friend mod_goldilocks operator / (mod_goldilocks a, mod_goldilocks b) { return mod_goldilocks(a) /= b; }
};

template <typename T> T power(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, M; std::cin >> N >> M;
		using bs = std::bitset<10>;
		std::vector<bs> S(N);
		for (int i = 0; i < N; i++) {
			std::string r; std::cin >> r;
			assert(int(r.size()) == M);
			for (int j = 0; j < M; j++) {
				assert(r[j] == 'O' || r[j] == '.');
				S[i][j] = r[j] == 'O';
			}
		}
		using num = modnum<998244353>;
		std::cout << [&]() -> num {
			auto popcount = [](bs a) -> int {
				return int(a.count());
			};
			std::vector<int> pref(N+1);
			for (int i = 0; i < N; i++) {
				pref[i+1] = (pref[i] + popcount(S[i])) & 1;
			}
			std::vector<std::array<int, 2>> pref_cnt(N+1);
			std::vector<std::array<int, 2>> suff_cnt(N+1);
			for (int i = 0; i <= N; i++) {
				pref_cnt[i] = i > 0 ? pref_cnt[i-1] : std::array<int, 2>{0, 0};
				pref_cnt[i][pref[i]]++;
			}
			for (int i = N; i >= 0; i--) {
				suff_cnt[i] = i < N ? suff_cnt[i+1] : std::array<int, 2>{0, 0};
				suff_cnt[i][pref[i]]++;
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
			}

			if (!(M & 1)) {
				// there's nothing full, so we just fill everything in the easy way
				num ans = 0;
				for (int i = 0; i+1 < K; i++) {
					int a = odds[i], b = odds[i+1] + 1;
					assert((pref[a] & 1) == (pref[b] & 1));
					int side = (pref[a] & 1);
					int cost = (b-a-1) + extra_cost[i];
					//std::cerr << a << "-" << b << ' ' << pref_cnt[a][side] << ' ' << suff_cnt[b][side] << ' ' << cost << '\n';
					ans += num(pref_cnt[a][side]) * num(suff_cnt[b][side]) * num(cost);
				}
				return ans;
			}

			assert(M & 1);

			// Now, it's time to do the funny matching thing
			for (int i = 0; i+1 < K; i++) { std::cerr << extra_cost[i]; } std::cerr << '\n';

			num ans = 0;
			for (const bool side : {0, 1}) {
				std::cerr << '\n' << "doing side " << side << '\n';
				// we're going to find the matching for lo..hi
				struct pt_t {
					bool is_need;
					int loc;
				};
				std::vector<pt_t> pts; pts.reserve(N*M);
				std::vector<int> valid_ends; valid_ends.reserve(N*M + 1);
				valid_ends.push_back(0);
				for (int i = 0; i < N; i++) {
					if (pref[i] == side) {
						valid_ends.back()++;
					}
					int cnt = popcount(S[i]);
					if (cnt == M) {
						// need 1 here
						pts.push_back({true, i});
						valid_ends.push_back(0);
					} else {
						int extras = M - 1 - cnt;
						for (int z = 0; z < extras; z++) {
							pts.push_back({false, i});
							valid_ends.push_back(0);
						}
					}
				}
				if (pref[N] == side) valid_ends.back()++;

				int L = int(pts.size());
				assert(int(valid_ends.size()) == L+1);
				assert(std::accumulate(valid_ends.begin(), valid_ends.end(), 0) == pref_cnt[N][side]);
				std::vector<int> pref_valid_ends(L+2);
				for (int i = 0; i <= L; i++) pref_valid_ends[i+1] = pref_valid_ends[i] + valid_ends[i];

				// greedy match forwards and backwards
				std::vector<int> pt_bal(L+1);
				pt_bal[0] = 0;
				for (int i = 0; i < L; i++) {
					pt_bal[i+1] = pt_bal[i] + (pts[i].is_need ? -1 : +1);
				}

				std::vector<int> is_forced(N+1);
				for (int i = 0; i <= N; i++) {
					is_forced[i] = (pref[i] != side);
				}
				for (int i = 0; i <= L; i++) {
					if (!valid_ends[i]) continue;
				}

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
						for (; k < pts[i].loc; k++) {
							if (pref[k] != side) {
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
								while (!extra_pts.empty() && extra_pts.back() >= pts[j].loc) {
									cost--;
									extra_pts.pop_back();
								}
								prv[i] = {j, cost};
								nxt[j] = {i, cost};
							}
						}
					}
				}

				std::vector<int> chain; chain.reserve(L);
				for (int st = 0; st < L; st++) {
					if (prv[st].dest != -1) continue;
					chain.clear();
					for (int i = st; i != -1; i = nxt[i].dest) {
						chain.push_back(i);
					}
					int C = int(chain.size());
					std::cerr << "have chain " << C << '\n';
					for (int i : chain) std::cerr << pts[i].is_need << '@' << pts[i].loc << ' ';
					std::cerr << '\n';

					std::vector<int> chain_vals(C); // TODO: p sure this should be int
					chain_vals[0] = 0;
					for (int z = 0; z+1 < int(chain.size()); z++) {
						assert(pts[chain[z]].is_need != pts[chain[z+1]].is_need);
						chain_vals[z+1] = chain_vals[z] +  
							(pts[chain[z]].is_need ? -1 : +1) * nxt[chain[z]].cost;
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
				}
			}

			return -1;
		}() << '\n';
	}


	return 0;
}
