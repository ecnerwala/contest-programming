#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {

		int N, K; cin >> N >> K;
		std::vector<std::string> S(N);
		int tot_len = 0;
		for (auto& s : S) { cin >> s; tot_len += int(s.size()); }

		std::vector<std::array<int, 26>> nodes; nodes.reserve(tot_len+1);
		std::vector<int> is_string; is_string.reserve(tot_len+1);
		std::vector<int> cnt_string; cnt_string.reserve(tot_len+1);
		nodes.push_back({});
		nodes.back().fill(-1);
		is_string.push_back(0);
		cnt_string.push_back(0);

		for (const auto& s : S) {
			int cur = 0;
			cnt_string[cur]++;
			for (char c : s) {
				c -= 'a';
				if (nodes[cur][c] == -1) {
					nodes[cur][c] = int(nodes.size());
					nodes.push_back({});
					nodes.back().fill(-1);
					is_string.push_back(0);
					cnt_string.push_back(0);
				}
				cur = nodes[cur][c];
				cnt_string[cur]++;
			}
			is_string[cur]++;
		}

		int cur = 0;
		int cur_cnt = 0;
		std::string ans; ans.reserve(tot_len);
		while (true) {
			cur_cnt += is_string[cur];
			for (int v : nodes[cur]) {
				if (v != -1) {
					cur_cnt ++;
				}
			}
			if (cur_cnt >= K) {
				break;
			}
			for (int c = 0; c < 26; c++) {
				int v = nodes[cur][c];
				if (v == -1) continue;
				if (cur_cnt - 1 + cnt_string[v] >= K) {
					cur = v;
					ans += char('a' + c);
					cur_cnt--;
					goto done;
				} else {
					cur_cnt += cnt_string[v] - 1;
				}
			}
			assert(false);
done:
			;
		}
		if (ans.empty()) cout << "EMPTY" << '\n';
		else cout << ans << '\n';
	}

	return 0;
}
