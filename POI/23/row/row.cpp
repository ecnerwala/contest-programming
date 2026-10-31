#include <bits/stdc++.h>

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

	static int minv(int a, int m) {
		a %= m;
		assert(a);
		return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
	}

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
		res.v = minv(v, MOD);
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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K, M; cin >> N >> K >> M;
	vector<array<int, 2>> edges(M);
	vector<vector<int>> adj(K);
	for (auto& e : edges) {
		int u, v; cin >> u >> v; u--, v--;
		e = {u, v};
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	array<vector<int>, 2> A;
	for (int z = 0; z < 2; z++) {
		A[z].resize(N);
		for (auto& a : A[z]) { cin >> a; a--; }
	}

	mt19937 mt(48);
	using num = modnum<998244353>;
	vector<num> val(K); for (auto& v : val) { v = num(mt()); }

	array<vector<vector<num>>, 2> cur_vals;
	for (int z = 0; z < 2; z++) {
		cur_vals[z] = vector<vector<num>>(K, vector<num>({num(0)}));
		for (int k : A[z]) {
			cur_vals[z][k].push_back(num(0));
			num v = val[k];
			for (int u : adj[k]) {
				cur_vals[z][u].back() += v;
			}
		}
	}

	cout << (cur_vals[0] == cur_vals[1] ? "TAK" : "NIE") << '\n';

	return 0;
}

// Consider all commands of type 1, and consider commands S which don't commute with it
// Then, the multiset of commands in S between the first and second 1 in the
// first set must equal the multiset of commands in S in the second set.
