#include<bits/stdc++.h>
#include <numeric>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N, K; cin >> N >> K;
		auto read_vec = [&]() -> vector<int64_t> {
			vector<int64_t> res(N);
			for (int i = 0; i < K; i++) {
				cin >> res[i];
			}
			int64_t a, b, c, d; cin >> a >> b >> c >> d;
			for (int i = K; i < N; i++) {
				res[i] = (a * res[i-2] + b * res[i-1] + c) % d;
			}
			return res;
		};
		auto S = read_vec();
		auto X = read_vec();
		auto Y = read_vec();

		for (int i = 0; i < N; i++) {
			Y[i] += X[i];
		}

		int64_t totS = std::accumulate(S.begin(), S.end(), int64_t(0));
		int64_t totX = std::accumulate(X.begin(), X.end(), int64_t(0));
		int64_t totY = std::accumulate(Y.begin(), Y.end(), int64_t(0));

		int64_t ans = 0;
		if (totX <= totS && totS <= totY) {
			int64_t loCnt = 0;
			int64_t hiCnt = 0;
			for (int i = 0; i < N; i++) {
				if (S[i] < X[i]) loCnt += X[i] - S[i];
				if (S[i] > Y[i]) hiCnt += S[i] - Y[i];
			}
			ans = max(loCnt, hiCnt);
		} else {
			ans = -1;
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
