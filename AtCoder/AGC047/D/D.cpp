#include<bits/stdc++.h>

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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using num = modnum<int(1e9)+7>;

	int H; cin >> H;
	int L = (1 << (H-1));
	vector<pair<int, num>> P(L);
	for (int i = 0; i < L; i++) {
		cin >> P[i].first; P[i].first--;
	}

	// product including itself (only for leaves)
	vector<num> val(2*L);
	val[1] = 1;
	for (int i = 2; i < 2*L; i++) val[i] = val[i/2] * num(i);
	val = vector<num>(val.begin()+L, val.end());

	for (int i = 0; i < L; i++) {
		P[i].second = val[i] * val[P[i].first];
	}

	// the stuff to exclude given a root
	vector<num> ival(L);
	ival[1] = 1;
	for (int i = 2; i < L; i++) ival[i] = ival[i/2] / num(i);
	for (int i = 1; i < L; i++) {
		ival[i] = ival[i] * ival[i] * num(i);
	}

	vector<pair<int, num>> buf; buf.reserve(L);

	num ans = 0;

	for (int i = L-1; i >= 1; i--) {
		// sort the interval
		int height = __builtin_clz(i) - __builtin_clz(L);
		assert(height >= 1);
		int lo = (i << height) - L;
		// mergesort this range together
		buf.clear();
		std::y_combinator([&](auto self, int l1, int r1, int l2, int r2) -> void {
			assert(l1 < r1 || l2 < r2);
			if (r1 - l1 == 0 || r2 - l2 == 0) {
				// just handle both, one of these will be empty
				buf.insert(buf.end(), P.begin()+l1, P.begin()+r1);
				buf.insert(buf.end(), P.begin()+l2, P.begin()+r2);
				return;
			}
			assert(l1 < r1 && l2 < r2);
			int mi = min(P[l1].first, P[l2].first);
			int ma = max(P[r1-1].first, P[r2-1].first);
			assert(mi < ma);
			int h = 32 - __builtin_clz(mi ^ ma);
			assert(h >= 1);
			assert(((L + mi) >> h) == ((L + ma) >> h));
			assert(((L + mi) >> (h-1)) != ((L + ma) >> (h-1)));
			int md = (((L + ma) >> (h-1)) << (h-1)) - L;
			assert(mi < md && md <= ma);

			int m1 = l1; while (m1 < r1 && P[m1].first < md) m1++;
			num a1 = 0, b1 = 0;
			for (int x = l1; x < m1; x++) a1 += P[x].second;
			for (int x = m1; x < r1; x++) b1 += P[x].second;

			int m2 = l2; while (m2 < r2 && P[m2].first < md) m2++;
			num a2 = 0, b2 = 0;
			for (int x = l2; x < m2; x++) a2 += P[x].second;
			for (int x = m2; x < r2; x++) b2 += P[x].second;

			num coeff = ival[i] * ival[(L+mi) >> h];
			ans += coeff * (a1 * b2 + a2 * b1);

			self(l1, m1, l2, m2);
			self(m1, r1, m2, r2);
		})(lo, lo + (1 << (height-1)), lo + (1 << (height-1)), lo + (1 << height));
		assert(int(buf.size()) == (1 << height));
		copy(buf.begin(), buf.end(), P.begin()+lo);
	}

	cout << ans << '\n';

	return 0;
}
