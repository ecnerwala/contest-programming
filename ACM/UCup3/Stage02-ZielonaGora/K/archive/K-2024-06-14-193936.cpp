#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

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

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// coeff of a in x and y
	T vx = 1, vy = 0;
	while (x) {
		T k = y / x;
		y %= x;
		vy -= k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
	}
	assert(y == 1);
	return vy < 0 ? m + vy : vy;
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
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	int v;

public:

	modnum() : v(0) {}
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = mod_inv_in_range(v, MOD);
		return res;
	}
	friend modnum inv(const modnum& m) { return m.inv(); }
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
	friend modnum neg(const modnum& m) { return m.neg(); }

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
	modnum& operator += (const modnum& o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (const modnum& o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N); for (auto& a : A) cin >> a;

	const int L = 20;
	const int S = (1 << L);

	std::mt19937_64 mt(48);
	std::vector<uint64_t> vals(S);
	std::vector<uint64_t> vals_pref(S+1);
	for (int i = 0; i < S; i++) {
		vals[i] = mt();
		assert(vals[i] != 0);
		vals_pref[i+1] = vals_pref[i] ^ vals[i];
	}

	struct seg_node {
		std::array<int, 2> ch{-1, -1};
		uint64_t v = 0;
	};

	std::vector<seg_node> nodes; nodes.reserve(2*S + 4 * N * L);
	nodes.resize(2*S);
	for (int i = 1; i < S; i++) {
		nodes[i].ch = {2*i+0, 2*i+1};
	}

	auto incr = std::y_combinator([&](auto self, int n, int lo, int hi, int k) -> std::pair<int, bool> {
		assert(lo <= k && k < hi);
		int r = int(nodes.size());
		nodes.push_back(nodes[n]);
		bool carry;
		if (hi - lo == 1) {
			assert(nodes[r].v == 0 || nodes[r].v == vals[k]);
			carry = (nodes[r].v != 0);
			nodes[r].v ^= vals[k];
		} else {
			auto [c0, c1] = nodes[r].ch;
			int md = (lo + hi) / 2;
			if (k < md) {
				std::tie(c0, carry) = self(c0, lo, md, k);
				if (carry) {
					std::tie(c1, carry) = self(c1, md, hi, md);
				}
			} else {
				std::tie(c1, carry) = self(c1, md, hi, k);
			}
			nodes[r].ch = {c0, c1};
			nodes[r].v = nodes[c0].v ^ nodes[c1].v;
		}
		return {r, carry};
	});

	std::vector<int> roots(N+1);
	roots[0] = 1;
	for (int i = 0; i < N; i++) {
		bool carry;
		std::tie(roots[i+1], carry) = incr(roots[i], 0, S, A[i]);
		assert(!carry);
	}

	// Dir 0 means prefix, dir 1 means suffix
	auto get_match = [&](int n0, int n1, int dir) -> std::pair<int, uint64_t> {
		assert(nodes[n0].v != nodes[n1].v);
		int lo = 0, hi = S;
		int len = 0; uint64_t val = 0;
		while (hi - lo > 1) {
			assert(nodes[n0].v != nodes[n1].v);
			int md = (lo + hi) / 2;
			assert(hi - lo > 1);
			bool z;
			if (nodes[nodes[n0].ch[dir]].v != nodes[nodes[n1].ch[dir]].v) {
				z = dir;
			} else {
				assert(md - lo == hi - md);
				len += (dir == 0) ? (md - lo) : (hi - md);
				val ^= nodes[nodes[n0].ch[dir]].v;
				z = !dir;
			}
			n0 = nodes[n0].ch[z];
			n1 = nodes[n1].ch[z];
			if (z == 0) hi = md;
			else lo = md;
		}
		return {len, val};
	};
	auto get_diff = [&](int n0, int n1) -> std::pair<int, bool> {
		assert(nodes[n0].v != nodes[n1].v);

		auto [pref_len, pref_val] = get_match(n0, n1, 0);
		auto [suff_len, suff_val] = get_match(n0, n1, 1);

		assert(pref_len + suff_len < S);
		uint64_t m0 = nodes[n0].v ^ pref_val ^ suff_val;
		uint64_t m1 = nodes[n1].v ^ pref_val ^ suff_val;

		int lo = pref_len, hi = S - suff_len;

		bool is_plus_one = m1 == vals[hi-1] && m0 == (vals_pref[hi-1] ^ vals_pref[lo]);

		return {pref_len, is_plus_one};
	};

	std::vector<std::array<int, 2>> edges;

	std::vector<int> nxt(N+1);
	for (int i = 0; i <= N; i++) {
		nxt[i] = i+1;
	}

	std::vector<int> cur_trans(N+1);
	for (int i = 0; i < N; i++) {
		cur_trans[i] = A[i];
		edges.push_back({i, i+1});
	}

	struct interval_t {
		int first, last;
	};
	std::vector<int> comp(N+1, 0);
	int num_comps = 1;
	min_pq<std::pair<int, std::array<int, 2>>> start_pq;
	for (int i = 0; i < N; i++) {
		start_pq.push({cur_trans[i], {i, nxt[i]}});
	}
	interval_t start_interval{0, N};
	std::y_combinator([&](auto self, int cur_comp, min_pq<std::pair<int, std::array<int, 2>>>& pq, interval_t inter) -> void {
		if (inter.first == inter.last) {
			return;
		}
		std::array<std::vector<interval_t>, 2> inters;
		{
			assert(!pq.empty());
			int cur_val = -1;
			std::vector<interval_t> nxt_inters;
			nxt_inters.push_back({inter.first, -1});
			while (true) {
				auto [v, verts] = pq.top();
				auto [i, j] = verts;
				if (comp[i] != cur_comp || comp[j] != cur_comp) {
					pq.pop();
					continue;
				}
				if (cur_val == -1) cur_val = v;
				assert(cur_val != -1);
				assert(v >= cur_val);
				if (v > cur_val) break;
				pq.pop();
				nxt_inters.back().last = i;
				nxt_inters.push_back({j, -1});
			}
			assert(cur_val != -1);
			nxt_inters.back().last = inter.last;

			assert(nxt_inters.size() >= 2);

			inters[0].reserve(int(nxt_inters.size() + 1) / 2);
			inters[1].reserve(int(nxt_inters.size()) / 2);
			// iterate through the intervals
			for (int i = 0; i < int(nxt_inters.size()); i++) {
				inters[i%2].push_back(nxt_inters[i]);
			}
		}
		assert(!inters[0].empty());
		assert(!inters[1].empty());
		std::array<std::vector<int>, 2> pts;
		std::array<int, 2> cur_idx{0, 0};
		std::array<int, 2> cur_val{inters[0][0].first, inters[1][0].first};
		while (true) {
			// advance inters[0], then swap the two
			pts[0].push_back(cur_val[0]);
			if (cur_val[0] == inters[0][cur_idx[0]].last) {
				cur_idx[0]++;
				if (cur_idx[0] == int(inters[0].size())) {
					break;
				}
				cur_val[0] = inters[0][cur_idx[0]].first;
			} else {
				cur_val[0] = nxt[cur_val[0]];
			}
			std::swap(inters[0], inters[1]);
			std::swap(pts[0], pts[1]);
			std::swap(cur_idx[0], cur_idx[1]);
			std::swap(cur_val[0], cur_val[1]);
		}
		// Now, pts[0] is small and complete, while pts[1] is big
		{
			int nxt_comp = num_comps++;
			for (auto p : pts[0]) {
				comp[p] = nxt_comp;
			}
			// update nxt pointers
			for (int z = 0; z < 2; z++) {
				for (int a = 0; a+1 < int(inters[z].size()); a++) {
					int i = inters[z][a].last;
					int j = inters[z][a+1].first;
					auto [v, is_plus_one] = get_diff(i, j);
					if (is_plus_one) {
						edges.push_back({i, j});
					}
					nxt[i] = j;
					cur_trans[i] = v;
					if (z == 1) {
						pq.push({cur_trans[i], {i, j}});
					}
				}
				nxt[inters[z].back().last] = -1;
			}
			min_pq<std::pair<int, std::array<int, 2>>> nxt_pq;
			for (int a = 0; a+1 < int(pts[0].size()); a++) {
				int i = pts[0][a];
				int j = pts[0][a+1];
				assert(nxt[i] == j);
				nxt_pq.push({cur_trans[i], {i, j}});
			}
			self(nxt_comp, nxt_pq, interval_t{inters[0].front().first, inters[0].back().last});
			self(cur_comp, pq, interval_t{inters[1].front().first, inters[1].back().last});
		}
	})(0, start_pq, start_interval);

	using num = modnum<int(1e9)+7>;
	std::vector<num> dp(N+1);
	dp[0] = 1;
	std::sort(edges.begin(), edges.end());
	for (auto [i, j] : edges) {
		dp[j] += dp[i];
	}
	cout << dp[N] << '\n';

	return 0;
}
