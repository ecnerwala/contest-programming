#include<bits/stdc++.h>

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

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		string S; cin >> S;
		assert(int(S.size()) == N);

		using num = modnum<int(1e9)+7>;

		struct dat_t {
			num len = 0;
			num val = 0;
			num before_val = 0;
			num after_val = 0;
			num both_val = 0;

			char first_char = '\0';
			num first_after = 0;
			char last_char = '\0';
			num last_before = 0;
		};

		auto merge = [&](const dat_t& a, const dat_t& b) -> dat_t {
			dat_t r;
			r.len = a.len + b.len;
			r.val = a.val + b.val;
			r.before_val = a.before_val + b.before_val;
			r.after_val = a.after_val + b.after_val;
			r.both_val = a.both_val + b.both_val;

			r.val += a.len * b.before_val;
			r.after_val += a.len * b.both_val;

			r.val += a.after_val * b.len;
			r.before_val += a.both_val * b.len;

			if (a.first_char == '\0') {
				r.first_char = b.first_char;
				r.first_after = b.first_after;
			} else {
				r.first_char = a.first_char;
				r.first_after = a.first_after + b.len;
			}
			if (b.last_char == '\0') {
				r.last_char = a.last_char;
				r.last_before = a.last_before;
			} else {
				r.last_char = b.last_char;
				r.last_before = b.last_before + a.len;
			}

			if (a.last_char != '\0' && b.first_char != '\0' && a.last_char != b.first_char) {
				r.val += a.last_before * b.first_after;
				r.before_val += b.first_after;
				r.after_val += a.last_before;
				r.both_val++;
			}
			return r;
		};

		dat_t cur;
		for (char c : S) {
			if (c == 'F') {
				dat_t nxt;
				nxt.len = 1;
				cur = merge(cur, nxt);
			} else if (c == 'X' || c == 'O') {
				dat_t nxt;
				nxt.len = 1;
				nxt.first_char = nxt.last_char = c;
				nxt.first_after = nxt.last_before = 1;
				cur = merge(cur, nxt);
			} else if (c == '.') {
				cur = merge(cur, cur);
			} else assert(false);
		}

		cout << "Case #" << case_num << ": " << cur.val << '\n';
	}

	return 0;
}
