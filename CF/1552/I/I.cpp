#include <bits/stdc++.h>

namespace kactl {
using namespace std;
#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
bool dfs(int a, int L, vector<vi>& g, vi& btoa, vi& A, vi& B) {
	if (A[a] != L) return 0;
	A[a] = -1;
	for (int b : g[a]) if (B[b] == L + 1) {
		B[b] = 0;
		if (btoa[b] == -1 || dfs(btoa[b], L + 1, g, btoa, A, B))
			return btoa[b] = a, 1;
	}
	return 0;
}

int hopcroftKarp(vector<vi>& g, vi& btoa) {
	int res = 0;
	vi A(g.size()), B(btoa.size()), cur, next;
	for (;;) {
		fill(all(A), 0);
		fill(all(B), 0);
		/// Find the starting nodes for BFS (i.e. layer 0).
		cur.clear();
		for (int a : btoa) if(a != -1) A[a] = -1;
		rep(a,0,sz(g)) if(A[a] == 0) cur.push_back(a);
		/// Find all layers using bfs.
		for (int lay = 1;; lay++) {
			bool islast = 0;
			next.clear();
			for (int a : cur) for (int b : g[a]) {
				if (btoa[b] == -1) {
					B[b] = lay;
					islast = 1;
				}
				else if (btoa[b] != a && !B[b]) {
					B[b] = lay;
					next.push_back(btoa[b]);
				}
			}
			if (islast) break;
			if (next.empty()) return res;
			for (int a : next) A[a] = lay;
			cur.swap(next);
		}
		/// Use DFS to scan for augmenting paths.
		rep(a,0,sz(g))
			res += dfs(a, 0, g, btoa, A, B);
	}
}
#undef rep
#undef all
#undef sz
}

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
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

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

public:

	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }

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
		v = int(ll(v) * ll(o.v) % MOD);
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


int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using bs = std::bitset<110>;
	using num = modnum<998244353>;
	auto cmp = [&](bs a, bs b) -> bool {
		bs d = a ^ b;
		auto x = d._Find_first();
		if (x == d.size()) return false;
		return b[x];
	};

	int NPEOPLE, M; cin >> NPEOPLE >> M;
	vector<bs> S(M);
	for (auto& s : S) {
		int l; cin >> l;
		for (int i = 0; i < l; i++) {
			int a; cin >> a; a--;
			s[a] = true;
		}
	}
	S.emplace_back();
	for (int i = 0; i < NPEOPLE; i++) S.back()[i] = true;
	sort(S.begin(), S.end(), cmp);
	S.resize(unique(S.begin(), S.end()) - S.begin());
	M = int(S.size());

	vector<int> par(M, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a);
		b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};

	for (int a = 0; a < M; a++) {
		for (int b = 0; b < M; b++) {
			if ((S[a] & S[b]).any() && (S[a] & ~S[b]).any() && (~S[a] & S[b]).any()) {
				merge(a, b);
			}
		}
	}

	auto check_possible = [&](vector<int> cc) -> void {
		vector<array<int, 2>> rng(M, {0,0});
		bs lo0; bs hi0 = S[cc[0]];
		{
			auto x = S[cc[0]]._Find_first();
			lo0[x] = true;
			hi0[x] = false;
		}
		std::y_combinator([&](auto self, int a, array<int, 2> r, bs lo, bs hi) -> void {
			assert(r[1] - r[0] == int(S[a].count()));
			assert(!(lo & hi).any());
			assert((lo | hi) == S[a]);
			if (rng[a] != array<int, 2>{0,0}) {
				if (rng[a] != r) {
					cout << 0 << '\n';
					exit(0);
				}
				return;
			}
			rng[a] = r;
			for (int b : cc) {
				if ((S[a] & S[b]).any() && (S[a] & ~S[b]).any() && (~S[a] & S[b]).any()) {
					bs both = S[a] & S[b];
					bs bonly = ~S[a] & S[b];
					if ((both & lo) == lo) {
						auto nlo = bonly;
						auto nhi = both;
						self(b, array<int, 2>{r[0] - int(nlo.count()), r[0] + int(nhi.count())}, nlo, nhi);
					} else if ((both & hi) == hi) {
						auto nlo = both;
						auto nhi = bonly;
						self(b, array<int, 2>{r[1] - int(nlo.count()), r[1] + int(nhi.count())}, nlo, nhi);
					} else {
						std::cout << 0 << '\n';
						exit(0);
					}
				}
			}
		})(cc[0], array<int, 2>{0, int(S[cc[0]].count())}, lo0, hi0);
		int mi = 0;
		int ma = 0;
		for (int a : cc) {
			assert((rng[a] != array<int, 2>{0,0}));
			mi = min(mi, rng[a][0]);
			ma = max(ma, rng[a][1]);
		}
		for (int a : cc) {
			rng[a][0] -= mi;
			rng[a][1] -= mi;
		}
		ma -= mi;
		mi = 0;

		bs all_pts;
		for (int a : cc) all_pts |= S[a];

		if (ma != int(all_pts.count())) {
			std::cout << 0 << '\n';
			exit(0);
		}

		vector<bs> msks(ma, all_pts);
		for (int a : cc) {
			for (int i = 0; i < ma; i++) {
				if (rng[a][0] <= i && i < rng[a][1]) {
					msks[i] &= S[a];
				} else {
					msks[i] &= ~S[a];
				}
			}
		}
		vector<vector<int>> g(ma);
		vector<int> btoa(NPEOPLE, -1);
		for (int i = 0; i < ma; i++) {
			for (int p = 0; p < NPEOPLE; p++) {
				if (msks[i][p]) {
					g[i].push_back(p);
				}
			}
		}
		int flow = kactl::hopcroftKarp(g, btoa);
		if (flow != ma) {
			cout << 0 << '\n';
			exit(0);
		}
	};

	vector<bs> all_msk(M);
	for (int i = 0; i < M; i++) {
		all_msk[get_par(i)] |= S[i];
	}

	vector<tuple<int, bool, int>> to_process;
	for (int i = 0; i < M; i++) {
		if (get_par(i) == i) {
			to_process.push_back({int(all_msk[i].count()), S[i] == all_msk[i], i});
		}
	}
	sort(to_process.begin(), to_process.end());

	vector<bool> alive(NPEOPLE, true);

	num ans = 1;
	vector<bs> see_msks(NPEOPLE);
	for (auto [_, _x, a] : to_process) {
		vector<int> cur_cc;
		for (int i = 0; i < M; i++) {
			if (get_par(i) == a) {
				cur_cc.push_back(i);
			}
		}
		check_possible(cur_cc);
		vector<bs> children;
		for (int i = 0; i < NPEOPLE; i++) {
			if (alive[i] && all_msk[a][i]) {
				if (!children.empty()) alive[i] = false;

				bs see_msk;
				for (int b : cur_cc) {
					if (S[b][i]) see_msk[b] = true;
				}
				children.push_back(see_msk);
			}
		}
		assert(!children.empty());
		sort(children.begin(), children.end(), cmp);
		int cur_run = 0;
		int num_runs = 0;
		for (int z = 0; z < int(children.size()); z++) {
			if (z == 0 || children[z] != children[z-1]) {
				cur_run = 0;
				num_runs++;
			}
			cur_run++;
			ans *= num(cur_run);
		}
		assert((num_runs >= 2) == (int(cur_cc.size()) >= 2));
		if (num_runs >= 2) ans *= num(2);
	}
	cout << ans << '\n';

	return 0;
}
