#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;

	std::array<int, 26> dp; dp.fill(-1);
	for (int i = 0; i < N; i++) {
		string s; cin >> s;
		int st = std::max(dp[s[0] - 'a'] + 1, *std::max_element(dp.begin(), dp.end()));
		for (int z = 0; z+1 < int(s.size()); z++) {
			st += (s[z+1] == s[z]);
		}
		dp[s.back()-'a'] = std::max(dp[s.back()-'a'], st);
	}
	cout << *std::max_element(dp.begin(), dp.end()) << '\n';

	return 0;
}
