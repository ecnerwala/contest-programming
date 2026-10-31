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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

template <typename U, typename V> struct pairnum {
	U u;
	V v;

	pairnum() : u(0), v(0) {}
	pairnum(long long val) : u(val), v(val) {}
	pairnum(const U& u_, const V& v_) : u(u_), v(v_) {}

	friend std::ostream& operator << (std::ostream& out, const pairnum& n) { return out << '(' << n.u << ',' << ' ' << n.v << ')'; }
	friend std::istream& operator >> (std::istream& in, pairnum& n) { long long val; in >> val; n = pairnum(val); return in; }

	friend bool operator == (const pairnum& a, const pairnum& b) { return a.u == b.u && a.v == b.v; }
	friend bool operator != (const pairnum& a, const pairnum& b) { return a.u != b.u || a.v != b.v; }

	pairnum inv() const {
		return pairnum(u.inv(), v.inv());
	}
	pairnum neg() const {
		return pairnum(u.neg(), v.neg());
	}
	pairnum operator- () const {
		return pairnum(-u, -v);
	}
	pairnum operator+ () const {
		return pairnum(+u, +v);
	}

	pairnum& operator ++ () {
		++u, ++v;
		return *this;
	}
	pairnum& operator -- () {
		--u, --v;
		return *this;
	}

	pairnum& operator += (const pairnum& o) {
		u += o.u;
		v += o.v;
		return *this;
	}
	pairnum& operator -= (const pairnum& o) {
		u -= o.u;
		v -= o.v;
		return *this;
	}
	pairnum& operator *= (const pairnum& o) {
		u *= o.u;
		v *= o.v;
		return *this;
	}
	pairnum& operator /= (const pairnum& o) {
		u /= o.u;
		v /= o.v;
		return *this;
	}

	friend pairnum operator ++ (pairnum& a, int) { pairnum r = a; ++a; return r; }
	friend pairnum operator -- (pairnum& a, int) { pairnum r = a; --a; return r; }
	friend pairnum operator + (const pairnum& a, const pairnum& b) { return pairnum(a) += b; }
	friend pairnum operator - (const pairnum& a, const pairnum& b) { return pairnum(a) -= b; }
	friend pairnum operator * (const pairnum& a, const pairnum& b) { return pairnum(a) *= b; }
	friend pairnum operator / (const pairnum& a, const pairnum& b) { return pairnum(a) /= b; }
};

using mnum = modnum<998244353>;
using pnum = pairnum<mnum, mnum>;
const pnum BASE(39, 58);
std::vector<pnum> base_pow;

void init_hsh(int L) {
	if (int(base_pow.size()) < L+1) {
		base_pow.reserve(L+1);
		if (base_pow.empty()) base_pow.push_back(1);
		while (int(base_pow.size()) < L+1) {
			base_pow.push_back(base_pow.back() * BASE);
		}
	}
}

struct hsh {
	int len;
	pnum v;
	friend bool operator == (const hsh& a, const hsh& b) {
		return a.len == b.len && a.v == b.v;
	}
	friend bool operator < (const hsh& a, const hsh& b) {
		return std::make_tuple(a.len, int(a.v.u), int(a.v.v)) < std::make_tuple(b.len, int(b.v.u), int(b.v.v));
	}
	hsh append(int64_t nv) const {
		return hsh{len+1, v * BASE + nv};
	}
	hsh remove(int64_t nv) const {
		assert(len > 0);
		return hsh{len-1, v - nv * base_pow[len-1]};
	}
};

