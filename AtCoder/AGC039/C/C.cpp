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
using num = modnum<998244353>;

const int MAXN = 2.1e5;
int N;
bool X[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	string X_; cin >> X_;
	for (int i = 0; i < N; i++) {
		X[i] = bool(X_[i] - '0');
	}

	num ans = 0;
	unordered_map<int, num> weights;
	for (int p = 2*N; p >= 1; p--) {
		if ((2 * N) % p != 0) {
			continue;
		}
		if (N % p == 0) {
			continue;
		}
		assert(p % 2 == 0);

		num weight = p;
		for (auto it : weights) {
			if (it.first % p == 0) {
				weight -= it.second;
			}
		}
		weights[p] = weight;

		num xval = 0;
		for (int i = 0; i < (p/2); i++) {
			xval *= 2;
			xval += X[i];
		}
		bool canEqual = true;
		for (int i = 0; i < N; i++) {
			bool v = X[i % (p/2)] ^ ((i / (p/2)) & 1);
			if (v < X[i]) {
				canEqual = true;
				break;
			} else if (v > X[i]) {
				canEqual = false;
				break;
			} else {
				continue;
			}
		}
		if (canEqual) xval++;
		//cerr << p << ' ' << xval << ' ' << weight << '\n';
		ans += xval * weight;
	}
	cout << ans << '\n';

	return 0;
}
