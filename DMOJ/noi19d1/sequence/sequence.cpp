#include<bits/stdc++.h>

int64_t solve(int N, int K, int L, const std::vector<std::array<int64_t, 2>>& A) {
	using namespace std;
	vector<array<bool, 2>> take(N);
	for (auto& t : take) t[0] = t[1] = false;

	for (int z = 0; z < 2; z++) {
		vector<pair<int64_t, int>> vals(N);
		for (int i = 0; i < N; i++) vals[i] = {A[i][z], i};
		nth_element(vals.begin(), vals.begin()+K, vals.end(), std::greater<>());
		for (int i = 0; i < K; i++) {
			take[vals[i].second][z] = true;
		}
	}

	using pq = priority_queue<pair<int64_t, int>>;
	pq add_both;
	array<pq, 2> remove_one;
	array<pq, 2> add_one;
	pq remove_both;
	auto insert_evts = [&](int i) -> void {
		if (take[i][0] && take[i][1]) {
			remove_both.emplace(-A[i][0]-A[i][1], i);
		} else if (take[i][0] ^ take[i][1]) {
			bool z = take[i][1];
			assert(take[i][z] && !take[i][!z]);
			remove_one[z].emplace(-A[i][z], i);
			add_one[!z].emplace(A[i][!z], i);
		} else {
			add_both.emplace(A[i][0] + A[i][1], i);
		}
	};
	auto clean_evts = [&]() -> void {
		for (int z = 0; z < 2; z++) {
			while (!add_one[z].empty()) {
				int i = add_one[z].top().second;
				if (!take[i][z] && take[i][!z]) {
					break;
				}
				add_one[z].pop();
			}
			while (!remove_one[z].empty()) {
				int i = remove_one[z].top().second;
				if (take[i][z] && !take[i][!z]) {
					break;
				}
				remove_one[z].pop();
			}
		}
	};

	for (int i = 0; i < N; i++) {
		insert_evts(i);
	}

	while (int(remove_both.size()) < L) {
		// options: remove_one+add_one on one side, or remove_one from each and add_both
		clean_evts();
		int64_t best_cost = -1e18;
		for (int z = 0; z < 2; z++) {
			if (remove_one[z].empty() || add_one[z].empty()) continue;
			int64_t cnd = remove_one[z].top().first + add_one[z].top().first;
			best_cost = max(best_cost, cnd);
		}
		if (!remove_one[0].empty() && !remove_one[1].empty() && !add_both.empty()) {
			int64_t cnd = remove_one[0].top().first + remove_one[1].top().first + add_both.top().first;
			best_cost = max(best_cost, cnd);
		}
		if (!add_one[0].empty() && !add_one[1].empty() && !remove_both.empty()) {
			int64_t cnd = add_one[0].top().first + add_one[1].top().first + remove_both.top().first;
			best_cost = max(best_cost, cnd);
		}

		for (int z = 0; z < 2; z++) {
			if (remove_one[z].empty() || add_one[z].empty()) continue;
			int64_t cnd = remove_one[z].top().first + add_one[z].top().first;
			if (cnd == best_cost) {
				int r = remove_one[z].top().second; remove_one[z].pop();
				int a = add_one[z].top().second; add_one[z].pop();
				take[r][z] = false;
				take[a][z] = true;
				insert_evts(r);
				insert_evts(a);
				goto done;
			}
		}
		if (!remove_one[0].empty() && !remove_one[1].empty() && !add_both.empty()) {
			int64_t cnd = remove_one[0].top().first + remove_one[1].top().first + add_both.top().first;
			if (cnd == best_cost) {
				int r0 = remove_one[0].top().second; remove_one[0].pop();
				int r1 = remove_one[1].top().second; remove_one[1].pop();
				int a = add_both.top().second; add_both.pop();
				take[r0][0] = take[r1][1] = false;
				take[a][0] = take[a][1] = true;
				insert_evts(r0);
				insert_evts(r1);
				insert_evts(a);
				goto done;
			}
		}
		if (!add_one[0].empty() && !add_one[1].empty() && !remove_both.empty()) {
			int64_t cnd = add_one[0].top().first + add_one[1].top().first + remove_both.top().first;
			if (cnd == best_cost) {
				int r = remove_both.top().second; remove_both.pop();
				int a0 = add_one[0].top().second; add_one[0].pop();
				int a1 = add_one[1].top().second; add_one[1].pop();
				take[r][0] = take[r][1] = false;
				take[a0][0] = take[a1][1] = true;
				insert_evts(r);
				insert_evts(a0);
				insert_evts(a1);
				goto done;
			}
		}
		assert(false);
done:
		;
	}

	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		for (int z = 0; z < 2; z++) {
			if (take[i][z]) {
				ans += A[i][z];
			}
		}
	}
	return ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N, K, L; cin >> N >> K >> L;
		vector<array<int64_t, 2>> A(N);
		for (int z = 0; z < 2; z++) {
			for (auto& a : A) cin >> a[z];
		}
		int64_t ans = solve(N, K, L, A);
		cout << ans << '\n';
	}

	return 0;
}
