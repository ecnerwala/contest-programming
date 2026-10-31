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

	int T; cin >> T;
	while (T--) {
		int L, N; cin >> L >> N;
		vector<array<int, 2>> A(N); for (auto& a : A) { cin >> a[0] >> a[1]; a[0]--, a[1]--; }
		map<int, vector<int>> ops;
		for (auto a : A) { ops[a[0]].push_back(a[1]); ops[a[1]]; }

		using num = modnum<int(1e9)+7>;

		struct state_t {
			int r;
			int v;
			num ways;
		};
		vector<state_t> q(2*N+1);
		int lo = 0;
		int hi = 0;
		q[hi++] = {L, 0, num(1)};

		vector<num> q_ways(N+1);
		q_ways[0] = num(1);

		int last_t = -1;
		for (auto [t, o] : ops) {
			assert(lo < hi);
			assert(q[lo].r >= t);

			// first, move from last_t to t
			{
				state_t cnd{L, q[lo].v + 1, q_ways[q[lo].v] * num(t - last_t)};
				if (q[hi-1].r != L) {
					q[hi++] = cnd;
					q_ways[cnd.v] += cnd.ways;
				} else if (q[hi-1].v == cnd.v) {
					q[hi-1].ways += cnd.ways;
					q_ways[cnd.v] += cnd.ways;
				} else {
					// this guy is worse, so we ignore it
				}
			}

			last_t = t;

			// Now, anything stuck at t is bad
			if (q[lo].r == t) {
				q_ways[q[lo].v] -= q[lo].ways;
				lo++;
			}

			if (!o.empty()) {
				int r = *min_element(o.begin(), o.end());
				if (q[hi-1].r >= r) {
					state_t cnd{r, N+1, num(0)};
					while (hi > lo && q[hi-1].r >= r) {
						hi--;
						q_ways[q[hi].v] -= q[hi].ways;

						if (q[hi].v < cnd.v) {
							cnd.v = q[hi].v;
							cnd.ways = num(0);
						}
						cnd.ways += q[hi].ways;
					}
					q[hi++] = cnd;
					q_ways[cnd.v] += cnd.ways;
				}
			}
		}

		assert(hi - lo == 1);
		cout << q[lo].v << ' ' << q[lo].ways << '\n';
	}

	return 0;
}
