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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1;
	while (b) {
		if (b & 1) r *= a;
		a *= a;
		b >>= 1;
	}
	return r;
}

int main() {
	int T; cin >> T;
	while (T-- > 0) {
		int N; cin >> N;
		string S;
		string C;
		cin >> S >> C;
		assert(int(S.size()) == N);

		// which side the top-left corner is on
		bool bot = false;
		bool right = false;
		int nh = 0;
		int nv = 0;
		for (char c : S) {
			if (c == 'L' || c == 'R') {
				if (right == (c == 'R')) right = !right;
				nv ++;
			} else if (c == 'T' || c == 'B') {
				if (bot == (c == 'B')) bot = !bot;
				nh ++;
			} else assert(false);
		}

		assert(int(C.size()) == 2);
		if (C == "BT" || C == "TB") {
			cout << pow(num(2), nv) + 1 << '\n';
		} else if (C == "LR" || C == "RL") {
			cout << pow(num(2), nh) + 1 << '\n';
		} else {
			if (C[1] == 'T' || C[1] == 'B') swap(C[0], C[1]);
			num hval = 0;
			num vval = 0;
			if (nh == 0) {
				hval = 1;
			} else {
				if (bot == (C[0] == 'B')) {
					hval = pow(num(2), nh-1) + 1;
				} else {
					hval = pow(num(2), nh-1);
				}
			}
			if (nv == 0) {
				vval = 1;
			} else {
				if (right == (C[1] == 'R')) {
					vval = pow(num(2), nv-1) + 1;
				} else {
					vval = pow(num(2), nv-1);
				}
			}
			cout << hval * vval + 1 << '\n';
		}
	}

	return 0;
}
