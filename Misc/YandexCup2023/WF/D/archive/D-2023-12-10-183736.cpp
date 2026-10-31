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

	// first filter intervals down to some matching
	{
		std::vector<int64_t> en_matching; en_matching.reserve(N);
		std::vector<int64_t> st_matching; st_matching.reserve(N);

		for (int z = 0; z < 2; z++) {
			std::sort(intervals.begin(), intervals.end(), [](auto a, auto b) { return a.en > b.en; });
			auto it = intervals.begin();
			std::priority_queue<std::pair<int64_t, int>> pq;
			for (int64_t lo = it->en; !pq.empty() || it != intervals.end(); lo--) {
				if (pq.empty()) {
					// jump ahead
					assert(it != intervals.end());
					assert(it->en <= lo);
					lo = it->en;
				}
				for (; it != intervals.end() && it->en >= lo; ++it) {
					pq.push({it->st, it - intervals.begin()});
				}
				while (!pq.empty() && pq.top().first > lo) {
					pq.pop();
				}
				if (!pq.empty()) {
					en_matching.push_back(lo);
					pq.pop();
				}
			}

			for (auto& a : intervals) a = {-a.en, -a.st};

			for (auto& v : st_matching) v = -v;
			std::reverse(st_matching.begin(), st_matching.end());

			for (auto& v : en_matching) v = -v;
			std::reverse(en_matching.begin(), en_matching.end());

			std::swap(st_matching, en_matching);
		}
		std::reverse(st_matching.begin(), st_matching.end());
		std::reverse(en_matching.begin(), en_matching.end());
		assert(st_matching.size() == en_matching.size());
		N = int(en_matching.size());
		intervals.resize(N);
		for (int i = 0; i < N; i++) {
			intervals[i] = {st_matching[i], en_matching[i]};
			//cerr << st_matching[i] << ' ' << en_matching[i] << '\n';
		}
	}

	std::vector<int64_t> ans(Q+1);
	{
		int i = 0, j = 0;
		int64_t cur_hi = 0;
		while (i < N) {
			int cur_cnt = j - i;
			if (j < N || (i < j && intervals[i].en - cur_cnt < intervals[j].st)) {
				int64_t nxt_hi = intervals[i].en - cur_cnt;
				ans[cur_cnt] += nxt_hi - std::exchange(cur_hi, nxt_hi);
				i ++;
			} else {
				int64_t nxt_hi = intervals[j].st;
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
