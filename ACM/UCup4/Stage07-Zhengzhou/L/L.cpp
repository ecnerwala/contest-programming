#include <bits/stdc++.h>

namespace kactl {
using namespace std;

#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef pair<int, int> pii;
typedef vector<int> vi;

using ll = uint32_t;
struct FT {
	vector<ll> s;
	FT(int n) : s(n) {}
	void update(int pos, ll dif) { // a[pos] += dif
		for (; pos < sz(s); pos |= pos + 1) s[pos] += dif;
	}
	ll query(int pos) { // sum of values in [0, pos)
		ll res = 0;
		for (; pos > 0; pos &= pos - 1) res += s[pos-1];
		return res;
	}
	int lower_bound(ll sum) {// min pos st sum of [0, pos] >= sum
		// Returns n if no sum is >= sum, or -1 if empty sum is.
		if (sum <= 0) return -1;
		int pos = 0;
		for (int pw = 1 << 25; pw; pw >>= 1) {
			if (pos + pw <= sz(s) && s[pos + pw-1] < sum)
				pos += pw, sum -= s[pos-1];
		}
		return pos;
	}
};

struct FT2 {
	vector<vi> ys; vector<FT> ft;
	FT2(int limx) : ys(limx) {}
	void fakeUpdate(int x, int y) {
		for (; x < sz(ys); x |= x + 1) ys[x].push_back(y);
	}
	void init() {
		for (vi& v : ys) sort(all(v)), ft.emplace_back(sz(v));
	}
	int ind(int x, int y) {
		return (int)(lower_bound(all(ys[x]), y) - ys[x].begin()); }
	void update(int x, int y, ll dif) {
		for (; x < sz(ys); x |= x + 1)
			ft[x].update(ind(x, y), dif);
	}
	ll query(int x, int y) {
		ll sum = 0;
		for (; x; x &= x - 1)
			sum += ft[x-1].query(ind(x-1, y));
		return sum;
	}
	void maybeUpdate(int x, int y, ll dif, bool real) {
		if (real) {
			update(x, y, dif);
		} else {
			fakeUpdate(x, y);
		}
	}
};
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	struct op_t {
		int op;
		std::array<int, 3> xs;
		uint32_t w;
		int q;
	};
	std::vector<op_t> ops(Q);
	int num_out = 0;
	for (auto& q : ops) {
		cin >> q.op;
		assert(q.op == 1 || q.op == 2);
		int type, x, y, d; cin >> type >> x >> y >> d;
		x--, y--;
		assert(0 <= y && y <= x && x <= N);
		uint32_t w = 0;
		int q_;
		if (q.op == 1) { cin >> w; q_ = -1; }
		else if (q.op == 2) { w = 1; q_ = num_out++; }
		else assert(false);

		if (type == 0) {
		} else if (type == 1) {
			d = -d;
		} else assert(false);

		std::array<int, 3> coords;
		coords[0] = (N-x) - d;
		coords[1] = y;
		coords[2] = x - y;
		q.xs = coords;
		q.w = w;
		q.q = q_;
		// area of a positive region is (N - coords[0] - coords[1] - coords[2])^2
		if (type == 0) assert(coords[0] + coords[1] + coords[2] < N);
		else if (type == 1) assert(coords[0] + coords[1] + coords[2] > N);
	}

