#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class Cascade {

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

	using num = modnum<int(1e9)+7>;

	struct card_t {
		int cost;
		int power;
		bool cascade;
	};

public:
	num solve(vector<card_t> cards) {
		int N = int(cards.size());
		sort(cards.begin(), cards.end(), [](const card_t& a, const card_t& b) { return a.cost < b.cost; });
		vector<num> dp(N);
		int l = 0;
		num pref = 0;
		for (int i = 0; i < N; i++) {
			while (cards[l].cost < cards[i].cost) {
				pref += dp[l];
				l++;
			}
			dp[i] = cards[i].power;
			if (cards[i].cascade && l) {
				dp[i] += pref / num(l);
			}
		}
		while (l < N) {
			pref += dp[l];
			l++;
		}
		return pref / num(N);
	}

	int expectedPower(int N, vector <int> costPrefix, vector <int> powerPrefix, string cascadePrefix, int seed, int CM, int PM, int CT) {
		vector<card_t> cards(N);
		int L = int(costPrefix.size());
		for (int i = 0; i <= L-1; i++) {
			cards[i].cost = costPrefix[i];
			cards[i].power = powerPrefix[i];
			cards[i].cascade = (cascadePrefix[i] == 'Y');
		}

		int64_t state = seed;
		for (int i = L; i <= N-1; i++) {
			state = (state * 1103515245 + 12345) % (1ull << 31);
			cards[i].cost = int(state % CM);
			state = (state * 1103515245 + 12345) % (1ull << 31);
			cards[i].power = int(state % PM);
			state = (state * 1103515245 + 12345) % (1ull << 31);
			cards[i].cascade = (state < CT);
		}
		return int(solve(std::move(cards)));
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::Cascade;

// vim:ft=cpp
