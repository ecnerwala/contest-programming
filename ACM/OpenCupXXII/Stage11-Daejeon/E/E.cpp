#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	std::vector<std::pair<std::array<int, 2>, int>> A(N);
	int64_t tot = 0;
	for (auto& a : A) {
		cin >> a.first[0] >> a.first[1] >> a.second;
		tot += a.second;
	}
	std::sort(A.begin(), A.end(), [](const auto& a, const auto& b) {
		return a.first[0] < b.first[0];
	});

	for (int i = 0; i < N; i++) {
		int mi = -1;
		int ma = N;
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			if (A[md].first[0] <= A[i].first[1]) {
				mi = md;
			} else {
				ma = md;
			}
		}
		A[i].first[1] = ma;
	}
	for (int i = 0; i < N; i++) {
		A[i].first[0] = i;
	}

	std::vector<int64_t> dp(N+1);
	dp[N] = 0;
	for (int i = N-1; i >= 0; i--) {
		std::sort(A.begin()+i, A.end(), [](const auto& a, const auto& b) {
			return a.first[1] < b.first[1];
		});
		int64_t cur_tot = 0;
		std::priority_queue<int, vector<int>, greater<int>> pq;
		for (int j = i; j < N; j++) {
			cur_tot += A[j].second;
			pq.push(A[j].second);
			while (int(pq.size()) > K) {
				cur_tot -= pq.top();
				pq.pop();
			}
			dp[i] = std::max(dp[i], dp[A[j].first[1]] + cur_tot);
		}
	}

	cout << tot - dp[0] << '\n';

	return 0;
}
