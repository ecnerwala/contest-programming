#include <bits/stdc++.h>

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> A(N); for (auto& a : A) cin >> a;
	vector<int> B(N+1);
	for (int i = 1; i < N; i++) cin >> B[i];
	B[0] = B[N] = 0;

	for (int i = 0; i < N; i++) {
		B[i+1] = min(B[i+1], B[i] + A[i]);
	}
	for (int i = N-1; i >= 0; i--) {
		B[i] = min(B[i], B[i+1] + A[i]);
	}
	for (int i = 0; i < N; i++) {
		A[i] = max(0, B[i] + B[i+1] - A[i]);
	}
	assert(A[0] == 0 && A[N-1] == 0);

	using num = modnum<998244353>;
	num ans = 1;
	for (int i = 0, j = 0; i < N-1; i = j) {
		assert(!A[i]);
		int tot_val = 0;
		for (j = i+1; A[j]; j++) tot_val ^= A[j] & 1;
		if (tot_val) {
			int num_bad = 0;
			for (int z = 0; z < 2; z++) {
				bool is_bad = true;
				int cur_pref = 0;
				for (int x = z ? j-1 : i+1; z ? x >= i+1 : x <= j-1; z ? x-- : x++) {
					if (A[x] == 2 * B[z ? x : x+1]) {
						num_bad += is_bad;
						is_bad = is_bad || (A[x] == 2 && cur_pref);
					} else {
						is_bad = false;
						num_bad += is_bad;
						is_bad = is_bad || (A[x] == 1 && cur_pref);
					}
					cur_pref ^= A[x] & 1;
				}
			}
			ans *= num((j-i+1) - num_bad);
		} else {
			ans *= num(1);
		}
	}

	cout << ans << '\n';

	return 0;
}

// One observation: Let the number of times we decrement each thing be a_i or b_i
//  Consider b_i and b_{i+1}: at least abs(b_i - b_{i+1}) have to have an endpoint at a_i
//   Also, a_i <= b_i + b_{i+1}
//   Also, a_i has the same parity as b_i + b_{i+1}
//  If we fix the b_i, then we should just greedily set
//    a_i = min(A_i, b_i + b_{i+1}) - eps
//    where eps is 0 or 1 to account for parity
//  and it's feasible iff A_i >= abs(b_i - b_{i+1})
//
//  Thus, we just have to pick some b_i so that
//    0 <= b_i <= B_i and A_i >= abs(b_i - b_{i+1})
//  and sum min(A_i, b_i + b_{i+1}) - eps is maximized
//
//  One thing: we can greedily cap B_i = min(B_i, B_{i+1} + A_i) and likewise the other way
//  Another: we can cap A_i = min(A_i, B_i + B_{i+1})
