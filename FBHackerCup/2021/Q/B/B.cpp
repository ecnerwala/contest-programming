#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		std::vector<std::string> G(N);
		for (auto& s : G) { cin >> s; assert(int(s.size()) == N); }

		int min_cnt = N+1;
		int num_ways = 0;
		for (int i = 0; i < N; i++) {
			int cnt_dot = 0;
			bool has_o = false;
			int dot_pos = -1;
			for (int j = 0; j < N; j++) {
				if (G[i][j] == 'X') {
				} else if (G[i][j] == '.') {
					cnt_dot ++;
					dot_pos = j;
				} else {
					has_o = true;
					break;
				}
			}
			if (has_o) continue;
			assert(cnt_dot >= 1);
			if (cnt_dot < min_cnt) min_cnt = cnt_dot, num_ways = 0;
			if (cnt_dot == min_cnt) {
				if (cnt_dot == 1) {
					G[i][dot_pos] = 'z';
				}
				num_ways++;
			}
		}
		for (int j = 0; j < N; j++) {
			int cnt_dot = 0;
			bool has_o = false;
			for (int i = 0; i < N; i++) {
				if (G[i][j] == 'X') {
				} else if (G[i][j] == '.') {
					cnt_dot ++;
				} else {
					has_o = true;
					break;
				}
			}
			if (has_o) continue;
			assert(cnt_dot >= 1);
			if (cnt_dot < min_cnt) min_cnt = cnt_dot, num_ways = 0;
			if (cnt_dot == min_cnt) {
				num_ways++;
			}
		}

		cout << "Case #" << case_num << ": ";
		if (num_ways == 0) {
			cout << "Impossible" << '\n';
		} else {
			cout << min_cnt << ' ' << num_ways << '\n';
		}
	}

	return 0;
}
