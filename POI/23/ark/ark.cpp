#include <bits/stdc++.h>

int modinv(int a, int m) {
	a %= m;
	return a == 1 ? 1 : int(m - int64_t(modinv(m, a)) * m / a);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int X, Y, K; cin >> X >> Y >> K;

	int Xmid = (X+1)/2;
	int Ymid = 1;

	const int a0 = 4*Xmid-3; assert(a0 == 2*X-1);
	const int b0 = 4*Ymid-3; assert(b0 == 1);

	const int invY = modinv(Y, X);

	auto pos_mod = [&](auto a, int m) -> int {
		int r = int(a % m);
		return r < 0 ? r + m : r;
	};

	auto get_loc = [&](int a, int b) -> int64_t {
		assert((a & 1) && (b & 1));
		assert(((a+b)&7) == ((a0+b0)&7));

		// (a + 8Xk1) + (b + 8Yk2) == a0 + b0
		// a = a0 - 2k mod 8X
		// b = b0 + 2k mod 8Y

		int kx = (a0 - a) >> 1; // k mod 4X
		int ky = (b - b0) >> 1; // k mod 4Y
		assert((kx & 3) == (ky & 3));

		int k4 = (kx & 3);
		kx >>= 2;
		ky >>= 2;

		kx = pos_mod(kx, X);
		ky = pos_mod(ky, Y);

		int64_t ans = ky + int64_t(pos_mod(int64_t(invY) * (kx - ky), X)) * Y;

		ans <<= 2;
		ans |= k4;

		return ans;
	};

	map<int64_t, int64_t> bounces;
	for (int k = 0; k < K; k++) {
		int x, y; cin >> x >> y;
		array<int64_t, 4> locs;

		if ((x ^ y ^ Xmid ^ Ymid) & 1) {
			locs[0] = get_loc(4*x-1, 4*y-1);
			locs[1] = get_loc(4*x-3, -(4*y-1));
			locs[2] = get_loc(-(4*x-3), -(4*y-3));
			locs[3] = get_loc(-(4*x-1), 4*y-3);
		} else {
			locs[0] = get_loc(4*x-3, 4*y-3);
			locs[1] = get_loc(-(4*x-3), 4*y-1);
			locs[2] = get_loc(-(4*x-1), -(4*y-1));
			locs[3] = get_loc(4*x-1, -(4*y-3));
		}
		for (int z = 0; z < 4; z++) {
			bounces[locs[z]] = locs[(z+1)&3];
		}
	}

	const int64_t period = 4 * int64_t(X) * int64_t(Y);

	int64_t ans = 0;

	int64_t cur_idx = -1;
	while (!bounces.empty()) {
		auto it = bounces.upper_bound(cur_idx);
		if (it == bounces.end()) {
			cur_idx -= period;
			it = bounces.begin();
		}
		ans += it->first - cur_idx - 1;

		cur_idx = it->second;
		bounces.erase(it);
		it = bounces.find(cur_idx);

		cur_idx = it->second;
		bounces.erase(it);
		it = bounces.find(cur_idx);

		cur_idx = it->second;
		bounces.erase(it);
		it = bounces.find(cur_idx);

		bounces.erase(it);
	}

	cout << ans << '\n';

	return 0;
}
