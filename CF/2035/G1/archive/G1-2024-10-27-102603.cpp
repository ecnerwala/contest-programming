#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

struct splitmix64_hash {
	static uint64_t splitmix64(uint64_t x) {
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(uint64_t x) const {
		static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
};

template <typename K, typename V, typename Hash = splitmix64_hash>
using hash_map = __gnu_pbds::gp_hash_table<K, V, Hash>;

template <typename K, typename Hash = splitmix64_hash>
using hash_set = hash_map<K, __gnu_pbds::null_type, Hash>;

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

using std::cerr;
using num = modnum<998244353>;
using lin_fn = pairnum<num, num>;

std::pair<int, num> solve(std::vector<int> cnd) {
	int N = int(cnd.size());
	std::vector<int> check_time(N-1);
	std::vector<int> nxt_smaller(N-1);
	std::vector<int> prv_smaller(N-1);
	[&](this auto&& self, int l, int h, int d) -> void {
		if (l == h) return;
		int m = (l + h) / 2;
		check_time[m] = d;
		prv_smaller[m] = l-1;
		nxt_smaller[m] = h;
		self(l, m, d+1);
		self(m+1, h, d+1);
	}(0, N-1, 0);

	num invN = inv(num(N));

	num ans = 1;
	std::vector<int> lcp_dp; lcp_dp.reserve(N);
	struct seq_t {
		lin_fn tot_ways{0, 0};
		int st = 0;
		std::vector<std::pair<int, lin_fn>> decr;
	};
	std::vector<hash_map<int, seq_t>> mp(N-1);
	for (int x = 0; x < N; x++) {
		if (cnd[x] != -1) {
			int k = cnd[x];
			if (k == 0 && x > 0) continue;
			//cerr << "doing " << x << ' ' << k << '\n';
			int cur_lcp;
			{
				int mi = -1;
				int ma = int(lcp_dp.size());
				while (ma - mi > 1) {
					int md = (mi + ma) / 2;
					if (k > lcp_dp[md]) {
						mi = md;
					} else {
						ma = md;
					}
				}
				if (ma == int(lcp_dp.size())) {
					ans = 0;
					lcp_dp.push_back(k);
				} else {
					lcp_dp[ma] = k;
				}
				cur_lcp = ma;
			}
			num ways = 0;
			{
				num cur_w = 1;
				num p_smaller = num(k) * invN;
				for (int node = x-1; true; cur_w *= p_smaller, node = prv_smaller[node]) {
					if (node == -1) {
						if (cur_lcp == 0) {
							// one way in
							ways += cur_w;
						}
						break;
					} else {
						auto it = mp[node].find(cur_lcp-1);
						if (it != mp[node].end()) {
							auto& seq = it->second;
							for (; seq.st < int(seq.decr.size()) && seq.decr[seq.st].first > k; seq.st++) {
								seq.tot_ways -= seq.decr[seq.st].second;
							}
							ways += cur_w * (seq.tot_ways.u + num(k) * seq.tot_ways.v);
							//cerr  <<  "  " << "prv " << node << ' ' << cur_w << ' ' << pow(num(N), node+1) * (seq.tot_ways.u + num(k) * seq.tot_ways.v) << '\n';
						}
					}
				}
			}
			{
				std::vector<int> to_merge;
				for (int node = x-1; node != -1 && nxt_smaller[node] == x; node = prv_smaller[node]) {
					to_merge.push_back(node);
				}
				int node = x;
				while (!to_merge.empty()) {
					int old_node = to_merge.back(); to_merge.pop_back();
					for (auto& [lcp_val, old_seq] : mp[old_node]) {
						if (old_seq.st < int(old_seq.decr.size())) {
							if (node == N-1) {
								if (lcp_val == int(lcp_dp.size()) - 1) {
									for (int idx = old_seq.st; idx < int(old_seq.decr.size()); idx++) {
										auto [cur_k, f] = old_seq.decr[idx];
										num cur_w = (cur_k == cnd[old_node] ? f.u : f.v * num(N-cur_k));
										ans += cur_w;
									}
								}
							} else {
								auto& seq = mp[node][cur_lcp];
								for (int idx = old_seq.st; idx < int(old_seq.decr.size()); idx++) {
									auto [cur_k, f] = old_seq.decr[idx];
									num cur_w = (cur_k == cnd[old_node] ? f.u : f.v * num(N-cur_k));
									lin_fn cur_fn;
									cur_fn = {-cur_w * num(k) * invN, cur_w * invN};
									seq.tot_ways += cur_fn;
									seq.decr.push_back({k, cur_fn});
								}
							}
						}
						old_seq = {};
					}
					mp[old_node] = {};
				}
			}
			//cerr << "  " << "lcp" << ' ' << cur_lcp << '\n';
			//cerr << "  " << ways * pow(num(N), x) << '\n';
			{
				num cur_w = ways * invN;
				num p_bigger = num(N-k) * invN;
				for (int node = x; true; cur_w *= (node == x ? 1 : p_bigger), node = nxt_smaller[node]) {
					if (node == N-1) {
						if (cur_lcp == int(lcp_dp.size()) - 1) {
							//cerr << "adding ans " << cur_w * pow(num(N), N) << '\n';
							ans += cur_w;
						}
						break;
					} else {
						auto& seq = mp[node][cur_lcp];
						assert(seq.st == int(seq.decr.size()) || k < seq.decr.back().first);
						lin_fn cur_fn;
						if (node == x) {
							cur_fn = {cur_w, 0};
						} else {
							cur_fn = {-cur_w * num(k) * invN, cur_w * invN};
						}
						//cerr << "  insert " << node << ' ' << -pow(num(N), node+1) * cur_fn.u << ' ' << pow(num(N), node+1) * cur_fn.v << '\n';
						seq.tot_ways += cur_fn;
						seq.decr.push_back({k, cur_fn});
						break;
					}
				}
			}
		}
	}
	return {int(lcp_dp.size()), ans * pow(num(N), N)};
}
std::pair<int, num> solve_slow(std::vector<int> cnd) {
	int N = int(cnd.size());
	std::vector<int> A(N);
	int best_score = 0;
	int ways = 0;
	[&](this auto&& self, int idx) -> void {
		if (idx == N) {
			int score = 0;
			for (int k = 0; k < N; k++) {
				int l = 0, h = N-1;
				while (l < h) {
					int m = (l + h) / 2;
					if (A[m] < k) {
						l = m+1;
					} else {
						h = m;
					}
				}
				int x = l;
				if (A[x] == k && cnd[x] == k) {
					score++;
				}
			}
			if (score > best_score) {
				best_score = score;
				ways = 0;
			}
			if (score == best_score) ways++;
			return;
		}
		for (A[idx] = 0; A[idx] < N; A[idx]++) self(idx+1);
	}(0);
	return {best_score, ways};
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		// we want to maximize the # of correct tests
		std::vector<int> cnd(N, -1);
		for (int e = 0; e < M; e++) {
			int x, k; cin >> x >> k; x--, k--;
			assert(cnd[x] == -1);
			cnd[x] = k;
		}
		auto [lcp, ways] = solve(cnd);
		//auto [lcp, ways] = solve_slow(cnd);
		cout << M - lcp << ' ' << ways << '\n';
	}

	return 0;
}
