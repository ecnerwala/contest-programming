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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using num = modnum<998244353>;

	int N, M; cin >> N >> M;
	num ans = 0;

	for (int v = 1; v <= M; v++) {
		num cnt_open = 0;
		num tot_open = 0;
		num cnt_closed = 1;
		num tot_closed = 0;
		for (int i = 0; i < N; i++) {
			num nxt_cnt_open = cnt_open * (M-v);
			num nxt_tot_open = tot_open * (M-v);
			num nxt_cnt_closed = cnt_closed * (M-v);
			num nxt_tot_closed = tot_closed * (M-v);

			// If the value is == v, must open
			{
				nxt_cnt_open += cnt_open;
				nxt_tot_open += tot_open;
				nxt_cnt_open += cnt_closed;
				nxt_tot_open += tot_closed + cnt_closed;
			}

			// If the value is < v, must close
			{
				nxt_cnt_closed += cnt_open * (v-1);
				nxt_tot_closed += tot_open * (v-1);
				nxt_cnt_closed += cnt_closed * (v-1);
				nxt_tot_closed += tot_closed * (v-1);
			}

			cnt_open = nxt_cnt_open;
			tot_open = nxt_tot_open;
			cnt_closed = nxt_cnt_closed;
			tot_closed = nxt_tot_closed;
		}
		ans += tot_open + tot_closed;
	}
	cout << ans << '\n';

	return 0;
}

// For each value v, we need to do 
