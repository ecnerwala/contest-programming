#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}


template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }
template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

class SevenSegmentDisplayNim {

public:

	int whoWins(vector <int> LO, vector <int> HI) {
		assert(int(LO.size()) == 7);
		assert(int(HI.size()) == 7);
		using num = modnum<int(1e9)+7>;
		num TOT = 1;
		for (int i = 0; i < 7; i++) {
			TOT *= num(HI[i] - LO[i] + 1);
		}

		std::array<int, 3> vlo{max({LO[0], LO[1], LO[2]}), LO[3], max({LO[4], LO[5], LO[6]})};
		std::array<int, 3> vhi{min({HI[0], HI[1], HI[2]}), HI[3], min({HI[4], HI[5], HI[6]})};

		int lose = 0;
		for (int i = vlo[0]; i <= vhi[0]; i++) {
			for (int j = vlo[1]; j <= vhi[1]; j++) {
				int k = i ^ j;
				lose += vlo[2] <= k && k <= vhi[2];
			}
		}

		return int(TOT - num(lose));
	}
};

void my_tests() {
	// FILL IN TESTS
	cerr << SevenSegmentDisplayNim().whoWins(vector<int>(7, 0), vector<int>(7, 10000)) << '\n';

	auto unpack = [](int msk) -> array<int, 7> {
		array<int, 7> r;
		for (int i = 0; i < 7; i++) {
			r[i] = msk % 6;
			msk /= 6;
		}
		return r;
	};
	auto pack = [](array<int, 7> a) -> int {
		int resp = 0;
		for (int i = 6; i >= 0; i--) resp = resp * 6 + a[i];
		return resp;
	};
	vector<bool> wins(279936);
	for (int state = 0; state < int(wins.size()); state++) {
		array<int, 7> a = unpack(state);
		wins[state] = false;
		for (int v0 = 0; v0 <= a[0]; v0++) {
			for (int v1 = 0; v1 <= a[1]; v1++) {
				for (int v2 = 0; v2 <= a[2]; v2++) {
					for (int v3 = 0; v3 <= a[3]; v3++) {
						if (v0 && v1 && v2 && v3) continue;
						for (int v4 = 0; v4 <= a[4]; v4++) {
							for (int v5 = 0; v5 <= a[5]; v5++) {
								for (int v6 = 0; v6 <= a[6]; v6++) {
									if (v0 && v1 && v2 && v4 && v5 && v6) continue;
									if (v3 && v4 && v5 && v6) continue;
									if (!(v0 || v1 || v2 || v3 || v4 || v5 || v6)) continue;
									if (!wins[pack({a[0] - v0, a[1] - v1, a[2] - v2, a[3] - v3, a[4] - v4, a[5] - v5, a[6] - v6})]) wins[state] = true;
								}
							}
						}
					}
				}
			}
		}
		if (!wins[state]) {
			for (int v : a) cerr << v << ' ';
			cerr << '\n';
		}
		bool my_loses = (a[0] == a[1] && a[1] == a[2] && a[4] == a[5] && a[5] == a[6] && !(a[0] ^ a[3] ^ a[6]));
		assert(!wins[state] == my_loses);
	}
}


}  // namespace ecnerwala

using ecnerwala::SevenSegmentDisplayNim;

// vim:ft=cpp
