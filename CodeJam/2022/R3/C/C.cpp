#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		const string CHARS = "ACDEHIJKMORST";

		int N; cin >> N;
		std::vector<std::array<int, 2>> ch(N);
		for (int z = 0; z < 2; z++) {
			for (int i = 0; i < N; i++) {
				cin >> ch[i][z];
				ch[i][z]--;
			}
		}

		auto output = [&]() -> std::optional<string> {
			std::vector<std::vector<int>> edges(N);
			std::vector<int> deg(N);
			for (int i = 0; i < N; i++) {
				for (int a = 0; a < 2; a++) {
					int j = ch[i][a];
					if (i == j) return std::nullopt;
					edges[i].push_back(j);
					edges[j].push_back(i);
					deg[i]++;
					deg[j]++;
					for (int b = 0; b < 2; b++) {
						int k = ch[j][b];
						if (i == k) return std::nullopt;
						edges[i].push_back(k);
						edges[k].push_back(i);
						deg[i]++;
						deg[k]++;
					}
				}
			}
			std::vector<int> ordering; ordering.reserve(N);
			for (int i = 0; i < N; i++) {
				if (deg[i] <= 12) {
					ordering.push_back(i);
				}
			}

			for (int z = 0; z < int(ordering.size()); z++) {
				int cur = ordering[z];
				for (int nxt : edges[cur]) {
					deg[nxt]--;
					if (deg[nxt] == 12) {
						ordering.push_back(nxt);
					}
				}
			}
			assert(int(ordering.size()) == N);
			std::vector<int> res(N, -1);
			for (int z = N-1; z >= 0; z--) {
				int cur = ordering[z];
				int msk = (1 << 13) - 1;
				for (int nxt : edges[cur]) {
					if (res[nxt] == -1) continue;
					msk &= ~(1 << res[nxt]);
				}
				assert(msk);
				res[cur] = __builtin_ctz(msk);
			}
			std::string ans(N, '\0');
			for (int i = 0; i < N; i++) {
				ans[i] = CHARS[res[i]];
			}
			return ans;
		}();

		cout << "Case #" << case_num << ": " << (output ? *output : "IMPOSSIBLE") << endl;
	}

	return 0;
}
