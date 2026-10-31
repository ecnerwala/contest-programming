#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		std::vector<int> S(N);
		for (auto& s : S) cin >> s;
		S.erase(std::unique(S.begin(), S.end()), S.end());
		auto V = S;
		std::sort(V.begin(), V.end());
		bool good = std::unique(V.begin(), V.end()) == V.end();

		cout << "Case #" << case_num << ": ";
		if (good) {
			for (int i = 0; i < int(S.size()); i++) {
				if (i) cout << ' ';
				cout << S[i];
			}
			cout << '\n';
		} else {
			cout << "IMPOSSIBLE" << '\n';
		}
	}

	return 0;
}
