#include<bits/stdc++.h>
using namespace std;

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
int MOD;
using num = modnum<MOD>;

int N;

int numPoss(int a) {
	if (a < 0) return 0;
	return min(int(sqrt(a)), 2 * N - 1) + 1;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> MOD;

	vector<pair<int, int>> cutoffs;
	for (int i = 0; i < N; i++) {
		cutoffs.emplace_back(N * N - 1 - i * i, i);
	}
	for (int i = N; i < 2 * N; i++) {
		cutoffs.emplace_back(4 * N * N - i * i, i);
	}

	sort(cutoffs.begin(), cutoffs.end());
	//for (auto it : cutoffs) { cerr << it.first << ' ' << it.second << '\n'; }

	num ans = 0;
	for (int totInner = 0; totInner <= N; totInner++) {
		int curOuter = 0;
		int curSmall = 0;
		vector<num> dp({num(1)});
		for (int z = 0; z < 2 * N; z++) {
			int v = cutoffs[z].first;
			int i = cutoffs[z].second;
			if (i >= N) {
				for (int curInner = 0; curInner < int(dp.size()); curInner++) {
					dp[curInner] *= num(max(numPoss(v) - curInner - curOuter, 0));
				}
				curOuter++;
			} else {
				vector<num> ndp(int(dp.size())+1, num(0));
				for (int curInner = 0; curInner < int(dp.size()); curInner++) {
					ndp[curInner] += dp[curInner] * num(max(numPoss(4 * N * N - i * i)
								- (totInner + (curSmall - curInner) + N), 0));
					ndp[curInner+1] += dp[curInner] * num(max(numPoss(v) - curInner - curOuter, 0));
				}
				dp = std::move(ndp);
				curSmall++;
			}
		}
		//cerr << totInner << ' ' << dp[totInner] << '\n';
		ans += dp[totInner] * (totInner % 2 ? num(-1) : num(1));
	}
	cout << ans << '\n';

	return 0;
}
