#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
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

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

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

using num = modnum<int(1e9)+7>;
struct polynomial_t {
	std::vector<num> v;
	polynomial_t() {}
	explicit polynomial_t(int deg) : v(deg+1) {}
	explicit polynomial_t(std::vector<num>&& v_) : v(std::move(v_)) {}
	/*
	friend polynomial_t operator * (const polynomial_t& a, const polynomial_t& b) {
		polynomial_t o;
		if (!a.v.empty() && !b.v.empty()) {
			o.v.resize(a.v.size() + b.v.size() - 1);
			for (int i = 0; i < int(a.v.size()); i++) {
				for (int j = 0; j < int(b.v.size()); j++) {
					o.v[i+j] += a.v[i] * b.v[j];
				}
			}
		}
		return o;
	}
	friend polynomial_t operator + (const polynomial_t& a, const polynomial_t& b) {
		polynomial_t o;
		o.v.resize(std::max(a.v.size(), b.v.size()));
		for (int i = 0; i < int(a.v.size()); i++) {
			o.v[i] += a.v[i];
		}
		for (int j = 0; j < int(b.v.size()); j++) {
			o.v[j] += b.v[j];
		}
		return o;
	}

	polynomial_t& operator *= (const polynomial_t& o) {
		return *this = *this * o;
	}
	polynomial_t& operator += (const polynomial_t& o) {
		return *this = *this + o;
	}
	*/
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; }
		std::vector<int> pts; pts.reserve(N+1);
		int num_extra = 0;
		for (int i = 0; i < N; i++) {
			if (A[i] != -1) {
				pts.push_back(A[i]);
			} else {
				num_extra++;
			}
		}
		pts.push_back(0);
		pts.push_back(M-1);
		std::sort(pts.begin(), pts.end());
		num ans = 0;

		std::map<int, num> mem;

		auto check_point_strict = [&](int v, bool global_strict, bool magic_strict) -> num {
			if (global_strict == magic_strict) {
				int x = v + global_strict;
				if (mem.count(x)) {
					return mem[x];
				}
			}
			int tot_balance = 0;
			struct ret_t {
				std::array<polynomial_t, 2> ways; // ways depending on magic val and extra global
			};

			std::vector<num> tmp0(num_extra+1);
			std::vector<num> tmp1(num_extra+1);
			std::vector<num> tmp2(num_extra+1);

			auto val = std::y_combinator([&](auto self, int lo, int hi) -> ret_t {
				ret_t r;
				if (hi - lo == 1) {
					if (A[lo] == -1) {
						r.ways[0].v.resize(2);
						r.ways[1].v.resize(2);

						// v ways to be small/small
						r.ways[0].v[0] += num(v);

						// M-1-v ways to be big/big
						r.ways[1].v[1] += num(M-1-v);

						// 1 way to be equal, which depends on strictness
						r.ways[magic_strict ? 0 : 1].v[global_strict ? 0 : 1] += num(1);
					} else {
						r.ways[0].v.resize(1);
						r.ways[1].v.resize(1);

						int magic = magic_strict ? A[lo] > v : A[lo] >= v;
						r.ways[magic].v[0] ++;

						int global = global_strict ? A[lo] > v : A[lo] >= v;
						tot_balance += global;
					}
				} else if (hi - lo == 2) {
					auto x = self(lo, lo+1);
					auto y = self(lo+1, hi);

					int lx = int(x.ways[0].v.size());
					int ly = int(y.ways[0].v.size());
					r.ways[0].v.resize(lx + ly - 1);
					r.ways[1].v.resize(lx + ly - 1);
					for (int i = 0; i < lx; i++) {
						for (int j = 0; j < ly; j++) {
							r.ways[0].v[i+j] += x.ways[0].v[i] * y.ways[0].v[j] + x.ways[0].v[i] * y.ways[1].v[j] + x.ways[1].v[i] * y.ways[0].v[j];
							r.ways[1].v[i+j] += x.ways[1].v[i] * y.ways[1].v[j];
						}
					}
				} else {
					int l = (hi - lo) / 3;
					int m0 = lo + l + (0 < (hi - lo) % 3);
					int m1 = m0 + l + (1 < (hi - lo) % 3);

					auto a = self(lo, m0);
					auto b = self(m0, m1);
					auto c = self(m1, hi);

					int la = int(a.ways[0].v.size());
					int lb = int(b.ways[0].v.size());
					int lc = int(c.ways[0].v.size());

					for (int i = 0; i < la + lb - 1; i++) {
						tmp0[i] = tmp1[i] = tmp2[i] = 0;
					}
					for (int i = 0; i < la; i++) {
						for (int j = 0; j < lb; j++) {
							tmp0[i+j] += a.ways[0].v[i] * b.ways[0].v[j];
							tmp1[i+j] += a.ways[0].v[i] * b.ways[1].v[j];
							tmp1[i+j] += a.ways[1].v[i] * b.ways[0].v[j];
							tmp2[i+j] += a.ways[1].v[i] * b.ways[1].v[j];
						}
					}

					int lab = la + lb - 1;
					r.ways[0].v.resize(lab+lc-1);
					r.ways[1].v.resize(lab+lc-1);
					for (int i = 0; i < lab; i++) {
						for (int j = 0; j < lc; j++) {
							r.ways[0].v[i+j] += tmp0[i] * (c.ways[0].v[j] + c.ways[1].v[j]);
							r.ways[0].v[i+j] += tmp1[i] * c.ways[0].v[j];
							r.ways[1].v[i+j] += tmp1[i] * c.ways[1].v[j];
							r.ways[1].v[i+j] += tmp2[i] * (c.ways[0].v[j] + c.ways[1].v[j]);
						}
					}
				}

				return r;
			})(0, N);

			num cur = 0;
			for (int i = 0; i < int(val.ways[1].v.size()); i++) {
				if ((i + tot_balance) * 2 > N) {
					cur += val.ways[1].v[i];
				}
			}
			if (magic_strict ^ global_strict) cur = -cur;
			if (magic_strict == global_strict) {
				int x = v + global_strict;
				mem[x] = cur;
			}
			return cur;
		};

		auto check_point = [&](int v) -> num {
			assert(0 <= v && v < M);
			num ret = 0;
			for (bool global_strict : {false, true}) {
				for (bool magic_strict : {false, true}) {
					ret += check_point_strict(v, global_strict, magic_strict);
				}
			}
			//cerr << "point " << v << ' ' << ret << '\n';
			return ret;
		};

		auto check_range = [&](int vlo, int vhi) -> num {
			assert(vlo <= vhi);
			if (vhi <= vlo + num_extra) {
				num res = 0;
				for (int v = vlo; v <= vhi; v++) {
					res += check_point(v);
				}
				return res;
			}

			std::vector<num> vals(num_extra + 2);
			for (int i = 0; i <= num_extra; i++) {
				vals[i+1] = vals[i] + check_point(vlo + i);
			}
			int L = num_extra + 1;
			num res = 0;
			for (int i = 0; i <= L; i++) {
				num numer = vals[i];
				num denom = 1;
				for (int j = 0; j <= L; j++) {
					if (i == j) continue;
					numer *= num((vhi - vlo + 1) - j);
					denom *= num(i - j);
				}
				res += numer / denom;
			}
			return res;
		};

		int last = -1;
		for (int v = std::max((int(pts.size()) - num_extra) / 2 - 4, 0); v <= std::min(int(pts.size() + num_extra) / 2 + 4, int(pts.size()) - 1); v++) {
			// first check this exact point
			if (pts[v] != last) {
				ans += check_point(pts[v]);
				last = pts[v];
			}
			if (v+1 < int(pts.size()) && pts[v]+1 <= pts[v+1]-1) {
				// now check the range
				ans += check_range(pts[v]+1, pts[v+1]-1);
			}
		}

		cout << ans << '\n';
	}

	return 0;
}
