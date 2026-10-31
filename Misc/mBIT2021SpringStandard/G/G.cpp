#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		vector<array<int, 2>> P(N); for (auto& p : P) cin >> p[0] >> p[1];
		vector<array<int, 2>> Q(N); for (auto& p : Q) cin >> p[0] >> p[1];

		auto box_size = [](const vector<array<int, 2>>& poly) -> int64_t {
			int xmin = poly[0][0];
			int xmax = poly[0][0];
			int ymin = poly[0][1];
			int ymax = poly[0][1];
			for (int i = 1; i < int(poly.size()); i++) {
				xmin = min(xmin, poly[i][0]);
				xmax = max(xmax, poly[i][0]);
				ymin = min(ymin, poly[i][1]);
				ymax = max(ymax, poly[i][1]);
			}
			return int64_t(ymax) - int64_t(ymin) + int64_t(xmax) - int64_t(xmin);
		};

		// put the min element at the start
		std::rotate(P.begin(), std::min_element(P.begin(), P.end()), P.end());
		std::rotate(Q.begin(), std::min_element(Q.begin(), Q.end()), Q.end());
		for (int i = N-1; i >= 0; i--) { P[i][0] -= P[0][0], P[i][1] -= P[0][1]; }
		for (int i = N-1; i >= 0; i--) { Q[i][0] -= Q[0][0], Q[i][1] -= Q[0][1]; }

		int64_t pfac = box_size(P);
		int64_t qfac = box_size(Q);
		bool good = true;
		for (int i = 0; i < N; i++) {
			if (P[i][0] * qfac != Q[i][0] * pfac || P[i][1] * qfac != Q[i][1] * pfac) {
				good = false;
				break;
			}
		}
		cout << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
