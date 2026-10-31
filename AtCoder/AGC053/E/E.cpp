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

using num = modnum<int(1e9)+7>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<array<int, 2>> A(N);
	for (auto& a : A) {
		cin >> a[0] >> a[1];
		if (a[0] < a[1]) swap(a[0], a[1]);
	}
	sort(A.begin(), A.end());

	const int V = N+1;
	vector<num> invs(V+1);
	{
		num cur = 1;
		for (int i = 1; i <= V; i++) {
			invs[i] = cur;
			cur *= num(i);
		}
		cur = inv(cur);
		for (int i = V; i >= 1; i--) {
			invs[i] *= cur;
			cur *= num(i);
		}
	}

	vector<int> first_pred(N);
	vector<int> num_successor(N);
	for (int i = 0; i < N; i++) {
		int mi = -1;
		int ma = i;
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			if (A[md][0] > A[i][1]) {
				ma = md;
			} else {
				mi = md;
			}
		}
		first_pred[i] = ma;
		num_successor[ma]++;
		num_successor[i]--;
	}
	for (int i = 0; i+1 < N; i++) num_successor[i+1] += num_successor[i];

	num dp0 = 1, dp1 = 1, dp2 = 0;
	vector<num> pref0(N+1);
	for (int i = 0; i < N; i++) {
		dp0 *= invs[num_successor[i]+1];
		pref0[i+1] = pref0[i] + dp0;
		dp0 *= num_successor[i];

		dp2 *= num_successor[i]+2;
		dp2 += dp1 * pref0[first_pred[i]];

		dp1 *= num_successor[i]+1;
	}
	cout << dp2 + dp1 << '\n';

	return 0;
}
