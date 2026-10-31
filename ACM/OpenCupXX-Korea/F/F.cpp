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

using ll = long long;
const ll INF = 1e18;
const int MAXQ = 3.1e5;

struct fn {
	ll lg = 0;
	ll shift = 0;
	num p2_mod = 1;
	num shift_mod = 0;
};

fn merge(fn a, fn b) {
	fn res;
	res.lg = a.lg + b.lg;
	res.p2_mod = a.p2_mod * b.p2_mod;
	if (a.shift == 0) {
		res.shift = 0;
	} else if (b.lg >= 62) {
		res.shift = INF;
	} else {
		auto v = __int128_t(a.shift) << b.lg;
		if (v >= INF) {
			res.shift = INF;
		} else {
			res.shift = ll(v);
		}
	}
	res.shift = min(res.shift + b.shift, INF);
	res.shift_mod = a.shift_mod * b.p2_mod + b.shift_mod;
	return res;
}

bool inRange(fn a, int x) {
	if (x < a.shift) {
		return false;
	}
	x -= a.shift;
	if (x == 0) {
		return true;
	}
	return __builtin_ctz(x) >= a.lg;
}

int G;
const int MAXG = MAXQ;
bool isInf[MAXG];
const int S = 19;
fn seg[(1 << S) * 2];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int Q; cin >> Q;
	G = 1;
	for (int q = 0; q < Q; q++) {
		int t; cin >> t;
		if (t == 1) {
			int k; cin >> k;
			fn cur;
			if (k == 0) {
				cur.lg = 1;
				cur.p2_mod = 2;
			} else {
				cur.shift = k;
				cur.shift_mod = k;
			}

			for (int i = (1 << S) + G-1; i; i /= 2) {
				seg[i] = merge(seg[i], cur);
			}

			isInf[G] = (k == 0);

			G++;
		} else if (t == 2) {
			int g, x; cin >> g >> x; x--;
			fn cur;
			for (int i = (1 << S) + g, j = (2 << S); i < j; i /= 2, j /= 2) {
				if (i & 1) {
					cur = merge(cur, seg[i]);
					++i;
				}
				assert(!(j & 1));
			}
			cout << cur.p2_mod * (isInf[g] ? num(x) * 2 + 1 : num(x)) + cur.shift_mod << '\n';
		} else if (t == 3) {
			int x; cin >> x;
			if (inRange(seg[1], x)) {
				cout << 0 << '\n';
			} else {
				fn curF;
				int cur = 1;
				while (cur < (1 << S)) {
					if (inRange(merge(seg[2 * cur + 1], curF), x)) {
						curF = merge(seg[2 * cur + 1], curF);
						cur = 2 * cur;
					} else {
						cur = 2 * cur + 1;
					}
				}
				cout << cur + 1 - (1 << S) << '\n';
			}
		}
	}

	return 0;
}
