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
template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

using num = modnum<998244353>;

void test2() {
	int ans = 0;
	for (int mr = 0; mr < 4; mr++) {
		for (int mc = 0; mc < 4; mc++) {
			for (int a = 0; a < 4; a++) {
				for (int b = 0; b < 4; b++) {
					if ((a & mr) == a && (b & mc) == b) {
						if ((a && b) == false) {
							int sgn = __builtin_popcount(mc) + __builtin_popcount(mr) + __builtin_popcount(a) + __builtin_popcount(b);
							int p2 = (2 - __builtin_popcount(mc)) * (2 - __builtin_popcount(mr));
							if (sgn & 1) ans -= (1 << p2);
							else ans += (1 << p2);

						}
					}
				}
			}
		}
	}
	cerr << ans << '\n';
	assert(ans == 11);
}

const int MAXN = 410;

int N;
int H[MAXN];
num fact[MAXN];
num ifact[MAXN];
num p2[MAXN*MAXN];
void precomp() {
	fact[0] = 1;
	for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * num(i);
	ifact[N] = inv(fact[N]);
	for (int i = N; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);
	p2[0] = 1;
	for (int i = 1; i <= N*N; i++) p2[i] = p2[i-1] + p2[i-1];
}
int pneg1(int n) {
	return (n & 1) ? -1 : 1;
}

num binom(int n, int r) {
	assert(0 <= r && r <= n);
	return fact[n] * ifact[n-r] * ifact[r];
}

vector<pair<num, num>> dp(int l, int r, int h) {
	if (l > r) {
		return vector<pair<num, num>>({pair<num, num>(1,0)});
	}
	int m = int(min_element(H+l, H+r+1) - H);
	//cerr << l << ' ' << r << ' ' << h << ' ' << m << '\n';

	auto v1 = dp(l, m-1, H[m]);
	auto v2 = dp(m+1, r, H[m]);
	vector<pair<num, num>> res(r-l+1+1);
	for (int i = 0; i < int(v1.size()); i++) {
		for (int j = 0; j < int(v2.size()); j++) {
			num a = v1[i].first * v2[j].first;
			num b = v1[i].first * v2[j].second + v1[i].second * v2[j].first + v1[i].second * v2[j].second;
			res[i+j].first += a;
			res[i+j].second += b;

			res[i+j+1].first -= a;
			res[i+j+1].second -= b;

			res[i+j+1].second += a + b;
		}
	}

	int dh = H[m] - h;
	for (int j = 0; j < dh; j++) {
		for (int i = 0; i <= r-l+1; i++) {
			res[i] = {
				p2[(r-l+1)-i] * res[i].first,
				(p2[(r-l+1)-i]-1) * res[i].second
			};
		}
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> H[i];
	}
	precomp();
	auto res = dp(0, N-1, 0);
	num ans = 0;
	for (auto it : res) {
		ans += it.first + it.second;
	}
	cout << ans << '\n';

	return 0;
}