	struct subop_t {
		int op;
		int dir;
		std::array<int, 2> xs;
		uint32_t w;
		int q;
	};
	std::vector<subop_t> subops; subops.reserve(ops.size() * 7);
	for (auto q : ops) {
		subops.push_back({
			q.op,
			0,
			{0, 0},
			q.w,
			q.q
		});
		for (int z = 0; z < 3; z++) {
			subops.push_back({
				q.op,
				z,
				{q.xs[(z+1)%3], q.xs[(z+2)%3]},
				q.w,
				q.q
			});
			subops.push_back({
				q.op,
				z,
				{0, q.xs[(z+2)%3]},
				-q.w,
				q.q
			});
		}
	}
	std::vector<uint32_t> ans(num_out);
	auto do_match = [&](int dir) -> void {
		kactl::FT2 ft0(N);
		kactl::FT2 ftx(N);
		kactl::FT2 ftx2(N);
		kactl::FT2 fty(N);
		kactl::FT2 fty2(N);
		kactl::FT2 ftxy2(N);
		for (int pass = 0; pass < 2; pass++) {
			for (const auto& op : subops) {
				if (op.dir != dir) continue;
				int x = op.xs[0];
				int y = op.xs[1];
				if (op.op == 1) {
					uint32_t w = op.w;
					ft0.maybeUpdate(x, y, w, pass);
					ftx.maybeUpdate(N-x, y, w * uint32_t(x), pass);
					ftx2.maybeUpdate(N-x, y, w * uint32_t(x) * uint32_t(x), pass);
					fty.maybeUpdate(x, N-y, w * uint32_t(y), pass);
					fty2.maybeUpdate(x, N-y, w * uint32_t(y) * uint32_t(y), pass);
					ftxy2.maybeUpdate(N-x, N-y, w * uint32_t(N - x - y) * uint32_t(N - x - y), pass);
				} else if (op.op == 2) {
					int q = op.q;
					ans[q] += op.w * ft0.query(x, y) * uint32_t(N - x - y) * uint32_t(N - x - y);

					ans[q] += op.w * ftx2.query(N-x+1, y);
					ans[q] -= op.w * ftx.query(N-x+1, y) * uint32_t(2 * (N-y));
					ans[q] += op.w * (ft0.query(N+1, y) - ft0.query(x, y)) * uint32_t(N-y) * uint32_t(N-y);

					ans[q] += op.w * fty2.query(x, N-y+1);
					ans[q] -= op.w * fty.query(x, N-y+1) * uint32_t(2 * (N-x));
					ans[q] += op.w * (ft0.query(x, N+1) - ft0.query(x, y)) * uint32_t(N-x) * uint32_t(N-x);

					ans[q] += op.w * ftxy2.query(N-x+1, N-y+1);
				} else assert(false);
			}
		}
	};
	do_match(0);
	do_match(1);
	do_match(2);
	auto do_mismatch = [&](int d1, int d2) -> void {
		for (int pass = 0; pass < 2; pass++) {
			for (const auto& op : subops) {
				if (op.op == 1 && op.dir != d1) continue;
				if (op.op == 2 && op.dir != d2) continue;
				int x, y;
				if (op.op == 1) {
					if ((d1 + 1) % 3 == d2) x = op.xs[1], y = op.xs[0];
					else x = op.xs[0], y = op.xs[1];
				} else if (op.op == 2) {
					if ((d2 + 1) % 3 == d1) x = op.xs[1], y = op.xs[0];
					else x = op.xs[0], y = op.xs[1];
				} else assert(false);
				// x is always the same
				// y is opposite directions
				if (op.op == 1) {
					uint32_t w = op.w;
				} else if (op.op == 2) {
					int q = op.q;
				} else assert(false);
			}
		}
	};
	for (int a = 0; a < 3; a++) {
		for (int b = 0; b < 3; b++) {
			if (a == b) continue;
			do_mismatch(a, b);
		}
	}

	for (auto v : ans) cout << v << '\n';

	return 0;
}

// We'll break out things into
// A: constant-l pieces: sum_{a <= x} l
// B: constant-r pieces: sum_{a <= x} a - r
//
// The value of (A, x, l) == 2 * x * l
// The value of (B, x, r) == x^2 - 2 * x * r = x * (x - 2r)
//
// When we compare 2 pieces, we always casework on the smaller x
//
// For AA, we just take min l
// For BB, we just take max r
// For AB, we need to do something funny
// If l+r <= x, the value is just B's value (x^2 - 2xr)
// Else the value is 2*l*(x-l-r) + l^2 - r^2 = 2lx - l^2 - 2lr - r^2 = 2lx - (l+r)^2
