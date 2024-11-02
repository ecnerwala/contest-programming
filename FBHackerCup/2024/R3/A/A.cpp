#include<bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }
template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, K; cin >> N >> K;
		cerr << "Case #" << case_num << " " << "N = " << N << ' ' << "K = " << K << '\n';

		std::vector<std::string> G(N);
		for (auto& r : G) { cin >> r; assert(int(r.size()) == N); }
		if (K >= 4) {
			for (auto& r : G) {
				for (auto& c : r) {
					if (c == '?') c = '1';
				}
			}
			K = 0;
		}
		assert(0 <= K && K <= 3);
		int min_x = N;
		int max_x = -1;
		int min_y = N;
		int max_y = -1;
		int min_x_q = N;
		int max_x_q = -1;
		int min_y_q = N;
		int max_y_q = -1;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (G[i][j] == '1') {
					setmin(min_x, i);
					setmax(max_x, i);
					setmin(min_y, j);
					setmax(max_y, j);
				} else if (G[i][j] == '?') {
					setmin(min_x_q, i);
					setmax(max_x_q, i);
					setmin(min_y_q, j);
					setmax(max_y_q, j);
				}
			}
		}
		int ans = 0;
		auto check_rect = [&](int dx, int dy) {
			if (dx >= 0 && dy >= 0) {
				setmax(ans, dx * dy);
			}
		};
		if (K == 3) {
			// pick one corner
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					if (G[i][j] == '?') {
						// pick one guy which is on 2 boundaries, then the others are free
						check_rect((std::max(i, max_x) - std::min(min_x_q, min_x) + 1), (std::max(j, max_y) - std::min(min_y_q, min_y) + 1));
						check_rect((std::max(max_x_q, max_x) - std::min(i, min_x) + 1), (std::max(j, max_y) - std::min(min_y_q, min_y) + 1));
						check_rect((std::max(i, max_x) - std::min(min_x_q, min_x) + 1), (std::max(max_y_q, max_y) - std::min(j, min_y) + 1));
						check_rect((std::max(max_x_q, max_x) - std::min(i, min_x) + 1), (std::max(max_y_q, max_y) - std::min(j, min_y) + 1));
					}
				}
			}
		} else if (K == 2) {
			// just pick the best 2 rows
			std::vector<int> row_min_y(N);
			std::vector<int> row_max_y(N);
			for (int i = 0; i < N; i++) {
				row_min_y[i] = N;
				row_max_y[i] = -1;
				for (int j = 0; j < N; j++) {
					if (G[i][j] == '?') {
						setmin(row_min_y[i], j);
						setmax(row_max_y[i], j);
					}
				}
			}
			for (int i_min = 0; i_min < N; i_min++) {
				for (int i_max = 0; i_max < N; i_max++) {
					check_rect(
						(std::max({i_min, i_max, max_x}) - std::min({i_min, i_max, min_x}) + 1),
						(std::max(row_max_y[i_max], max_y) - std::min(row_min_y[i_min], min_y) + 1)
					);
				}
			}
		} else if (K == 1) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					if (G[i][j] == '?') {
						check_rect(
							(std::max(i, max_x) - std::min(i, min_x) + 1),
							(std::max(j, max_y) - std::min(j, min_y) + 1)
						);
					}
				}
			}
		} else if (K == 0) {
			check_rect( (max_x - min_x + 1), (max_y - min_y + 1));
		} else assert(false);

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
