#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;

	const int H = 1.1e6;
	vector<array<double, 3>> ans(H, array<double, 3>({0, 0, 0}));

	for (int i = 0; i < N; i++) {
		int64_t x1, y1, x2, y2, x3, y3; cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
		assert(x1 <= x2 && x2 <= x3);
		assert(y1 == 0 && y2 > 0 && y3 == 0);

		int64_t dx = (x3 - x1);
		// the area at a given y is (y2 - y)^2 * (x3 - x1) / y2;
		ans[y2][2] += double(dx) / double(y2) / 2.;
		ans[y2][1] -= double(dx);
		ans[y2][0] += double(dx) * double(y2) / 2.;
	}

	for (int i = H-1; i > 0; i--) {
		ans[i-1][0] += ans[i][0];
		ans[i-1][1] += ans[i][1];
		ans[i-1][2] += ans[i][2];
	}

	vector<double> actual_ans(H);
	for (int i = 0; i < H; i++) {
		actual_ans[i] = ans[i][0] + (ans[i][2] * i + ans[i][1]) * i;
	}

	for (int q = 0; q < Q; q++) {
		int64_t y; cin >> y;
		cout << fixed << setprecision(20) << actual_ans[min<int64_t>(y, H-1)] << '\n';
	}

	return 0;
}
