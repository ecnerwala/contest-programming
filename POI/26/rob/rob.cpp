#include <bits/stdc++.h>

const int mx[4] = {0, 1, 0, -1};
const int my[4] = {1, 0, -1, 0};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; int64_t T; cin >> N >> T;
	vector<int64_t> D(4*N);
	for (int i = 0; i < N; i++) cin >> D[i];
	for (int i = N; i < 4*N; i++) D[i] = D[i-N];
	N *= 4;

	int64_t total_dt = 0;
	int64_t total_dx = 0;
	int64_t total_dy = 0;
	for (int i = 0; i < N; i++) {
		total_dx += mx[i&3] * D[i];
		total_dy += my[i&3] * D[i];
		total_dt += D[i] + 1;
	}

	int64_t X, Y; cin >> X >> Y;
	int64_t ans = 0;
	ans += (X == 0 && Y == 0);
	auto check_seg = [&](int64_t xlo, int64_t xhi, int64_t ylo, int64_t yhi, int64_t L) {
		if (L == 0) return;

		if (xlo > xhi) swap(xlo, xhi);
		if (ylo > yhi) swap(ylo, yhi);
		assert(xlo == xhi || ylo == yhi);

		int64_t xt0 = X;
		int64_t yt0 = Y;
		int64_t xd = total_dx;
		int64_t yd = total_dy;

		if (xlo != xhi) {
			swap(xlo, ylo);
			swap(xhi, yhi);
			swap(xt0, yt0);
			swap(xd, yd);
		}
		assert(xlo == xhi);
		if (xd != 0) {
			if ((xt0 - xlo) % xd != 0) return;
			int64_t tgt_l = (xt0 - xlo) / xd;
			if (!(0 <= tgt_l && tgt_l < L)) return;
			int64_t xt = xt0 - tgt_l * xd;
			int64_t yt = yt0 - tgt_l * yd;
			assert(xlo <= xt && xt <= xhi);
			ans += (ylo <= yt && yt <= yhi);
			return;
		} else if (xt0 == xlo) {
			if (yd == 0) {
				ans += (ylo <= yt0 && yt0 <= yhi) * L;
				return;
			}
			assert(yd != 0);
			int64_t yt1 = yt0 - (L-1) * yd;
			if (yd < 0) {
				swap(yt0, yt1);
				yd *= -1;
			}
			swap(yt0, yt1);
			assert(yd > 0);
			assert(yt0 <= yt1);
			ylo = max(ylo, yt0);
			yhi = min(yhi, yt1);

			if (yhi < ylo) return;

			ylo -= yt0;
			yhi -= yt0;
			ans += (yhi + 1 + yd - 1) / yd - (ylo + yd - 1) / yd;
		}
	};

	int64_t L = T / total_dt;
	T %= total_dt;

	int64_t cur_x = 0;
	int64_t cur_y = 0;

	for (int i = 0; i < N; i++) {
		int64_t d = D[i];
		while (d > 0) {
			int64_t v = T > 0 ? min(d, T) : d;
			assert(v >= 1);
			int64_t nxt_x = cur_x + mx[i&3] * v;
			int64_t nxt_y = cur_y + my[i&3] * v;

			check_seg(cur_x + mx[i&3], nxt_x, cur_y + my[i&3], nxt_y, L + (T > 0));

			if (T > 0) T -= v;
			d -= v;
			cur_x = nxt_x;
			cur_y = nxt_y;
		}
		if (T > 0) T--;
	}

	assert(cur_x == total_dx && cur_y == total_dy);

	cout << ans << '\n';

	return 0;
}
