#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	// for each starting point, there's some point where the hall condition becomes bad, where we have more days than lecturers
	int N; cin >> N;
	struct inter_t {
		int64_t st, en;
	};
	std::vector<inter_t> intervals(N);
	for (auto& [st, en] : intervals) {
		cin >> st >> en;
		assert(st <= en);
	}

	const int Q = N;

	// First, filter the intervals down
	std::vector<int64_t> st; st.reserve(N);
	std::vector<int64_t> en; en.reserve(N);
	{
		std::sort(intervals.begin(), intervals.end(), [](auto a, auto b) { return a.st < b.st; });
		std::priority_queue<int64_t, std::vector<int64_t>, std::greater<int64_t>> pq;
		auto it = intervals.begin();
		for (int64_t v = 0; !pq.empty() || it != intervals.end(); v++) {
			if (pq.empty()) {
				assert(it != intervals.end());
				assert(it->st >= v);
				v = it->st;
			}
			for (; it != intervals.end() && it->st == v; ++it) {
				pq.push(it->en);
			}
			while (!pq.empty() && pq.top() < v) {
				pq.pop();
			}
			if (!pq.empty()) {
				st.push_back(v);
				en.push_back(pq.top());
				pq.pop();
			}
		}
		std::sort(en.begin(), en.end());
		for (int i = int(en.size()) - 1; i >= 1; i--) {
			en[i-1] = std::min(en[i-1], en[i] - 1);
		}
		assert(int(st.size()) == int(en.size()));
		N = int(st.size());
	}

	std::vector<int64_t> ans(Q+1);
	{
		int64_t cur_hi = 0;
		int i = 0, j = 0;
		while (i < N) {
			int cur_cnt = j - i;
			// There are 2 possible events:
			//   if hi == st[j], then we get an extra
			//   if hi == en[i] + cur_cnt, then we lose one
			// Option (a) takes precedence, so use a strict comparison here.
			if (j == N || (i < j && en[i] - i < st[j] - j)) {
				int64_t nxt_hi = (en[i] - i) + j;
				ans[cur_cnt] += nxt_hi - std::exchange(cur_hi, nxt_hi);
				i ++;
			} else {
				int64_t nxt_hi = (st[j] - j) + j;
				ans[cur_cnt] += nxt_hi - std::exchange(cur_hi, nxt_hi);
				j ++;
			}
		}
	}

	for (int i = Q; i >= 1; i--) {
		ans[i-1] += ans[i];
	}
	for (int i = 1; i <= Q; i++) {
		cout << ans[i] << '\n';
	}

	return 0;
}
