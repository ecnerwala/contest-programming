#include <bits/stdc++.h>

template <typename T> struct array_deque {
	int lo;
	std::vector<T> v;
	array_deque() {}

	void reserve(int N) {
		v.reserve(N);
	}

	void reset() {
		lo = 0;
		v.clear();
	}

	bool empty() {
		return lo == int(v.size());
	}

	T front() {
		return v[lo];
	}
	T back() {
		return v.back();
	}
	void pop_back() {
		v.pop_back();
	}
	void push_back(T x) {
		v.push_back(x);
	}
	void pop_front() {
		lo++;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, S; cin >> N >> S;
	std::vector<int> A(N); for (auto& a : A) cin >> a;

	std::sort(A.begin(), A.end());

	std::vector<std::pair<int, int>> freq; freq.reserve(N);
	int num_0 = 0;
	int num_1 = 0;
	for (int i = 0, j = 0; i < int(A.size()); i = j) {
		while (j < int(A.size()) && A[j] == A[i]) j++;
		int cnt = j-i;
		if (A[i] == 0) {
			num_0 = cnt;
		} else if (A[i] == 1) {
			num_1 = cnt;
		} else {
			freq.push_back({A[i] - 1, cnt});
		}
	}

	// lots of 0's
	if (S < N && num_0 > 5 * num_1) {
		freq.insert(freq.begin(), {0, num_1});
		for (auto& [v, f] : freq) v++;
		num_0 = 0;
		num_1 = 1;
		S += N;
	}

	int tot_adjusted = num_0;
	for (auto [v, f] : freq) {
		tot_adjusted += v * f;
	}

	std::vector<std::vector<int>> dp(tot_adjusted+1);
	for (auto& v : dp) v.reserve(10);

	// initialize dp with 0
	for (int i = 0; i <= num_0; i++) {
		dp[i].push_back(num_0 - i);
	}

	std::vector<std::array<array_deque<std::pair<int, int>>, 2>> queues;

	int cur_tot = num_0;
	for (auto [v, f] : freq) {
		for (int rem = 0; rem < v && rem <= cur_tot; rem++) {
			// sliding window of size f
			int max_x = 0;
			for (int i = rem, cur_extra = -((cur_tot - rem) / v); i <= cur_tot; i += v, cur_extra++) {
				assert(cur_extra <= 0);
				for (auto& x : dp[i]) {
					x -= cur_extra;
					assert(x >= 0);
					max_x = std::max(max_x, x);
				}
			}

			int a = max_x / num_1 + 1;
			if (int(queues.size()) < a) {
				queues.resize(a);
				for (auto& q : queues) {
					q[0].reserve(N);
					q[1].reserve(N);
				}
			}

			for (int bucket = 0; bucket < a; bucket++) {
				queues[bucket][0].reset();
				queues[bucket][1].reset();
			}

			// propogate things forward
			for (int i = rem; i <= cur_tot + v * f; i += v) {
				// do the dp stuff
				for (auto& x : dp[i]) {
					int bucket = (x / num_1);
					assert(0 <= bucket && bucket < a);
					auto& q = queues[bucket];
					while (!q[0].empty() && x <= q[0].back().first) { q[0].pop_back(); }
					q[0].push_back({x, i});

					while (!q[1].empty() && x >= q[1].back().first) { q[1].pop_back(); }
					q[1].push_back({x, i});
				}

				dp[i].clear();

				for (int bucket = 0; bucket < a; bucket++) {
					auto& q = queues[bucket];
					while (!q[0].empty() && q[0].front().second < i - v * f) q[0].pop_front();
					while (!q[1].empty() && q[1].front().second < i - v * f) q[1].pop_front();

					assert(q[0].empty() == q[1].empty());
					if (!q[0].empty()) {
						dp[i].push_back(q[0].front().first);
					}
					if (!q[1].empty()) {
						if (q[1].front().first != q[0].front().first) {
							dp[i].push_back(q[1].front().first);
						}
					}

					// shrink down dp[i] if we want?
				}
			}

			for (int i = rem, cur_extra = -((cur_tot - rem) / v); i <= cur_tot + v * f; i += v, cur_extra++) {
				for (auto& x : dp[i]) {
					x += cur_extra;
					assert(x >= 0);
				}
			}
		}
		cur_tot += v * f;
	}

	int64_t ans = 0;
	for (auto& a : dp) {
		int cur_max = -1;
		for (int v : a) {
			cur_max = std::max(cur_max, v-1);
			ans += (v + num_1 - cur_max);
			cur_max = v + num_1;
		}
	}

	cout << ans << '\n';

	return 0;
}
