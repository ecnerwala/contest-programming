#include <bits/stdc++.h>


int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto solve_single = [&](std::vector<int> B) -> std::pair<int, int> {
		int tot_or = 0;
		int best_or = 0;
		for (int i : B) {
			if (i == 0) continue;
			tot_or |= i;
			while (i & (i-1)) {
				i &= (i-1);
			}
			best_or |= i ^ (i-1);
		}
		if (tot_or == best_or) {
			return {best_or, 0};
		}
		assert(best_or >= 1);
		for (int j = 0; j < int(B.size()); j++) {
			int old_val = B[j];
			for (int i = 0; (1 << i) <= B[j]; i++) {
				B[j] = B[j] ^ (B[j] - (1 << i));
				int cnd = 0;
				for (int z = 0; z < int(B.size()); z++) {
					cnd |= B[z];
				}
				if (cnd == best_or) {
					return {best_or, 1};
				}
				B[j] = old_val;
			}
		}
		return {best_or, 2};
	};

	auto solve_slow = [&](std::vector<int> A, std::vector<std::array<int, 2>> queries) {
		int N = int(A.size());
		int Q = int(queries.size());
		std::vector<std::pair<int, int>> ans(Q, {-1, -1});
		for (int q = 0; q < Q; q++) {
			auto [l, r] = queries[q];
			ans[q] = solve_single(std::vector<int>(A.begin()+l, A.begin()+r));
		}
		return ans;
	};

	auto solve = [&](std::vector<int> A, std::vector<std::array<int, 2>> queries) {
		int N = int(A.size());
		int Q = int(queries.size());

		const int B = 30;

		std::vector<std::array<std::pair<int, int>, B>> pref(N+1);
		for (int b = 0; b < 30; b++) {
			pref[0][b] = {0, 0};
		}
		for (int i = 0; i < N; i++) {
			pref[i+1] = pref[i];
			for (int b = 0; b < 30; b++) {
				if ((A[i] >> b) & 1) {
					pref[i+1][b].first++;
					pref[i+1][b].second ^= i;
				}
			}
		}

		std::vector<std::pair<int, int>> ans(Q, {-1, -1});
		std::vector<int> single_locs; single_locs.reserve(B);
		std::array<std::array<std::vector<std::pair<int, int>>, B>, B> need_count;
		for (int q = 0; q < Q; q++) {
			auto [l, r] = queries[q];
			int top_bit = -1;
			int first_miss = -1;
			int last_miss = -1;
			single_locs.clear();
			int single_bits = 0;
			for (int b = B-1; b >= 0; b--) {
				int cnt = pref[r][b].first - pref[l][b].first;
				if (top_bit == -1) {
					if (cnt == 0) continue;
					top_bit = b;
				}

				if (cnt == 0) {
					if (first_miss == -1) first_miss = b;
					last_miss = b;
				}
				if (cnt == 1) {
					int loc = pref[r][b].second ^ pref[l][b].second;
					single_locs.push_back(loc);
					single_bits |= (1 << b);
				}
			}
			ans[q].first = (1 << (top_bit+1)) - 1;
			ans[q].second = -1;
			if (first_miss == -1) {
				ans[q].second = 0;
				continue;
			}

			int miss_mask = (1 << (first_miss + 1)) - (1 << last_miss);

			std::sort(single_locs.begin(), single_locs.end());
			single_locs.resize(std::unique(single_locs.begin(), single_locs.end()) - single_locs.begin());

			int cnt = 0;
			for (int i : single_locs) {
				if (A[i] < miss_mask) continue;
				if (A[i] & miss_mask) continue;
				int delta = (A[i] - (1 << last_miss));
				assert((delta & miss_mask) == miss_mask);
				if (single_bits & A[i] & ~(A[i] ^ delta)) {
					cnt++;
				} else {
					ans[q].second = 1;
					goto done;
				}
			}
			assert(ans[q].second == -1);
			need_count[last_miss][first_miss].push_back({cnt, q});
done: ;
		}

		std::vector<int> cur_pref(N+1);
		for (int last_miss = 0; last_miss < B; last_miss++) {
			for (int first_miss = last_miss; first_miss < B; first_miss++) {
				cur_pref[0] = 0;
				int miss_mask = (1 << (first_miss + 1)) - (1 << last_miss);
				for (int i = 0; i < N; i++) {
					cur_pref[i+1] = cur_pref[i];
					if (A[i] < miss_mask) continue;
					if (A[i] & miss_mask) continue;
					cur_pref[i+1]++;
				}
				for (auto [cnt, q] : need_count[last_miss][first_miss]) {
					auto [l, r] = queries[q];
					assert(ans[q].second == -1);
					if (cur_pref[r] - cur_pref[l] > cnt) {
						ans[q].second = 1;
					} else {
						ans[q].second = 2;
					}
				}
			}
		}
		for (auto [v, c] : ans) {
			assert(c != -1);
		}
		return ans;
	};

	std::mt19937 mt(48);
	for (int z = 0; z < 0; z++) {
		cerr << "Testing " << '\n';
		int N = 10;
		std::vector<int> A(N);
		std::uniform_int_distribution<int> dist(0, int(100));
		for (auto& a : A) a = dist(mt);
		std::vector<std::array<int, 2>> queries;
		for (int i = 0; i < N; i++) {
			for (int j = i+1; j <= N; j++) {
				queries.push_back({i, j});
			}
		}
		auto res = solve(A, queries);
		auto slo = solve_slow(A, queries);
		if (res != slo) {
			cerr << "FAIL" << '\n';
			cerr << N << ' ' << queries.size() << '\n';
			for (auto& a : A) { cerr << a << ' '; } cerr << '\n';
			for (int q = 0; q < int(queries.size()); q++) {
				if (res[q] != slo[q]) {
					cerr << queries[q][0] << ' ' << queries[q][1] << ' ' << res[q].first << ' ' << res[q].second << ' ' << slo[q].first << ' ' << slo[q].second << '\n';
				}
			}
			exit(1);
		}
		cerr << "tests passed" << '\n';
	}

	int T; cin >> T;
	while (T--) {
		int N, Q; cin >> N >> Q;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		std::vector<std::array<int, 2>> queries(Q);
		for (auto& [l, r] : queries) {
			cin >> l >> r; l--;
		}

		auto ans = solve(A, queries);

		for (auto [v, c] : ans) {
			assert(c != -1);
			cout << v << ' ' << c << '\n';
		}
	}

	return 0;
}
