#include <bits/stdc++.h>

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// coeff of a in x and y
	T vx = 1, vy = 0;
	while (x) {
		T k = y / x;
		y %= x;
		vy -= k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
	}
	assert(y == 1);
	return vy < 0 ? m + vy : vy;
}

template <typename T> struct extended_gcd_result {
	T gcd;
	T coeff_a, coeff_b;
};
template <typename T> extended_gcd_result<T> extended_gcd(T a, T b) {
	T x = a, y = b;
	// coeff of a and b in x and y
	T ax = 1, ay = 0;
	T bx = 0, by = 1;
	while (x) {
		T k = y / x;
		y %= x;
		ay -= k * ax;
		by -= k * bx;
		std::swap(x, y);
		std::swap(ax, ay);
		std::swap(bx, by);
	}
	return {y, ay, by};
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	int v;

public:

	modnum() : v(0) {}
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = mod_inv_in_range(v, MOD);
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
		v = int(int64_t(v) * int64_t(o.v) % MOD);
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
num PRIM_ROOT = 3;
num INV_PRIM_ROOT = inv(PRIM_ROOT);
struct my_num {
	num a;
	num ia;
	int v;
	bool b;
	my_num() : a(1), ia(1), v(0), b(0) {}
	my_num(num a_, num ia_, int v_, bool b_) : a(a_), ia(ia_), v(v_), b(b_) {}
	static my_num rt(num x) {
		if (x == 0) return {1, 1, 1, 0};
		else {
			// TODO: Tonelli shanks
			bool b = false;
			if (pow(x, (num::MOD - 1) / 2) == num(-1))  {
				b = true;
				x *= INV_PRIM_ROOT;
			}
			const int S = __builtin_ctz(num::MOD - 1);
			const int Q = (num::MOD - 1) >> S;
			int M = S;
			num c = pow(PRIM_ROOT, Q);
			num t = pow(x, Q);
			num R = pow(x, (Q+1)/2);
			assert(t != 0);
			while (true) {
				if (t == 1) {
					// done
					return {R, inv(R), 0, b};
				} else {
					int i = 0;
					num pt = t;
					while (pt != 1) {
						i++;
						pt *= pt;
					}
					num a = pow(c, 1 << (M-i-1));
					M = i;
					c = a * a;
					t *= a * a;
					R *= a;
				}
			}
		}
	}
	friend my_num operator + (my_num l, my_num r) {
		my_num res{l.a * r.a, l.ia * r.ia, l.v + r.v, bool(l.b ^ r.b)};
		if (l.b && r.b) {
			res.a *= PRIM_ROOT;
			res.ia *= INV_PRIM_ROOT;
		}
		return res;
	}
	friend my_num operator - (my_num l, my_num r) {
		my_num res{l.a * r.ia, l.ia * r.a, l.v - r.v, bool(l.b ^ r.b)};
		if (r.b && !l.b) {
			// divide by PRIM_ROOT
			res.ia *= PRIM_ROOT;
			res.a *= INV_PRIM_ROOT;
		}
		return res;
	}
	explicit operator num() const {
		assert(v >= 0);
		assert(!b);
		return v ? 0 : a;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::array<int, 2>> A(N);
		for (auto& a : A) { cin >> a[0] >> a[1]; }
		std::vector<num> ans(1 << M);
		{
			num v0 = 1;
			for (auto a : A) {
				v0 *= num(a[1] - a[0] + 1);
			}
			ans[0] = v0;
		}
		for (int q = 1; q < M; q++) {
			std::vector<my_num> vals(1 << (M-q));
			auto do_mult = [&](num v0, num v1, int h) -> void {
				assert(0 <= h && h < (1 << (M-q)));
				my_num r0 = my_num::rt(v0);
				my_num r1 = my_num::rt(v1);
				assert(num(r0 + r0) == v0);
				assert(num(r1 + r1) == v1);
				vals[0] = vals[0] + (r0 + r1);
				vals[h] = vals[h] + (r0 - r1);
			};
			int tot_hr = 0;
			for (auto [l, r] : A) {
				r++;
				r &= (1 << M) - 1;

				int vr = r & ((1 << q) - 1);
				if (vr >= (1 << (q-1))) vr -= 2 * (vr - (1 << (q-1)));
				int vl = l & ((1 << q) - 1);
				if (vl >= (1 << (q-1))) vl -= 2 * (vl - (1 << (q-1)));

				int hr = r >> q;
				//do_mult(1, -1, hr);
				tot_hr ^= hr;

				int hx = (l ^ r) >> q;
				do_mult(vr - vl, vr + vl, hx);
			}
			for (int z = 0; z < M-q; z++) {
				for (int i = 0; i < (1 << (M-q)); i++) {
					if (i & (1 << z)) continue;
					auto a = vals[i], b = vals[i + (1 << z)];
					vals[i] = a + b;
					vals[i + (1 << z)] = a - b;
				}
			}
			for (int i = 0; i < (1 << (M-q)); i++) {
				num v = num(vals[i]) * ((__builtin_popcount(tot_hr ^ i) & 1)? -1 : 1);
				ans[(i << q) + (1 << (q-1))] = v;
			}
		}

		for (int q = 0; q < M; q++) {
			for (int i = 0; i < (1 << M); i++) {
				if (i & (1 << q)) continue;
				num a = ans[i], b = ans[i + (1 << q)];
				ans[i] = a+b;
				ans[i+(1<<q)] = a-b;
			}
		}
		for (auto a : ans) { cerr << a << ' '; } cerr << '\n';

		{
			num coeff = inv(num(1 << M));
			for (int i = 0; i < (1 << M); i++) {
				ans[i] *= coeff;
			}
		}
		int final_ans = 0;
		{
			num coeff = 1;
			for (int i = 0; i < (1 << M); i++) {
				final_ans ^= int(ans[i] * coeff);
				coeff *= 2;
			}
		}
		cout << final_ans << '\n';
		break;
	}

	return 0;
}
