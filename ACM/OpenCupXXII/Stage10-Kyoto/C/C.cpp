#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> B(N);
	for (auto& b : B) cin >> b;
	std::vector<int> C(N);
	for (auto& c : C) cin >> c;

	std::vector<std::pair<int, int>> A(2*N);
	for (int i = 0; i < N; i++) {
		A[i] = {B[i], -1};
	}
	for (int i = 0; i < N; i++) {
		A[N+i] = {C[i], 1};
	}
	std::sort(A.begin(), A.end());
	int mx = A.back().first;
	int64_t tot_cost = int64_t(mx) * 2;
	int64_t ans = tot_cost;
	int cur = 0;
	int cur_weight = 0;
	for (auto [nxt, weight] : A) {
		if (cur_weight >= 0) {
			tot_cost -= nxt - cur;
		} else {
			tot_cost += nxt - cur;
		}
		ans = min(ans, tot_cost);
		cur = nxt;
		cur_weight += weight;
	}
	cout << ans << '\n';

	return 0;
}
