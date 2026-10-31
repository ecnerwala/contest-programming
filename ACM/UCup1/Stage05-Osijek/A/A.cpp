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
	std::vector<int> V(N);
	for (auto& v : V) cin >> v;

	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	using mnum = modnum<998244353>;
	std::array<mnum, 3> tot_ans;
	mnum single_ans = 0;
	for (int i = 0; i < N; i++) single_ans += mnum(V[i]) * mnum(V[i]);
	tot_ans.fill(single_ans);

	std::array<int64_t, 50> precomp;
	precomp.fill(0);
	std::y_combinator([&](auto self, int cur, int prv) -> std::array<int, 50> {
		std::array<int, 50> v_cur;
		for (int a = 0; a < 25; a++) {
			int v = (V[cur] >> a) & 1;
			v_cur[2*a+0] = v == 0;
			v_cur[2*a+1] = (v == 0) - (v == 1);
		}

		std::array<int, 50> dp_cur = v_cur;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			auto dp_nxt = self(nxt, cur);
			for (int i = 0; i < int(v_cur.size()); i++) {
				precomp[i] += int64_t(dp_cur[i]) * int64_t(dp_nxt[i]);
				dp_cur[i] += v_cur[i] * dp_nxt[i];
			}
		}
		return dp_cur;
	})(0, -1);

	mnum all_pairs = mnum(N) * mnum(N-1) / mnum(2);

	for (int t = 0; t < 3; t++) {
		const int SZ = 25 * 26 / 2;
		std::array<int64_t, SZ> cur_ans;
		cur_ans.fill(0);
		std::y_combinator([&](auto self, int cur, int prv) -> std::array<int, SZ> {
			std::array<int8_t, SZ> v_cur;
			int idx = 0;
			for (int a = 0; a < 25; a++) {
				for (int b = a; b < 25; b++) {
					int v = ((V[cur] >> a) & 1) * 2 + ((V[cur] >> b) & 1);
					v_cur[idx++] = (t == 0) ? (v == 3) : (t == 1) ? (v == 0) : (v == 0 || v == 3) ? 1 : -1;
				}
			}
			std::array<int, SZ> dp_cur;
			for (int i = 0; i < SZ; i++) dp_cur[i] = v_cur[i];
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				auto dp_nxt = self(nxt, cur);
				for (int i = 0; i < int(v_cur.size()); i++) {
					cur_ans[i] += int64_t(dp_cur[i]) * int64_t(dp_nxt[i]);
					dp_cur[i] += v_cur[i] * dp_nxt[i];
				}
			}
			return dp_cur;
		})(0, -1);

		int idx = 0;
		for (int a = 0; a < 25; a++) {
			for (int b = a; b < 25; b++) {
				mnum w = mnum(1 << a) * mnum(1 << b) * mnum(a == b ? 1 : 2);
				if (t == 0) {
					tot_ans[0] += w * mnum(2) * cur_ans[idx++];
				} else if (t == 1) {
					tot_ans[1] += w * mnum(2) * (cur_ans[idx++] - precomp[2*a+0] - precomp[2*b+0] + all_pairs);
				} else if (t == 2) {
					tot_ans[2] += w * inv(mnum(2)) * (all_pairs - precomp[2*a+1] - precomp[2*b+1] + cur_ans[idx++]);
				} else assert(false);
			}
		}
	}

	cout << tot_ans[0] << ' ' << tot_ans[1] << ' ' << tot_ans[2] << '\n';

	return 0;
}
