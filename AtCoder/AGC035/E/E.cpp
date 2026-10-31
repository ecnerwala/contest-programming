#include<bits/stdc++.h>
using namespace std;

int MOD;
template <int& MOD_> struct modnum {
	static constexpr int& MOD = MOD_;
	//static_assert(MOD_ > 0, "MOD must be positive");

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
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}

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
		v += o.v;
		if (v >= MOD) v -= MOD;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		if (v < 0) v += MOD;
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
using num = modnum<MOD>;

// if k mod 2 == 0, then you can't contain any k/2+1 consecutive from the n/2
// else k mod 2 == 1

const int MAXN = 160;
const int MAXK = MAXN;

num dp1[MAXN];
num solve1K(int n, int k) {
	assert(k >= 1);
	// can't take k+1 consecutive
	dp1[0] = 1;
	num sum = dp1[0];
	for (int i = 1; i <= n+1; i++) {
		dp1[i] = sum;

		sum += dp1[i];
		if (i >= k+1) sum -= dp1[i-(k+1)];
	}
	return dp1[n+1];
}

num go(int N, int K) {
	if (K % 2 == 0) {
		num a = solve1K(N / 2, K / 2);
		num b = solve1K(N - N / 2, K / 2);
		return a * b;
	} else if (K == 1) {
		return solve1K(N, 2);
	} else {
		assert(K >= 3);
		// store the furthest back even that is/might be linked up
		// it's (# even <= K/2+1, # odd) or (# even > K/2, 0)
		using half_state = pair<int, int>;
		using state = pair<half_state, half_state>;
		map<state, num> dp;
		dp[state()] = 1;
		for (int i = 1; i <= N; i++) {
			//cerr << "Add " << i << '\n';
			map<state, num> ndp;
			for (auto it : dp) {
				const num cnt = it.second;
				{ // case 1: don't take
					const state s = it.first;
					state ns(half_state(0, 0), s.first);
					if (ns.second.first == K/2 + 1) {
						ns.second.first = K/2;
						ns.second.second = 0;
					} else if (ns.second.first <= K/2) {
						ns.second.second = 0;
					}
					//cerr << "notake" << '\n';
					//cerr << s.first.first << ' ' << s.first.second << ' ' << s.second.first << ' ' << s.second.second << '\n';
					//cerr << ns.first.first << ' ' << ns.first.second << ' ' << ns.second.first << ' ' << ns.second.second << '\n';
					//cerr << cnt << '\n';
					ndp[ns] += cnt;
				}

				{ // case 2: take it
					const state s = it.first;
					state ns(half_state(s.second.first + 1, s.second.second), s.first);
					if (ns.second.first == K/2 + 1) {
						ns.second.first = K/2 + 1 + ns.second.second;
						ns.second.second = 0;
					} else if (ns.second.first <= K/2) {
						ns.second.second = min(ns.second.second+1, ns.second.first);
						//ns.second.second = ns.second.second+1;
					}
					//cerr << "take" << '\n';
					//cerr << s.first.first << ' ' << s.first.second << ' ' << s.second.first << ' ' << s.second.second << '\n';
					//cerr << ns.first.first << ' ' << ns.first.second << ' ' << ns.second.first << ' ' << ns.second.second << '\n';
					//cerr << cnt << '\n';

					if (ns.first.first <= K) {
						ndp[ns] += cnt;
					} else assert(ns.first.first == K+1);
				}
			}
			dp = std::move(ndp);
		}
		cerr << dp.size() << '\n';
		num ans = 0;
		for (auto it : dp) {
			ans += it.second;
		}
		return ans;
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, K;
	cin >> N >> K >> MOD;
	num ans = go(N, K);
	cout << int(ans) << '\n';

	return 0;
}
