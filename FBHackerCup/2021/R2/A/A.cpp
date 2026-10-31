#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, M; cin >> N >> M;

		int ans = 0;
		std::vector<std::pair<int, bool>> cur(M);
		for (auto& v : cur) { cin >> v.first; v.second = true; }

		std::sort(cur.begin(), cur.end());

		std::vector<std::pair<int, bool>> nxt(M);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) { cin >> nxt[j].first; nxt[j].second = false; }
			std::sort(nxt.begin(), nxt.end());

			for (int j = 0, k = 0; j < M; j++) {
				while (k < M && cur[k].first < nxt[j].first) {
					k++;
				}
				if (k < M && cur[k].first == nxt[j].first) {
					nxt[j].second = cur[k].second;
					k++;
				} else {
					nxt[j].second = false;
					ans++;
				}
			}
			std::sort(nxt.begin(), nxt.end());
			std::swap(cur, nxt);
		}

		for (auto it : cur) {
			ans -= !it.second;
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
