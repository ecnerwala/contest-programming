#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;
template <typename T> using max_pq = std::priority_queue<T, std::vector<T>, std::less<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int V, M; cin >> V >> M;
		std::vector<int> P(V);
		for (auto& p : P) cin >> p;
		std::vector<int> D(V);
		for (auto& d : D) cin >> d;

		std::vector<int> X(M);
		for (auto& x : X) cin >> x;

		min_pq<std::pair<int, int>> right_checks;
		max_pq<std::pair<int, int>> left_checks;
		for (int i = 0; i < V; i++) {
			right_checks.push({P[i], ~i});
		}

		std::vector<int> ans; ans.reserve(M);
		int64_t pos = 0;
		for (auto x : X) {
			int cur_ans = 0;
			pos += x;
			if (x > 0) {
				while (!right_checks.empty() && pos >= right_checks.top().first) {
					int i = right_checks.top().second;
					right_checks.pop();
					if (i < 0) {
						i = ~i;
						if (D[i] >= P[i]) {
							right_checks.push({D[i], i});
						} else {
							left_checks.push({D[i], i});
						}
					} else {
						cur_ans++;
					}
				}
			} else {
				while (!left_checks.empty() && pos <= left_checks.top().first) {
					int i = left_checks.top().second;
					left_checks.pop();
					assert(i >= 0);
					cur_ans++;
				}
			}
			ans.push_back(cur_ans);
		}


		cout << "Case #" << case_num << ": ";
		for (int q = 0; q < M; q++) {
			cout << ans[q] << " \n"[q+1==M];
		}
	}

	return 0;
}
