#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		std::array<char, 26> D;
		for (auto& d : D) cin >> d;
		int N; cin >> N;
		std::vector<std::string> S(N);
		for (auto& s : S) {
			cin >> s;
			for (auto& c : s) c = D[c - 'A'];
		}
		std::sort(S.begin(), S.end());
		bool has_repeat = std::unique(S.begin(), S.end()) != S.end();

		cout << "Case #" << case_num << ": " << (has_repeat ? "YES" : "NO") << '\n';
	}

	return 0;
}
