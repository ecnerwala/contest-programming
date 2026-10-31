#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> bus_stops(N);
	for (auto& v : bus_stops) cin >> v;
	std::vector<int> people(N-1);
	for (auto& v : people) cin >> v;
	std::vector<int> umbrellas(N-1);
	for (auto& v : umbrellas) cin >> v;

	std::vector<int> umbrellas_used(N-1);

	std::vector<std::pair<int, int>> umbrella_q; umbrella_q.reserve(N-1);

	int balance_a = -bus_stops[0];
	int balance_b = -bus_stops[0];

	for (int i = 0; i < N-1; i++) {
		// add in people[i]
		assert(balance_a <= balance_b && balance_b <= 0);
		balance_a += people[i];
		balance_b += people[i];

		if (balance_a > 0) {
			int v = std::min(balance_a, umbrellas[i]);
			balance_a -= v;
			umbrella_q.push_back({v, i});
		} else {
			while (balance_a < 0) {
				if (balance_a == balance_b) {
					balance_a = balance_b = 0;
					continue;
				}
				assert(balance_a < balance_b);
				assert(!umbrella_q.empty());
				int v = std::min(-balance_a, umbrella_q.back().first);
				balance_a += v;
				umbrella_q.back().first -= v;
				if (umbrella_q.back().first == 0) umbrella_q.pop_back();
			}
			assert(0 <= balance_a && balance_a <= balance_b);
		}

		balance_a -= bus_stops[i+1];
		balance_b -= bus_stops[i+1];
		if (balance_a > 0) {
			cout << "NO" << '\n';
			exit(0);
		} else if (balance_b > 0) {
			assert(balance_a <= 0);

			while (balance_b > 0) {
				assert(balance_b > balance_a);
				assert(!umbrella_q.empty());
				int v = std::min(balance_b, umbrella_q.back().first);
				balance_b -= v;
				umbrellas_used[umbrella_q.back().second] += v;
				umbrella_q.back().first -= v;
				if (umbrella_q.back().first == 0) umbrella_q.pop_back();
			}
		}
		assert(balance_a <= balance_b && balance_b <= 0);
	}

	std::vector<std::array<int, 3>> ans(N-1);
	for (int i = 0; i < N-1; i++) {
		ans[i][1] = umbrellas_used[i];
		people[i] -= ans[i][1];

		ans[i][0] = std::min(bus_stops[i], people[i]);
		people[i] -= ans[i][0];

		ans[i][2] = people[i];
		bus_stops[i+1] -= people[i];
		assert(bus_stops[i+1] >= 0);
	}

	cout << "YES" << '\n';
	cout << std::accumulate(umbrellas_used.begin(), umbrellas_used.end(), int64_t(0)) << '\n';
	for (auto it : ans) {
		cout << it[0] << ' ' << it[1] << ' ' << it[2] << '\n';
	}

	return 0;
}
