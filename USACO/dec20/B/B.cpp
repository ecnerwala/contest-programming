#include <bits/stdc++.h>

template <int MOD_> struct modnum {
	static const int MOD = MOD_;
	static_assert(MOD > 0, "MOD must be positive");
private:
	int v;
public:
	modnum() : v(0) {}
	modnum(std::int64_t v_) : v(int(v_ % MOD)) {}

	explicit operator int() const { return v; }

	friend std::ostream& operator << (std::ostream& o, const modnum& m) {
		return o << int(m);
	}
	friend std::istream& operator >> (std::istream& i, modnum& m) {
		std::int64_t val;
		i >> val;
		m = modnum(val);
		return i;
	}

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum& operator ++ () {
		++v;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum operator ++ (int) {
		modnum r = *this;
		++*this;
		return r;
	}
	modnum& operator -- () {
		if (v == 0) v = MOD;
		--v;
		return *this;
	}
	modnum operator -- (int) {
		modnum r = *this;
		--*this;
		return r;
	}

	modnum neg() const {
		modnum r = *this;
		if (v == 0) r.v = 0;
		else r.v = MOD - v;
		return r;
	}

private:
	static int minv(int a, int m) {
		if (a <= 1) return a;
		return m - int(std::int64_t(minv(m % a, a)) * m / a);
	}
public:

	modnum inv() const {
		assert(v);
		modnum r;
		r.v = minv(v, MOD);
		return r;
	}

	modnum& operator += (const modnum& o) {
		v -= MOD - o.v;
		v = (v < 0 ? v + MOD : v);
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		v = (v < 0 ? v + MOD : v);
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(std::int64_t(v) * std::int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (const modnum& o) {
		return *this *= inv(o);
	}
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

template <typename num> struct matrix {
	int N;
	std::vector<num> v;

	matrix() : matrix(0) {}
	explicit matrix(int N_, num val = num()) : N(N_), v(N * N) {
		for (int i = 0; i < N; i++) {
			(*this)[{i,i}] = val;
		}
	}

	num& operator [] (std::array<int, 2> c) {
		assert(0 <= c[0] && c[0] < N);
		assert(0 <= c[1] && c[1] < N);
		return v[c[0] * N + c[1]];
	}
	const num& operator [] (std::array<int, 2> c) const {
		assert(0 <= c[0] && c[0] < N);
		assert(0 <= c[1] && c[1] < N);
		return v[c[0] * N + c[1]];
	}

	friend matrix operator * (const matrix& a, const matrix& b) {
		assert(a.N == b.N);
		int N = a.N; // Silly clang, there's no shadowing because it's a friend
		matrix r(N);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				for (int k = 0; k < N; k++) {
					r[{i,j}] += a[{i,k}] * b[{k,j}];
				}
			}
		}
		return r;
	}

	matrix& operator *= (const matrix& o) {
		return *this = *this * o;
	}

	matrix& operator += (const matrix& o) {
		assert(N == o.N);
		for (int i = 0; i < N*N; i++) {
			v[i] += o.v[i];
		}
		return *this;
	}
	matrix& operator -= (const matrix& o) {
		assert(N == o.N);
		for (int i = 0; i < N*N; i++) {
			v[i] -= o.v[i];
		}
		return *this;
	}

	friend matrix operator + (const matrix& a, const matrix& b) { return matrix(a) += b; }
	friend matrix operator - (const matrix& a, const matrix& b) { return matrix(a) -= b; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using num = modnum<int(1e9)+7>;
	using mat = matrix<num>;

	int N, K, Q; cin >> N >> K >> Q;

	mat adj(N);
	for (int i = 0; i < N; i++) {
		string s; cin >> s;
		for (int j = 0; j < N; j++) {
			char c = s[j];
			assert(c == '0' || c == '1');
			adj[{i,j}] = c - '0';
		}
	}

	const mat ID = mat(N, num(1));

	// We're going to precompute number of ways to take a nonempty set of steps from i to j using any buttons < k, not including the start or the end
	std::vector<mat> num_ways(K);
	// just the identity
	num_ways[0] = adj;
	for (int i = 0; i < K-1; i++) {
		// if you ever press the button in the middle, you have 2 paths other than it
		num_ways[i+1] = num_ways[i] + (num_ways[i] * num_ways[i]);
	}

	for (int q = 0; q < Q; q++) {
		int bs, u, bt, v; cin >> bs >> u >> bt >> v; bs--, u--, bt--, v--;
		num ans = 0;
		std::vector<num> LHS(N);
		LHS[u] = 1;
		std::vector<num> RHS(N);
		RHS[v] = 1;
		bool bs_changed = false;
		bool bt_changed = false;
		for (int mid = max(bs, bt); mid < K; mid++) {
			while (bs < mid) {
				std::vector<num> nxt(N);
				for (int i = 0; i < N; i++) {
					for (int j = 0; j < N; j++) {
						nxt[j] += LHS[i] * num_ways[bs][{i,j}];
					}
				}
				if (!bs_changed) LHS[u] = 0;
				for (int i = 0; i < N; i++) {
					LHS[i] += nxt[i];
				}
				bs++;
				bs_changed = true;
			}
			while (bt < mid) {
				std::vector<num> nxt(N);
				for (int i = 0; i < N; i++) {
					for (int j = 0; j < N; j++) {
						nxt[i] += num_ways[bt][{i,j}] * RHS[j];
					}
				}
				if (!bt_changed) RHS[v] = 0;
				for (int i = 0; i < N; i++) {
					RHS[i] += nxt[i];
				}
				bt++;
				bt_changed = true;
			}
			for (int i = 0; i < N; i++) {
				ans += LHS[i] * RHS[i];
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
