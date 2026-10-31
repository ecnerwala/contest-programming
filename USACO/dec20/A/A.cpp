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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> S(N); for (auto& v : S) cin >> v;
	vector<int> T(N); for (auto& v : T) cin >> v;
	sort(S.begin(), S.end());
	sort(T.begin(), T.end());
	vector<bool> V; V.reserve(2*N);
	for (int i = 0, j = 0; i < N || j < N; ) {
		if (i < N && (j == N || S[i] <= T[j])) {
			V.push_back(false);
			i++;
		} else {
			V.push_back(true);
			j++;
		}
	}

	// Now, we have this sequence of '(' and ')', and we want to find a maximal matching
	// Fix the first unmatched (
	// Then,
	//   all open parens before must get matched;
	//   all close parens after must get matched;
	//   all close parens before are allowed to be unmatched;
	//   all open parens after are allowed to be unmatched;
	// We can DP on the prefix and on the suffix:
	//   for the prefix, state is # of (currently) unmatched open parens
	//   for the suffix, state is # of (currently) unmatched close parens

	using num = modnum<int(1e9)+7>;
	vector<vector<num>> dp_forwards;
	vector<vector<num>> dp_reverse;
	for (int z = 0; z < 2; z++) {
		vector<vector<num>> dp(2*N+1);
		dp[0] = vector<num>({num(1)});
		for (int i = 0; i < 2*N; i++) {
			if (V[i]) {
				// this is a close; we're free to skip it, or match against one of the open ones
				dp[i+1] = dp[i];
				for (int v = 1; v < int(dp[i].size()); v++) {
					dp[i+1][v-1] += dp[i][v] * num(v);
				}
			} else {
				// this is an open; we have to track it, because it must eventually get matched
				// dp[i+1] = [0] + dp[i]
				dp[i+1] = vector<num>({num(0)});
				dp[i+1].insert(dp[i+1].end(), dp[i].begin(), dp[i].end());
			}
		}

		(z ? dp_reverse : dp_forwards) = std::move(dp);

		reverse(V.begin(), V.end());
		for (int i = 0; i < 2*N; i++) V[i] = !V[i];
	}

	reverse(dp_reverse.begin(), dp_reverse.end());

	vector<num> fact(N+1);
	fact[0] = 1;
	for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * num(i);

	num ans = dp_forwards[2*N][0];
	for (int i = 0; i < 2 * N; i++) {
		if (!V[i]) {
			// before and after must match exactly
			const auto& f = dp_forwards[i];
			const auto& r = dp_reverse[i+1];
			for (int v = 0; v < int(f.size()) && v < int(r.size()); v++) {
				ans += fact[v] * f[v] * r[v];
			}
		}
	}
	cout << ans << '\n';

	return 0;
}
