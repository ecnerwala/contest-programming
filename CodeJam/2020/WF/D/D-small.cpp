#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; int64_t R; int K; cin >> N >> R >> K;
		assert(K <= 10);
		struct circ {
			int64_t D;
			int64_t L;
		};
		vector<circ> circles(N);
		for (int i = 0; i < N; i++) {
			cin >> circles[i].D >> circles[i].L;
		}

		sort(circles.begin(), circles.end(), [&](const circ& a, const circ& b) {return a.L > b.L; });
		vector<bool> alive(N, true);

		auto circ_dist = [&](int64_t deg) -> double {
			return 2 * double(R) * sin(double(deg) / 360 / 1e9 * 2 * M_PI / 2);
		};
		double ans = 0;
		for (int i = 0; i < N; i++) {
			if (!alive[i]) continue;
			for (int j = i+1; j < N; j++) {
				if (!alive[j]) continue;
				double d = circ_dist(abs(circles[i].D - circles[j].D));
				ans = max(ans, double(circles[i].L + circles[j].L) + d);

				if (double(circles[i].L - circles[j].L) >= d) {
					// circles[j] is useless because it's fully enclosed
					alive[j] = false;
				}
			}
		}

		cout << "Case #" << case_num << ": ";
		cout << fixed << setprecision(10) << ans << '\n';
	}

	return 0;
}
