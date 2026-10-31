#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::pair<int64_t, int>> A(N);
		for (int i = 0; i < N; i++) {
			cin >> A[i].first;
			A[i].second = i;
		}

		int64_t tot_sum = 0;
		int64_t global_max = 0;
		for (int i = 0; i < N; i++) {
			tot_sum += A[i].first;
			global_max = std::max(global_max, A[i].first);
		}

		int64_t pref_ans = 0;
		std::map<int, std::pair<int64_t, int>> pref_max;

		int64_t suff_ans = 0;
		std::map<int, std::pair<int64_t, int>> suff_max;
		{
			std::pair<int64_t, int> cur = A[0];
			pref_max[0] = A[0];
			for (int i = 1; i < N; i++) {
				if (A[i] > cur) {
					pref_ans += (A[i].second - cur.second) * cur.first;
					cur = A[i];
					pref_max[i] = A[i];
				}
			}
		}
		{
			std::pair<int64_t, int> cur = A[N-1];
			suff_max[N-1] = A[N-1];
			for (int i = N-2; i >= 0; i--) {
				if (A[i] > cur) {
					suff_ans += (cur.second - A[i].second) * cur.first;
					cur = A[i];
					suff_max[i] = A[i];
				}
			}
		}

		auto pair_val = [&](auto it, auto jt) -> int64_t {
			return (jt->second.second - it->second.second) * std::min(it->second.first, jt->second.first);
		};
		auto iter_val = [&](auto mp, auto it) -> int64_t {
			int64_t ans = 0;
			if (it != mp.begin()) ans += pair_val(std::prev(it), it);
			if (std::next(it) != mp.end()) ans += pair_val(it, std::next(it));
			if (it != mp.begin() && std::next(it) != mp.end()) ans -= pair_val(std::prev(it), std::next(it));
			return ans;
		};

		auto get_ans = [&]() -> int64_t {
			return pref_ans + global_max + suff_ans - tot_sum;
		};

		int Q; cin >> Q;
		for (int q = 0; q < Q; q++) {
			int x, v; cin >> x >> v; x--;
			A[x].first += v;
			assert(v > 0);

			{
				auto it = pref_max.lower_bound(x);
				if (it != pref_max.begin() && A[x] < std::prev(it)->second) {
					// then we shouldn't insert, so noop
				} else {
					// delete things while they're good
					while (it != pref_max.end() && A[x] > it->second) {
						// go delete
						pref_ans -= iter_val(pref_max, it);
						it = pref_max.erase(it);
					}
					auto [new_it, did_insert] = pref_max.emplace(x, A[x]);
					assert(did_insert);
					pref_ans += iter_val(pref_max, new_it);
				}
			}
			{
				auto it = suff_max.upper_bound(x);
				if (it != suff_max.end() && A[x] < it->second) {
					// we shouldn't insert
				} else {
					while (it != suff_max.begin() && A[x] > std::prev(it)->second) {
						suff_ans -= iter_val(suff_max, std::prev(it));
						suff_max.erase(std::prev(it));
					}
					auto [new_it, did_insert] = suff_max.emplace(x, A[x]);
					assert(did_insert);
					suff_ans += iter_val(suff_max, new_it);
				}
			}

			tot_sum += v;
			global_max = std::max(global_max, A[x].first);
			cout << get_ans() << '\n';
		}
	}

	return 0;
}
