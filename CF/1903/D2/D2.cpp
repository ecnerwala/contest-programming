#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	std::vector<int> A(N);
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}
	std::sort(A.begin(), A.end());

	std::vector<std::pair<int64_t, int>> queries(Q);
	for (int q = 0; q < Q; q++) {
		cin >> queries[q].first; queries[q].second = q;
	}
	std::sort(queries.begin(), queries.end());
	std::vector<int64_t> ans(Q);

	std::vector<int64_t> cnt_A(1 << 20);
	for (int a : A) {
		 cin >> a;
	}
	
	std::vector<std::vector<std::pair<int, int64_t>>> cnts;
	for (int b = 0; b < 20; b++) {
		int l = 20 - 1 - b;
		cnts[b].resize(1 << (20 - 1 - b));
		for (int a : A) {
			if (a & (1 << b)) continue;
			cnts[b][a >> (b+1)].first++;
			cnts[b][a >> (b+1)].second += a & ((1 << b) - 1);
		}
		for (int m = 0; m < (20 - 1 - b); m++) {
			for (int x = 0; x < (1 << (20 - 1 - b)); x++) {
				if (x & (1 << m)) continue;
				cnts[b][x].first += cnts[b][x + (1 << m)].first;
				cnts[b][x].second += cnts[b][x + (1 << m)].second;
			}
		}
	}

	int64_t tot_A = 0;
	for (int i = 0; i < N; i++) {
		tot_A += A[i];
	}
	
	auto solve_query = [&](int64_t k) -> int64_t {
		if (k + tot_A >= (int64_t(N) << 20)) {
			return (k + tot_A) / N;
		}
		int cur_ans = 0;
		int cur_bigger = 0;
		for (int b = 19; b >= 0; b--) {
			auto [new_cnt, new_sum] = cnts[b][cur_ans >> (b+1)];
			int64_t cnd_ops = (int64_t(cur_bigger + new_cnt) << b) - new_sum;
			if (k >= cnd_ops) {
				k -= cnd_ops;
				cur_ans += (1 << b);
				cur_bigger += new_cnt;
			}
		}
		return cur_ans;
	};

	for (auto [k, q] : queries) {
		ans[q] = solve_query(k);
	}
	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}

	return 0;
}
