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

using num = modnum<998244353>;

struct quadratic {
	num a,b,c;
	friend quadratic operator + (const quadratic& f, const quadratic& g) {
		return {f.a + g.a, f.b + g.b, f.c + g.c};
	}

	num opt() const {
		// c - b^2 / a / 4
		return c - b * b / (4 * a);
	}
};

quadratic relax(quadratic g) {
	quadratic f;
	// (a+1) y^2 + (b-2x) y + (c + x^2)
	// where y = (x-b/2) / (a+1)
	// (x-b/2)^2 / (a+1) + (b-2x) * (x-b/2) / (a+1) + c + x^2
	// x^2 - (x-b/2)^2 / (a+1) + c
	// x^2 - (x-b/2)^2 / (a+1) + c
	// x^2 - x^2 / (a+1) + bx / (a+1) - (b/2)^2 / (a+1) + c
	num ia = inv(g.a+1);
	num b2 = g.b / num(2);
	f.a = 1 - ia;
	f.b = g.b * ia;
	f.c = g.c - b2 * b2 * ia;
	return f;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int Q; cin >> Q;

	vector<quadratic> relaxed(1 << N);

	for (int i = (1<<(N-1)); i < (1 << N); i++) {
		num w; cin >> w;
		relaxed[i] = quadratic{1,-2*w,w*w};
	}
	auto update_node = [&](int a) {
		relaxed[a] = relax(relaxed[2*a] + relaxed[2*a+1]);
	};
	for (int i = (1<<(N-1))-1; i > 0; i--) update_node(i);

	cout << relaxed[1].opt() << '\n';

	while (Q--) {
		int x; num w; cin >> x >> w;
		relaxed[x] = quadratic{1, -2*w, w*w};
		for (int a = x>>1; a; a >>= 1) update_node(a);

		cout << relaxed[1].opt() << '\n';
	}


	return 0;
}
