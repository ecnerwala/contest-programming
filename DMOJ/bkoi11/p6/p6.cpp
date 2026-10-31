#include<bits/stdc++.h>

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

template <typename val_t, typename cnt_t> struct cnt_max {
	val_t v = -1;
	cnt_t cnt = 0;
	cnt_max() {}
	explicit cnt_max(val_t v_) : v(v_), cnt(1) {}
	cnt_max(val_t v_, cnt_t cnt_) : v(v_), cnt(cnt_) {}

	friend cnt_max operator | (const cnt_max& a, const cnt_max& b) {
		if (a.v > b.v) return a;
		if (b.v > a.v) return b;
		return cnt_max(a.v, a.cnt + b.cnt);
	}
	cnt_max& operator |= (const cnt_max& o) {
		return *this = *this | o;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using cnt_t = cnt_max<int, modnum<30013>>;

	int N; cin >> N;
	vector<cnt_t> dp(N);

	vector<pair<array<int, 2>, int>> evts; evts.reserve(2*N);
	vector<int> coords; coords.reserve(2*N);

	for (int i = 0; i < N; i++) {
		int a, b, c, d; cin >> a >> b >> c >> d;
		assert(a < b && c < d);
		evts.push_back({array<int, 2>{a, c}, i});
		evts.push_back({array<int, 2>{b, d}, ~i});
		coords.push_back(c);
		coords.push_back(d);
	}
	sort(coords.begin(), coords.end());
	sort(evts.begin(), evts.end());

	vector<cnt_t> bit(2*N);

	cnt_t ans(0, 1);

	for (auto it : evts) {
		int y = int(lower_bound(coords.begin(), coords.end(), it.first[1]) - coords.begin());

		if (it.second < 0) {
			cnt_t v = dp[~it.second];
			// insert y
			for (int a = y+1; a <= 2*N; a += a & -a) {
				bit[a-1] |= v;
			}
		} else {
			dp[it.second] = cnt_t(0, 1);
			for (int a = y+1; a > 0; a -= a & -a) {
				dp[it.second] |= bit[a-1];
			}
			dp[it.second].v++;

			ans |= dp[it.second];
		}
	}

	cout << ans.v << ' ' << ans.cnt << '\n';

	return 0;
}
