#include <bits/stdc++.h>

/**
 * manacher(S): return the maximum palindromic substring of S centered at each point
 *
 * Input: string (or vector) of length N (no restrictions on character-set)
 * Output: vector res of length 2*N+1
 *   For any 0 <= i <= 2*N:
 *   * i % 2 == res[i] % 2
 *   * the half-open substring S[(i-res[i])/2, (i+res[i])/2) is a palindrome of length res[i]
 *   * For odd palindromes, take odd i, and vice versa
 */
template <typename V> std::vector<int> manacher(const V& S) {
	int N = int(S.size());
	std::vector<int> res(2*N+1, 0);
	for (int i = 1, j = -1, r = 0; i < 2*N; i++, j--) {
		if (i > r) {
			r = i+1, res[i] = 1;
		} else {
			res[i] = res[j];
		}
		if (i+res[i] >= r) {
			int b = r>>1, a = i-b;
			while (a > 0 && b < N && S[a-1] == S[b]) {
				a--, b++;
			}
			res[i] = b-a, j = i, r = b<<1;
		}
	}
	return res;
}

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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		//cerr << "Start Case" << '\n';
		using num = modnum<998244353>;

		string S; cin >> S;
		int N = int(S.size());

		vector<vector<int>> edges(N);
		vector<int> cc_id(N, -1);
		vector<num> cc_weight(N, 0);
		vector<int> max_pal(2*N+1, 0);
		for (int i = 1, j = -1, r = 0; i < 2*N; i++, j--) {
			if (i > r) {
				r = i+1, max_pal[i] = 1;
				assert(i&1);
				int b = i>>1;
				cc_id[b] = b;
				cc_weight[b] = 1;
			} else {
				max_pal[i] = max_pal[j];
			}
			if (i+max_pal[i]>=r) {
				int b = r>>1, a = i-b;
				while (a > 0 && b < N && S[a-1] == S[b]) {
					assert(cc_id[a-1] != -1);
					cc_id[b] = cc_id[a-1];
					cc_weight[cc_id[b]] = 2 * cc_weight[cc_id[b]] + 1;
					edges[b] = {};
					a--, b++;
				}
				if (a > 0 && b < N) edges[b].push_back(cc_id[a-1]);
				max_pal[i] = b-a, j = i, r = b<<1;
			}
		}

		vector<num> dp(N, num(1));
		vector<num> dp_ch(N, num(1));
		for (int i = N-1; i >= 0; i--) {
			if (cc_id[i] != i) continue;
			dp_ch[i] *= cc_weight[i];
			dp_ch[i] ++;
			dp[i] *= dp_ch[i];
			dp_ch[i] = inv(dp_ch[i]);

			sort(edges[i].begin(), edges[i].end());
			edges[i].resize(unique(edges[i].begin(), edges[i].end()) - edges[i].begin());

			for (int j : edges[i]) {
				dp_ch[j] *= dp_ch[i];
			}
			if (!edges[i].empty()) {
				int j = edges[i].back();
				dp[j] *= dp[i];
			}
		}
		cout << dp[0] << '\n';
	}

	return 0;
}
