#include <bits/stdc++.h>

template <int MOD_> struct modnum {
	static const int MOD = MOD_;
private:
	int v;
public:
	modnum() : v(0) {}
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& o, modnum m) { return o << int(m); }
	friend std::istream& operator >> (std::istream& i, modnum& m) { int64_t n; i >> n; m = modnum(n); return i; }

	friend bool operator == (modnum a, modnum b) { return a.v == b.v; }
	friend bool operator != (modnum a, modnum b) { return a.v != b.v; }

	modnum operator + () const { return *this; }
	modnum operator - () const { modnum r; r.v = v ? MOD - v : 0; return r; }
	friend modnum neg(modnum a) {
		return -a;
	}
private:
	static int minv(int a, int m) {
		return a == 1 ? 1 : int(m - int64_t(minv(m % a, a)) * m / a);
	}
public:
	friend modnum inv(modnum a) {
		assert(a.v);
		modnum r;
		r.v = minv(a.v, MOD);
		return r;
	}

	modnum& operator ++ () {
		v++;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum& operator -- () {
		if (!v) v = MOD;
		v--;
		return *this;
	}
	modnum operator ++ (int) { modnum r = *this; operator ++ (); return r; }
	modnum operator -- (int) { modnum r = *this; operator -- (); return r; }

	modnum& operator += (modnum o) {
		v -= MOD - o.v;
		v = v < 0 ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (modnum o) {
		v -= o.v;
		v = v < 0 ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (modnum o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (modnum o) {
		return *this *= inv(o);
	}

	friend modnum operator + (modnum a, modnum b) { return a += b; }
	friend modnum operator - (modnum a, modnum b) { return a -= b; }
	friend modnum operator * (modnum a, modnum b) { return a *= b; }
	friend modnum operator / (modnum a, modnum b) { return a /= b; }
};

template <typename T> T pow(T a, int64_t b) {
	T r = 1;
	for (; b; b >>= 1, a *= a) if (b & 1) r *= a;
	return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using num = modnum<int(1e9)+7>;

	int T; cin >> T; while (T--) {
		int N, K; cin >> N >> K;
		string type; cin >> type;
		vector<string> adj(N); for (auto& r : adj) cin >> r;

		// If we just match up the inedges and outedges of every vertex, we
		// will always get a collection of paths + cycles.
		// Each cycle has at least one backedge; let them be e1 and e2
		// We have several bad cases:
		//   1. e1 is in a cycle and e2 in a path
		//   2. e2 is in a cycle and e1 in a path
		//   3. e1 and e2 are in different cycles
		//   4. e1 and e2 are in the same cycle
		// We can count 1+3 easily (that's paths from e1.dest to e1.source using forwards edges)
		// We can count 2+3 easily (that's paths from e1.dest to e1.source using forwards edges)
		// We also need to count 3 and add it back in, and count 4
		// To count 3/4, we use an n^3 dp.

		const int V = N+1;
		vector<num> fact(V+1);
		fact[0] = 1;
		for (int i = 1; i <= V; i++) fact[i] = fact[i-1] * num(i);
		vector<num> ifact(V+1);
		ifact[V] = inv(fact[V]);
		for (int i = V; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

		vector<num> recip(V+1);
		for (int i = 1; i <= V; i++) recip[i] = ifact[i] * fact[i-1];

		vector<int> deg(N);
		for (int i = 0; i < N; i++) {
			deg[i] += type[i] == 'R';
			for (char c : adj[i]) deg[i] += c == '1';
		}

		num tot_ways = 1;
		for (int d : deg) tot_ways *= fact[d];

		auto get_path_1 = [&](int st, int en) -> num {
			if (st > en) return 0;
			// we want product of recip[deg[i]] over the used vertices
			vector<num> dp(N);
			dp[st] = tot_ways;
			for (int i = st; i <= en; i++) {
				dp[i] *= recip[deg[i]];
				for (int j = i+1; j <= en; j++) {
					if (adj[i][j] == '1') {
						dp[j] += dp[i];
					}
				}
			}
			return dp[en];
		};
		auto get_path_2 = [&](int st1, int st2, int en1, int en2) -> num {
			return get_path_1(st1, en1) * get_path_1(st2, en2) / tot_ways;
		};

		vector<array<int, 2>> back_edges; back_edges.reserve(2);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < i; j++) {
				if (adj[i][j] == '1') back_edges.push_back({i,j});
			}
		}
		assert(int(back_edges.size()) <= 2);

		num ans = tot_ways;
		for (auto e : back_edges) {
			ans -= get_path_1(e[1], e[0]);
		}
		if (int(back_edges.size()) == 2) {
			auto e1 = back_edges[0];
			auto e2 = back_edges[1];
			ans += get_path_2(e1[1], e2[1], e1[0], e2[0]);
			ans -= get_path_2(e1[1], e2[1], e2[0], e1[0]);
		}

		cout << ans << '\n';
	}

	return 0;
}
