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

const int MAXS = 1.1e6;
int S;
pair<int, int> P[MAXS];
int nextLeft[MAXS];
int nextRight[MAXS];

int numCh[MAXS];

num pref[MAXS];

num go() {
	for (int i = 0; i < S; i++) {
		nextLeft[i] = i-1;
		while (nextLeft[i] >= 0 && !(P[nextLeft[i]] > P[i])) {
			nextLeft[i] = nextLeft[nextLeft[i]];
		}
	}
	for (int i = S-1; i >= 0; i--) {
		nextRight[i] = i+1;
		while (nextRight[i] < S && !(P[nextRight[i]] > P[i])) {
			nextRight[i] = nextRight[nextRight[i]];
		}
	}

	pref[0] = 0;
	for (int i = 0; i < S; i++) {
		pref[i+1] = pref[i] + num(P[i].first);
	}

	num res = 0;
	for (int i = 0; i < S; i++) {
		res += num(i - nextLeft[i]) * num(S - nextRight[i]) * (num(P[i].first) * num(nextRight[i] - i) - (pref[nextRight[i]] - pref[i]));
		res += num(nextRight[i] - i) * num(nextLeft[i] + 1) * (num(P[i].first) * num(i - nextLeft[i] - 1) - (pref[i] - pref[nextLeft[i] + 1]));
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		cin >> S;
		for (int i = 0; i < S; i++) {
			cin >> P[i].first; P[i].second = i;
		}

		num res = go();

		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
