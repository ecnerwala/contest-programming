#include <bits/stdc++.h>

int modinv(int a, int m) { return a == 1 ? 1 : m - int(int64_t(modinv(m % a, a)) * m / a); }

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	int v;
	modnum() : v(0) {}
	modnum(int v_) : v(v_ % MOD) { if (v < 0) v += MOD; }
	explicit operator int () const { return v; }
	friend std::ostream& operator << (std::ostream& o, modnum n) { return o << int(n); }

	modnum& operator += (modnum o) {
		v += o.v;
		if (v >= MOD) v -= MOD;
		return *this;
	}
	modnum& operator -= (modnum o) {
		v -= o.v;
		if (v < 0) v += MOD;
		return *this;
	}
	modnum& operator *= (modnum o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	friend modnum operator + (modnum a, modnum b) { return a += b; }
	friend modnum operator - (modnum a, modnum b) { return a -= b; }
	friend modnum operator * (modnum a, modnum b) { return a *= b; }

	friend modnum inv(modnum a) {
		modnum r;
		r.v = modinv(a.v, MOD);
		return r;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	std::vector<int> P(N);
	P[0] = -1;
	for (int i = 1; i < N; i++) { cin >> P[i]; P[i]--; }

	std::vector<int> depth(N);
	depth[0] = 0;
	for (int i = 1; i < N; i++) depth[i] = depth[P[i]]+1;

	std::vector<int> ch(N);
	for (int i = 1; i < N; i++) ch[P[i]]++;

	using num = modnum<int(1e9)+7>;
	std::vector<num> invs(N+1);
	{
		num f = 1;
		for (int i = 1; i <= N; i++) invs[i] = f, f *= num(i);
		f = inv(f);
		for (int i = N; i >= 1; i--) invs[i] *= f, f *= num(i);
	}

	// We consider the tree with 2N states: one is the normal node state,
	// and the other is a "jumper" state which goes to normal w/ prob
	// 1/(depth+1), and goes to par jumper otherwise.

	// time from self to par jumper
	std::vector<num> ch_time_up(N);
	std::vector<num> time_up(N);
	for (int i = N-1; i >= 1; i--) {
		if (!ch[i]) {
			time_up[i] = 1;
		} else {
			// 1 + (sum over children of time_up) / ch
			// = time from self to own guy
			// Then, we 1/(depth+1) between dropping back to self or succeed
			// That means we take this loop 1 / depth times on average
			time_up[i] = invs[depth[i]] * (1 + ch_time_up[i] * invs[ch[i]]);
		}
		ch_time_up[P[i]] += time_up[i];
	}

	// time to from jumper to self
	std::vector<num> time_out(N);
	// time from par self to self
	std::vector<num> time_down(N);
	// happens immediately
	time_out[0] = 0;
	for (int i = 1; i < N; i++) {
		time_down[i] = ch[P[i]] + (ch_time_up[P[i]] - time_up[i]) + (ch[P[i]]-1) * time_out[P[i]];
		time_out[i] = (time_out[P[i]] + time_down[i]) * depth[i];
	}

	std::vector<num> sum_time_up = time_up;
	std::vector<num> sum_time_down = time_down;
	for (int i = 1; i < N; i++) {
		sum_time_up[i] += sum_time_up[P[i]];
		sum_time_down[i] += sum_time_down[P[i]];
	}

	auto get_lca = [&](int a, int b) -> int {
		if (depth[a] > depth[b]) std::swap(a, b);
		while (depth[b] > depth[a]) b = P[b];
		while (a != b) a = P[a], b = P[b];
		return a;
	};
	for (int i = 0; i < N; i++) {
		cerr << i << ' ' << time_up[i] << ' ' << time_out[i] << ' ' << time_down[i] << '\n';
	}
	for (int q = 0; q < Q; q++) {
		int x, y; cin >> x >> y; x--, y--;
		int l = get_lca(x, y);
		num ans = 0;
		if (l != x) ans += sum_time_up[x] - sum_time_up[l] + time_out[l];
		ans += sum_time_down[y] - sum_time_down[l];
		cout << int(ans) << '\n';
	}

	return 0;
}
