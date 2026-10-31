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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int Q; cin >> Q;

	using num = modnum<998244353>;

	vector<pair<num, num>> relaxed(1 << N);

	vector<num> ia(N-1);

	for (int i = (1<<(N-1)); i < (1 << N); i++) {
		num w; cin >> w;
		relaxed[i] = {w, w*w};
	}

	auto update_node = [&](int l, int i) {
		num b = relaxed[2*i].first + relaxed[2*i+1].first;
		num c = relaxed[2*i].second + relaxed[2*i+1].second;
		relaxed[i] = {b * ia[l], c - b * b * ia[l]};
	};
	num a = 1;
	for (int l = N-2; l >= 0; l--) {
		ia[l] = inv(2*a+1);
		a = 1 - ia[l];
		for (int i = (1<<(l+1))-1;i >= 1 << l; i--) {
			update_node(l, i);
		}
	}
	a = inv(a);

	auto solve = [&]() {
		return relaxed[1].second - relaxed[1].first * relaxed[1].first * a;
	};

	cout << solve() << '\n';

	while (Q--) {
		int x; num w; cin >> x >> w;
		relaxed[x] = {w, w*w};
		for (int i = x>>1, l = N-2; i; i >>= 1, l--) update_node(l, i);

		cout << solve() << '\n';
	}

	return 0;
}
