#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	int64_t K; cin >> K;
	std::vector<int64_t> F(M+N+M);
	for (int i = M; i < N+M; i++) {
		cin >> F[i];
	}
	N += 2*M;
	std::vector<int64_t> paid(N);
	std::vector<int64_t> free(N);
	std::vector<std::pair<int, int64_t>> stk; stk.reserve(N);

	int64_t ans = 0;
	int64_t cur_paid = 0;
	int64_t cur_free = 0;
	for (int i = M; i < N; i++) {
		cur_paid -= paid[i-M];
		cur_free -= free[i-M];

		free[i] = F[i];
		cur_free += free[i];
		stk.push_back({i, free[i]});

		int64_t tgt = std::min(cur_paid + cur_free, K);
		while (cur_paid < tgt) {
			while (stk.back().second == 0) {
				stk.pop_back();
			}
			int64_t diff = std::min(tgt - cur_paid, stk.back().second);
			stk.back().second -= diff;
			free[stk.back().first] -= diff;
			paid[stk.back().first] += diff;
			ans += diff;
			cur_paid += diff;
			cur_free -= diff;
		}
	}
	cout << ans << '\n';

	return 0;
}
