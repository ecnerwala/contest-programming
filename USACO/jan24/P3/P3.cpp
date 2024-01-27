#include <bits/stdc++.h>

int mod_inv_in_range(int a, int m) {
	return a == 1 ? 1 : m - int(int64_t(mod_inv_in_range(m % a, a)) * m / a);
}

template <int MOD_> struct modnum {
	static const int MOD = MOD_;
	int v;
	modnum() : v(0) {}
	explicit modnum(int64_t a) : v(int(a % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }

	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }

	modnum& operator += (modnum o) { v -= MOD - o.v; v = (v < 0) ? v + MOD : v; return *this; }
	friend modnum operator + (modnum a, modnum b) { return a += b; }
	modnum& operator -= (modnum o) { v -= o.v; v = (v < 0) ? v + MOD : v; return *this; }
	friend modnum operator - (modnum a, modnum b) { return a -= b; }

	modnum& operator *= (modnum o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	friend modnum operator * (modnum a, modnum b) { return a *= b; }

	friend modnum inv(modnum a) {
		modnum r;
		r.v = mod_inv_in_range(a.v, MOD);
		return r;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::array<int, 2>> P(N);
	for (auto [x, y] : P) { cin >> x >> y; x--, y--; }

	using num = modnum<int(1e9)+7>;

	// 2N+1 points we care about
	num tot_ans = num(0);

	std::vector<num> pow_2(N+1);
	pow_2[0] = num(1);
	for (int i = 1; i <= N; i++) pow_2[i] = pow_2[i-1] + pow_2[i-1];

	for (int z = 0; z < 2; z++) {
		std::vector<int> freq(N);
		for (int i = 0; i < N; i++) freq[P[i][z]]++;
		num ans = num(N+1) * pow_2[N];
		for (int i = 0; i < N; i++) {
			ans -= pow_2[N - freq[i]];
		}
		cerr << "ans 2d " << z << ' ' << ans << '\n';
		ans -= num(2) * pow_2[N];
		ans += num(1);
		cerr << "ans 1d " << z << ' ' << ans << '\n';
		tot_ans += ans;
	}

	const num inv2 = num((num::MOD + 1) / 2);
	for (int z = 0; z < 2; z++) {
		// Now do the 2d part
		num ans_2d = num(0);

		struct seg_t {
			num val = num(0);
			num lazy = num(1);
		};
		int S = 2 * N + 1;
		std::vector<seg_t> seg(2*S);
		auto update_node = [&](int a) {
			seg[a].val = seg[a].lazy * (seg[2*a+0].val + seg[2*a+1].val);
		};
		for (int i = 0; i <= 2*N; i++) {
			seg[S+i].val = (i & 1) ? num(num::MOD-1) : num(1);
		}
		auto apply_node = [&](int a, num v) {
			seg[a].lazy *= v;
			seg[a].val *= v;
		};
		auto update_range = [&](int l, int r, num v) {
			assert(0 <= l && l < r && r <= S);
			for (int a = S+l, b = S+r; a < b; a >>= 1, b >>= 1) {
				if (a & 1) apply_node(a++, v);
				if (b & 1) apply_node(--b, v);
			}
			for (int a = (S+l)/2; a; a >>= 1) update_node(a);
			for (int a = (S+r-1)/2; a; a >>= 1) update_node(a);
		};

		std::vector<std::vector<int>> by_x(N);
		for (int i = 0; i < N; i++) {
			auto [x, y] = P[i];
			if (z) y = N-1-y;
			by_x[x].push_back(y);
			update_range(0, 2*y+1, num(2));
		}

		for (int x = 0; x <= 2*N; x++) {
			if (x & 1) {
				for (int y : by_x[x / 2]) {
					update_range(0, 2*y+1, inv2);
				}
			}

			if (x & 1) ans_2d -= seg[1].val;
			else ans_2d += seg[1].val;

			if (x & 1) {
				for (int y : by_x[x / 2]) {
					update_range(2*y+1, 2*N+1, inv2);
				}
			}
		}

		ans_2d -= num(2) * pow_2[N];
		ans_2d += num(1);
		cerr << "ans 2d " << ans_2d << '\n';
		tot_ans -= ans_2d;
	}

	tot_ans *= num(2);
	cout << tot_ans << '\n';

	return 0;
}
