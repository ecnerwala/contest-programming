#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::string S; cin >> S;
		std::reverse(S.begin(), S.end());
		int lo = 1, hi = N;
		std::vector<int> ans; ans.reserve(N);
		for (char c : S) {
			if (c == '>') {
				ans.push_back(hi--);
			} else if (c == '<') {
				ans.push_back(lo++);
			} else assert(false);
		}
		ans.push_back(lo);
		std::reverse(ans.begin(), ans.end());
		for (int i = 0; i < N; i++) {
			cout << ans[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
