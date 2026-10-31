#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int64_t K; cin >> K;
		std::vector<int> A(N);
		for (auto& a : A) {
			cin >> a;
			if (a == K) a = 0;
		}

		int64_t ans = 0;
		min_pq<int64_t> delta_cost;
		for (int i = 0; i <= N; i++) {
			int64_t prv = i == 0 ? 0 : A[i-1];
			int64_t nxt = i == N ? 0 : A[i];
			if (nxt == prv) continue;
			if (nxt > prv) {
				int64_t cost_down = (K + prv) - nxt;
				int64_t cost_neutral = nxt - prv;
				ans += cost_down;
				delta_cost.push(cost_neutral - cost_down);
				ans += delta_cost.top();
				delta_cost.pop();
			} else {
				int64_t cost_neutral = prv - nxt;
				int64_t cost_up = (K + nxt) - prv;
				ans += cost_neutral;
				delta_cost.push(cost_up - cost_neutral);
			}
		}
		cout << ans / 2 << '\n';
	}

	return 0;
}
