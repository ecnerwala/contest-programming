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

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int N, M; cin >> N >> M;
	vector<int> S(N);
	vector<int> invS(N);
	for (int i = 0; i < N; i++) {
		cin >> S[i]; S[i] --;
		invS[S[i]] = i;
	}

	vector<int> badCnt(N);
	for (int i = 0; i+1 < N; i++) {
		int a = invS[i], b = invS[i+1];
		if (a > b) {
			badCnt[a]--;
			badCnt[b]++;
		}
	}
	for (int i = 0; i+1 < N; i++) {
		badCnt[i+1] += badCnt[i];
	}
	vector<int> cutoffs;
	cutoffs.push_back(0);
	for (int i = 0; i < N; i++) {
		if (badCnt[i] == 0) cutoffs.push_back(i+1);
	}

	vector<num> dp({num(1)});

	for (int z = 0; z+1 < int(cutoffs.size()); z++) {
		int l = cutoffs[z], r = cutoffs[z+1];
		vector<int> lhs;
		vector<int> rhs;
		for (int i = l; i < r; i++) {
			int v = S[i];
			if (rhs.empty() || v > rhs.back()) {
				rhs.push_back(v);
			} else if (lhs.empty() || v > lhs.back()) {
				lhs.push_back(v);
			} else {
				cout << 0 << '\n';
				exit(0);
			}
		}
		vector<num> ndp(r+1);
		for (int i = 0; i < int(dp.size()); i++) {
			ndp[i+int(lhs.size())] += dp[i];
			ndp[i+int(rhs.size())] += dp[i];
		}
		dp = std::move(ndp);
	}
	num ans = 0;
	for (int i = N-M; i <= M; i++) {
		ans += dp[i];
	}
	cout << ans << '\n';

	return 0;
}
