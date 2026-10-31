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

	int64_t last_best_cost = 0;

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
		assert(best_cost != int64_t(-1e18));
		assert(best_cost <= last_best_cost);
		last_best_cost = best_cost;
		int prv_has_both = int(remove_both.size());
		for (int z = 0; z < 2; z++) {
			if (remove_one[z].empty() || add_one[z].empty()) continue;
			int64_t cnd = remove_one[z].top().first + add_one[z].top().first;
			if (cnd == best_cost) {
				int r = remove_one[z].top().second; remove_one[z].pop();
				int a = add_one[z].top().second; add_one[z].pop();
				//cerr << "rem " << r << ' ' << z << ' ' << A[r][z] << '\n';
				//cerr << "add " << a << ' ' << z << ' ' << A[a][z] << '\n';
				//cerr << '\n';
				assert(take[r][z] && !take[r][!z]);
				assert(!take[a][z] && take[a][!z]);
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
				//cerr << "rem " << r0 << ' ' << 0 << ' ' << A[r0][0] << '\n';
				//cerr << "rem " << r1 << ' ' << 1 << ' ' << A[r1][1] << '\n';
				//cerr << "add " << a << ' ' << 0 << ' ' << A[a][0] << '\n';
				//cerr << "add " << a << ' ' << 1 << ' ' << A[a][1] << '\n';
				//cerr << '\n';
				assert(take[r0][0] && !take[r0][1]);
				assert(take[r1][1] && !take[r1][0]);
				assert(!take[a][0] && !take[a][1]);
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
				assert(take[r][0] && take[r][1]);
				assert(!take[a0][0] && take[a0][1]);
				assert(!take[a1][1] && take[a1][0]);
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
		assert(int(remove_both.size()) == prv_has_both+1);
	}

	int64_t ans = 0;
	int cnt_both = 0;
	array<int, 2> cnts{0,0};
	for (int i = 0; i < N; i++) {
		cnt_both += take[i][0] && take[i][1];
		for (int z = 0; z < 2; z++) {
			cnts[z] += take[i][z];
			if (take[i][z]) {
				//cerr << i << ' ' << z << ' ' << A[i][z] << '\n';
				ans += A[i][z];
			}
		}
	}
	assert(cnt_both == int(remove_both.size()));
	assert(cnt_both >= L);
	assert(cnts[0] == K);
	assert(cnts[1] == K);

	return ans;
}

int64_t slow(int N, int K, int L, const std::vector<std::array<int64_t, 2>> A) {
	std::vector<std::pair<int64_t, int>> vals[2];
	for (int z = 0; z < 2; z++) {
		vals[z].resize(N);
		for (int i = 0; i < N; i++) {
			vals[z][i] = {A[i][z], i};
		}
		sort(vals[z].begin(), vals[z].end(), std::greater<>());
	}
	int64_t ans = 0;
	for (int m = 0; m < (1 << N); m++) {
		if (__builtin_popcount(m) != L) continue;
		int64_t cnd = 0;
		for (int i = 0; i < N; i++) {
			if (m & (1 << i)) cnd += A[i][0] + A[i][1];
		}
		for (int z = 0; z < 2; z++) {
			int cnt = K-L;
			for (auto [v, i] : vals[z]) {
				if (!cnt) break;
				if (m & (1 << i)) {
					continue;
				}
				cnd += v;
				--cnt;
			}
		}
		ans = std::max(ans, cnd);
	}
	return ans;
}

void test() {
	using namespace std;
	mt19937 mt(48);
	while (true) {
		int N = 10;
		int K = 7;
		int L = 5;
		vector<array<int64_t, 2>> A(N);
		for (int i = 0; i < N; i++) {
			for (int z = 0; z < 2; z++) {
				A[i][z] = mt() % int(10) + 1;
			}
		}
		int64_t solve_val = solve(N, K, L, A);
		int64_t slow_val = slow(N, K, L, A);
		if (solve_val != slow_val) {
			cerr << N << ' ' << K << ' ' << L << '\n';
			for (int z = 0; z < 2; z++) {
				for (int i = 0; i < N; i++) {
					cerr << A[i][z] << " \n"[i+1==N];
				}
			}
			cerr << "slow_val" << ' ' << slow_val << '\n';
			cerr << "solve_val" << ' ' << solve_val << '\n';
			assert(slow_val == solve_val);
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	//test();
	int T; cin >> T;
	while (T--) {
		int N, K, L; cin >> N >> K >> L;
		vector<array<int64_t, 2>> A(N);
		for (int z = 0; z < 2; z++) {
			for (auto& a : A) cin >> a[z];
		}
		//int64_t slow_ans = slow(N, K, L, A);
		//cerr << "slow " << slow_ans << '\n';
		int64_t ans = solve(N, K, L, A);
		//cerr << "fast " << ans << '\n';
		//assert(slow_ans == ans);
		cout << ans << '\n';
	}

	return 0;
}
