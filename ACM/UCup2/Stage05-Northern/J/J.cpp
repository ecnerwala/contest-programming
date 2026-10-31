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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;

	using bs = bitset<2020>;
	std::vector<bs> A(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		if (u > v) std::swap(u, v);
		assert(u < v);
		A[v][u] = true;
	}

	std::vector<bs> Ys; Ys.reserve(N);
	std::vector<std::pair<int, bs>> basis; basis.reserve(N);
	bs Y_const;

	for (int i = 0; i < N; i++) {
		bs A_reduced = A[i];
		bs Y_const_reduced = Y_const;
		for (const auto& [idx, v] : basis) {
			if (Y_const_reduced[idx]) Y_const_reduced ^= v;
			if (A_reduced[idx]) A_reduced ^= v;
		}

		int Y_const_reduced_idx = int(Y_const_reduced._Find_first());
		bool has_Y_const = (Y_const_reduced_idx < N && A_reduced[Y_const_reduced_idx]);
		if (has_Y_const) {
			A_reduced ^= Y_const_reduced;
		}

		int A_reduced_idx = int(A_reduced._Find_first());
		if (A_reduced_idx < N) {
			// Then we're linearly independent, so just add it to the basis

			if (has_Y_const) {
				has_Y_const = false;
				A_reduced ^= Y_const_reduced;
				A_reduced_idx = int(A_reduced._Find_first());
				assert(A_reduced_idx < N);
			}

			bs Y1 = A[i];
			bs Y2;
			Y2[i] = true;

			Y1[N+1+int(Ys.size())] = true;
			Y2[N+1+int(Ys.size())+1] = true;

			A_reduced[N+1+int(Ys.size())] = true;

			Ys.push_back(Y1);
			Ys.push_back(Y2);

			basis.push_back({A_reduced_idx, A_reduced});
			basis.push_back({i, Y2});
		} else {
			// We're linearly dependent, so just process things
			bs did_flip;
			for (int z = 0; z < int(Ys.size()); z++) {
				if (!(z & 1) && A_reduced[N+1+z] && A_reduced[N+1+z+1]) {
					did_flip[N+1+z] = true;
					did_flip[N+1+z+1] = true;
					Ys[z].flip(i);
					Ys[z+1].flip(i);
					A_reduced.flip(N);
					z++;
					continue;
				}

				if (A_reduced[N+1+z]) {
					did_flip[N+1+(z^1)] = true;
					Ys[z^1].flip(i);
				}
			}
			// Let's go fix the basis now
			for (auto& [a, v] : basis) {
				assert(a < i);
				if ((v & did_flip).count() & 1) {
					v.flip(i);
				}
			}

			// Now, A is just possibly 1 and possibly has_Y_const

			if (has_Y_const) {
				if (A_reduced[N]) {
					Y_const.flip(N);
					Y_const_reduced.flip(N);
				}

				bs Y1 = Y_const;
				bs Y2;
				Y2[i] = true;
				Y2[N] = true;

				Y1[N+1+int(Ys.size())] = true;
				Y2[N+1+int(Ys.size())+1] = true;

				Y_const_reduced[N+1+int(Ys.size())] = true;

				Ys.push_back(Y1);
				Ys.push_back(Y2);

				basis.push_back({Y_const_reduced_idx, Y_const_reduced});
				basis.push_back({i, Y2});

				Y_const.reset();
				if (A_reduced[N]) {
					Y_const.flip(N);
				}
			} else if (A_reduced[N]) {
				Y_const.flip(i);
			} else {
				// Nothing left
			}
		}
	}

	// Yay, we're done
	bool Y_const_nonzero = (int(Y_const._Find_first()) < N);
	int num_vars = int(Ys.size()) + Y_const_nonzero;
	assert(num_vars <= N);
	using mnum = modnum<998244353>;
	mnum ans;
	if (Y_const_nonzero) {
		// each is equally likely
		ans = pow(mnum(2), N) / mnum(2);
	} else if (Y_const[N]) {
		ans = (pow(mnum(2), N) - pow(mnum(2), N - int(Ys.size())/2)) / mnum(2);
	} else {
		ans = (pow(mnum(2), N) + pow(mnum(2), N - int(Ys.size())/2)) / mnum(2);
	}
	cout << ans << '\n';

	return 0;
}
