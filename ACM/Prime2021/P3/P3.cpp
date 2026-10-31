#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> P(N);
	for (auto& p : P) cin >> p;

	// possible purchases
	// utility, -price, probability
	std::vector<std::tuple<int, int, int, double>> by_utility; by_utility.reserve(3e5);

	for (int i = 0; i < N; i++) {
		int K; cin >> K;
		while (K--) {
			double prob; int v;
			cin >> prob >> v;
			v -= P[i];
			if (v >= 0) {
				by_utility.push_back({v, -P[i], i, prob});
			}
		}
	}

	std::sort(by_utility.begin(), by_utility.end(), std::greater<>());

	std::vector<double> cur_need(N, 0);
	double max_need = 0;

	// max pq
	std::priority_queue<std::pair<int, double>> pq;
	double cur_have = 0;

	double ans = 0;

	for (auto [v, p, i, prob] : by_utility) {
		pq.push({p, prob});
		max_need = std::max(max_need, cur_need[i] += prob);

		while (!pq.empty() && cur_have < max_need) {
			auto [price, val] = pq.top(); pq.pop();
			if (val <= max_need - cur_have) {
				ans += val * -price;
				cur_have += val;
			} else {
				ans += (max_need - cur_have) * -price;
				pq.push({price, val - (max_need - cur_have)});
				cur_have = max_need;
				break;
			}
		}
	}

	cout << fixed << setprecision(11) << ans << '\n';

	return 0;
}
