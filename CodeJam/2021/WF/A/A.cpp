#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		int64_t W, H; cin >> W >> H;

		const int x0 = 0, y0 = 0;
		int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
		if (x1 > x2) {
			swap(x1, x2);
			swap(y1, y2);
		}
		assert(x0 <= x1 && x1 <= x2);

		int64_t tot_B = 0;

		vector<tuple<int, int, int64_t>> evts; evts.reserve(3*N);
		for (int i = 0; i < N; i++) {
			int x, y; cin >> x >> y;
			int64_t a, b; cin >> a >> b;

			tot_B += b;

			evts.push_back({x, 0, a+b});
			evts.push_back({x+x1, 1, a+b});
			evts.push_back({x+x2, 2, a+b});
		}

		using i128 = __int128;
		i128 den = 1;
		i128 n0 = 0;
		i128 n1 = 0;
		i128 n2 = 0;
		if (x0 < x1) {
			int dx = x1 - x0;
			int dy = y1 - y0;
			n0 *= dx;
			n1 *= dx;
			n2 *= dx;

			n0 += den * dy;
			n1 -= den * dy;

			den *= dx;
		}
		if (x1 < x2) {
			int dx = x2 - x1;
			int dy = y2 - y1;
			n0 *= dx;
			n1 *= dx;
			n2 *= dx;

			n1 += den * dy;
			n2 -= den * dy;

			den *= dx;
		}
		if (x0 < x2) {
			int dx = x2 - x0;
			int dy = y2 - y0;
			n0 *= dx;
			n1 *= dx;
			n2 *= dx;

			n2 += den * dy;
			n0 -= den * dy;

			den *= dx;
		}

		sort(evts.begin(), evts.end());
		for (int i = 0; i < N; i++) {
		}

		cout << "Case #" << case_num << ": ";
	}

	return 0;
}