std::pair<int64_t, int64_t> solve(int64_t D, std::vector<std::array<int64_t, 2>> P) {
	using namespace std;
	using i64 = int64_t;
	int N = int(P.size());
	init_hsh(2*N);

	// rotate
	for (auto& p : P) {
		auto x = p[0], y = p[1];
		p[0] = x + y;
		p[1] = x - y;
	}

	sort(P.begin(), P.end());

	vector<i64> y_bounds; y_bounds.reserve(2*N);
	for (auto& p : P) {
		y_bounds.push_back(p[1]-D);
		y_bounds.push_back(p[1]+D);
	}
	sort(y_bounds.begin(), y_bounds.end());
	y_bounds.resize(unique(y_bounds.begin(), y_bounds.end()) - y_bounds.begin());

	vector<array<i64, 2>> cur_pts; cur_pts.reserve(N);
	vector<i64> x_bounds; x_bounds.reserve(2*N);
	vector<i64> x_lo_bounds; x_lo_bounds.reserve(N);
	vector<i64> x_hi_bounds; x_hi_bounds.reserve(N);

	struct rect_evt {
		i64 x;
		array<i64, 2> y;
		int val;
	};

	map<hsh, vector<rect_evt>> evt_map;

	i64 den = 0;

	for (int y_ind = 0; y_ind+1 < int(y_bounds.size()); y_ind++) {
		i64 ylo = y_bounds[y_ind];
		i64 yhi = y_bounds[y_ind+1];
		assert(ylo < yhi);

		cur_pts.clear();
		for (const auto& p : P) {
			if (yhi - D <= p[1] && p[1] <= ylo + D) {
				cur_pts.push_back(p);
			}
		}
		x_bounds.clear();
		x_lo_bounds.clear();
		x_hi_bounds.clear();
		for (const auto& p : cur_pts) {
			x_lo_bounds.push_back(p[0]-D);
			x_hi_bounds.push_back(p[0]+D);
		}
		merge(x_lo_bounds.begin(), x_lo_bounds.end(), x_hi_bounds.begin(), x_hi_bounds.end(), std::back_inserter(x_bounds));
		x_bounds.resize(unique(x_bounds.begin(), x_bounds.end()) - x_bounds.begin());

		int st = 0, en = 0;
		hsh cur_hsh{0, 0};
		for (int x_ind = 0; x_ind+1 < int(x_bounds.size()); x_ind++) {
			i64 xlo = x_bounds[x_ind];
			i64 xhi = x_bounds[x_ind+1];
			assert(xlo < xhi);

			while (en < int(cur_pts.size()) && cur_pts[en][0] <= xlo + D) {
				if (st < en) {
					cur_hsh = cur_hsh.append(cur_pts[en][0] - cur_pts[en-1][0]);
					cur_hsh = cur_hsh.append(cur_pts[en][1] - cur_pts[en-1][1]);
				}
				en++;
			}
			while (st < en && cur_pts[st][0] < xhi - D) {
				++st;
				if (st < en) {
					cur_hsh = cur_hsh.remove(cur_pts[st][0] - cur_pts[st-1][0]);
					cur_hsh = cur_hsh.remove(cur_pts[st][1] - cur_pts[st-1][1]);
				}
			}
			if (st < en) {
				assert(cur_hsh.len == (en - st - 1) * 2);
				/*
				hsh o_hsh{0, 0};
				for (int i = st+1; i < en; i++) {
					o_hsh = o_hsh.append(cur_pts[i][0] - cur_pts[i-1][0]);
					o_hsh = o_hsh.append(cur_pts[i][1] - cur_pts[i-1][1]);
				}
				assert(o_hsh == cur_hsh);
				*/

				i64 x0 = xlo - cur_pts[st][0], x1 = xhi - cur_pts[st][0];
				i64 y0 = ylo - cur_pts[st][1], y1 = yhi - cur_pts[st][1];
				assert(x0 < x1);
				assert(y0 < y1);

				den += (x1 - x0) * (y1 - y0);

				auto& v = evt_map[cur_hsh];
				v.push_back({x0, {y0, y1}, +1});
				v.push_back({x1, {y0, y1}, -1});
			}
		}
	}

	i64 num = 0;
	for (auto& it : evt_map) {
		//cerr << "starting hash" << '\n';
		auto& evts = it.second;
		sort(evts.begin(), evts.end(), [](const rect_evt& a, const rect_evt& b) { return a.x < b.x; });
		vector<i64> ys; ys.reserve(evts.size()*2);
		for (auto& e : evts) {
			ys.push_back(e.y[0]);
			ys.push_back(e.y[1]);
		}
		sort(ys.begin(), ys.end());
		ys.resize(unique(ys.begin(), ys.end()) - ys.begin());
		int S = int(ys.size())-1;
		struct seg_node {
			i64 len0;
			i64 len1;
			int v;
		};
		vector<seg_node> seg(2*S);
		auto update = [&](int a) {
			assert(a);
			assert(seg[a].v >= 0);
			seg[a].len0 = seg[a].len1 = 0;
			if (a >= S) {
				i64 l = ys[a-S+1] - ys[a-S];
				if (seg[a].v == 0) {
					seg[a].len0 = l;
				} else if (seg[a].v == 1) {
					seg[a].len1 = l;
				}
			} else {
				if (seg[a].v == 0) {
					seg[a].len0 = seg[2*a].len0 + seg[2*a+1].len0;
					seg[a].len1 = seg[2*a].len1 + seg[2*a+1].len1;
				} else if (seg[a].v == 1) {
					seg[a].len1 = seg[2*a].len0 + seg[2*a+1].len0;
				}
			}
		};
		auto update_all = [&](int a) {
			for (; a; a /= 2) update(a);
		};
		// init the segtree
		for (int i = 2*S-1; i; i--) update(i);

		i64 cur_x = evts[0].x;
		for (auto& e : evts) {
			//cerr << e.x - cur_x << ' ' << seg[1].len1 << '\n';
			num += (e.x - cur_x) * seg[1].len1;

			//cerr << e.x << ' ' << e.y[0] << '-' << e.y[1] << ' ' << e.val << '\n';
			int l = int(lower_bound(ys.begin(), ys.end(), e.y[0]) - ys.begin());
			int r = int(lower_bound(ys.begin(), ys.end(), e.y[1]) - ys.begin());
			assert(l < r);
			for (int a = S+l, b = S+r; a < b; a /= 2, b /= 2) {
				if (a & 1) {
					seg[a].v += e.val;
					update(a);
					a++;
				}
				if (b & 1) {
					--b;
					seg[b].v += e.val;
					update(b);
				}
			}

			update_all(S+l);
			update_all(S+r-1);
			cur_x = e.x;
		}
	}

	//cerr << num << ' ' << den << '\n';
	return {num, den};
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		//cerr << "Case #" << case_num << '\n';
		int N; int64_t D; cin >> N >> D;
		vector<array<int64_t, 2>> P(N);
		for (int i = 0; i < N; i++) {
			cin >> P[i][0] >> P[i][1];
		}
		auto res = solve(D, P);
		int64_t num = res.first;
		int64_t den = res.second;
		int64_t g = __gcd(num, den);
		num /= g;
		den /= g;

		cout << "Case #" << case_num << ": " << num << ' ' << den << '\n';
	}

	return 0;
}
