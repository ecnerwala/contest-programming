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

	using num = modnum<998244353>;

	string S; cin >> S;
	int N = int(S.size());

	// Every B, you at least halve the space
	// If there are 2 B's at distance d, then he gets d space
	// If the last 2 B's are distance 1, or the 2nd and 3rd are distance 2, or 3rd and 4th are distance 4, etc

	reverse(S.begin(), S.end());

	// At any point, we have some collection of strings ending at this point, with variable numbers of B's and trailing S's
	std::array<std::queue<pair<num, int>>, 25> last_b;
	std::array<num, 25> last_b_sum;
	std::array<num, 25> no_b_sum;
	no_b_sum[0] = 1;

	num num_win = 0;

	for (int i = 0; i < N; i++) {
		for (int l = 0; l < 25; l++) {
			while (!last_b[l].empty() && i - last_b[l].front().second > (1 << l)) {
				no_b_sum[l+1] += last_b[l].front().first;
				last_b_sum[l] -= last_b[l].front().first;
				last_b[l].pop();
			}
		}

		if (S[i] == 'B') {
			for (int l = 0; l < 25; l++) {
				num_win += last_b_sum[l];

				while (!last_b[l].empty()) last_b[l].pop();
				last_b_sum[l] = 0;

				last_b[l].push({no_b_sum[l], i});
				last_b_sum[l] += no_b_sum[l];

				no_b_sum[l] = 0;
			}
		} else if (S[i] == 'S') {
			// nothing changes
		} else if (S[i] == '?') {
			num_win *= 2;
			for (int l = 0; l < 25; l++) {
				num_win += last_b_sum[l];

				last_b[l].push({no_b_sum[l], i});
				last_b_sum[l] += no_b_sum[l];
			}
		} else assert(false);
	}

	cout << num_win << '\n';

	return 0;
}
