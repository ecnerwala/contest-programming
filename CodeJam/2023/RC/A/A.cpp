#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int P; cin >> P;
		std::vector<std::string> S(P);
		for (auto& s : S) cin >> s;

		auto solve = [&]() -> bool {
			std::string prv;
			for (auto& cur : S) {
				std::array<int, 26> cnts; cnts.fill(0);
				for (char c : cur) cnts[c - 'A']++;
				cur.clear();

				int i = 0;
				while (i < int(prv.size()) && cnts[prv[i] - 'A'] > 0) {
					cur.push_back(prv[i]);
					cnts[prv[i] - 'A']--;
					i++;
				}
				if (i < int(prv.size())) {
					int best = -1;
					for (int v = 0; v < 26; v++) {
						if (cnts[v] > 0) best = v;
					}
					while (best <= prv[i] - 'A') {
						i--;
						if (i < 0) return false;
						assert(cur.back() == prv[i]);
						cur.pop_back();

						cnts[prv[i] - 'A']++;
						best = std::max(best, prv[i] - 'A');
					}
					for (int v = (prv[i] - 'A') + 1; true; v++) {
						assert(v < 26);
						if (cnts[v] > 0) {
							cur.push_back(char(v + 'A'));
							cnts[v]--;
							break;
						}
					}
				}
				for (int v = 0; v < 26; v++) {
					while (cnts[v] > 0) {
						cur.push_back(char(v + 'A'));
						cnts[v]--;
					}
				}

				prv = cur;
			}
			return true;
		};

		bool ans = solve();
		cout << "Case #" << case_num << ": ";
		if (ans) {
			cout << "POSSIBLE" << '\n';
			for (int i = 0; i < P; i++) {
				cout << S[i] << " \n"[i+1==P];
			}
		} else {
			cout << "IMPOSSIBLE" << '\n';
		}
	}

	return 0;
}
