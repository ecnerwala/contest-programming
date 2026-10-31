#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, K; cin >> N >> K;
		std::vector<int> S(N);
		for (auto& s : S) cin >> s;

		auto is_good = [&]() -> bool {
			std::map<int, int> cnt;
			for (auto s : S) {
				cnt[s]++;
			}
			int num_2 = 0;
			int num_1 = 0;
			for (auto [_, c] : cnt) {
				if (c > 2) {
					return false;
				} else if (c == 2) {
					num_2++;
				} else if (c == 1) {
					num_1++;
				} else assert(false);
			}
			return num_2 + (num_1 + 1) / 2 <= K;
		}();

		cout << "Case #" << case_num << ": " << (is_good ? "YES" : "NO") << '\n';
	}

	return 0;
}
