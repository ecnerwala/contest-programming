#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, D; cin >> N >> M >> D;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
		int64_t tot = 0;
		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			tot -= D;
			if (A[i] > 0) {
				tot += A[i];
				pq.push(A[i]);
				if (int(pq.size()) > M) {
					tot -= pq.top();
					pq.pop();
				}
				ans = std::max(ans, tot);
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
