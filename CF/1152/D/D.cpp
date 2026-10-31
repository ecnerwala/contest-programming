#include<bits/stdc++.h>
using namespace std;

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
using num = modnum<int(1e9)+7>;
//using num = long long;

const int MAXN = 1100;
int N;
bool vis[2 * MAXN][MAXN];
pair<num, bool> dp[2 * MAXN][MAXN];

// first is any, 1st - 2nd is unused
pair<num, bool> dfs(int ind, int pref) {
	assert(0 <= ind && ind <= 2 * N);
	assert(0 <= pref && pref <= (2 * N - ind));

	if (vis[ind][pref]) return dp[ind][pref];

	pair<num, bool> res;

	if (ind == 2 * N) {
		res.first = 0;
		res.second = 0;
	} else {
		// either +1 or -1
		if (pref - 1 >= 0) {
			auto ch = dfs(ind+1, pref-1);
			// max(ch.first, ch.second + 1), ch.first
			pair<num, bool> nres = res;
			nres.first += ch.first;
			if (res.second == false && ch.second == false) {
				nres.first ++;
				nres.second = true;
			}
			res = nres;
		}
		if (pref + 1 <= 2 * N - (ind+1)) {
			auto ch = dfs(ind+1, pref+1);
			pair<num, bool> nres = res;
			nres.first += ch.first;
			if (res.second == false && ch.second == false) {
				nres.first ++;
				nres.second = true;
			}
			res = nres;
		}
	}

	vis[ind][pref] = true;
	dp[ind][pref] = res;
	return dp[ind][pref];
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;

	cout << dfs(0, 0).first << '\n';

	return 0;
}
