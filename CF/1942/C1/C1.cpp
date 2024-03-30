#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, X, Y; cin >> N >> X >> Y;
		std::vector<int> pts(X);
		for (auto& p : pts) { cin >> p; p--; }
		std::sort(pts.begin(), pts.end());

		int num_pts = X;
		for (int i = 0; i < X; i++) {
			int g = (i+1==X ? N+pts[0] : pts[i+1]) - pts[i] - 1;
			if (g == 1) num_pts++;
		}
		cout << num_pts - 2 << '\n';
	}

	return 0;
}
