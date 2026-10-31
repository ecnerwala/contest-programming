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
	int v;

public:

	modnum() : v(0) {}
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

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
		v = int(int64_t(v) * int64_t(o.v) % MOD);
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

		int N, K; cin >> N >> K;
		cerr << "Case #" << case_num << " N = " << N << ' ' << "K = " << K << '\n';
		std::vector<int64_t> ans; ans.reserve(2*K);
		auto prune_vec = [&](std::vector<int64_t>& v) {
			if (int(v.size()) > K) {
				std::nth_element(v.begin(), v.begin() + K, v.end(), std::greater<>());
				v.resize(K);
			}
		};

		std::vector<std::tuple<int, int, bool, int>> evts; evts.reserve(2*N);
		for (int i = 0; i < N; i++) {
			int a, b, x, y; cin >> a >> b >> x >> y;
			evts.push_back({a, x, false, i});
			evts.push_back({b, y, true, i});
		}
		std::sort(evts.begin(), evts.end());

		std::vector<std::vector<int64_t>> vec_paths(N);
		std::vector<int64_t> cur_paths; cur_paths.reserve(2*K+1);
		int cur_path_time = -1;
		for (auto [t, c, is_sell, i] : evts) {
			if (t > cur_path_time) {
				cur_paths.clear();
				cur_path_time = t;
			}
			if (is_sell) {
				for (auto v : vec_paths[i]) {
					cur_paths.push_back(v - c);
				}
				cur_paths.push_back(-c);
				prune_vec(cur_paths);
			} else {
				vec_paths[i] = cur_paths;
				for (auto& v : vec_paths[i]) {
					v += c;
					ans.push_back(v);
				}
				prune_vec(ans);
			}
		}

		prune_vec(ans);
		using num = modnum<int(1e9)+7>;
		num tot = 0;
		for (int64_t v : ans) tot += num(v);
		cout << "Case #" << case_num << ": " << tot << '\n';
	}

	return 0;
}
