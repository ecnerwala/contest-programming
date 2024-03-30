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

		std::vector<int> gaps; gaps.reserve(X);
		for (int i = 0; i < X; i++) {
			int g = (i+1==X ? N+pts[0] : pts[i+1]) - pts[i] - 1;
			if (g & 1) gaps.push_back(g);
		}

		std::sort(gaps.begin(), gaps.end());

		int num_pts = X;
		for (auto g : gaps) {
			if (Y >= g/2) {
				Y -= g/2;
				num_pts += g;
			} else {
				break;
			}
		}
		num_pts += Y*2;
		num_pts = std::min(num_pts, N);

		cout << num_pts - 2 << '\n';
	}

	return 0;
}
