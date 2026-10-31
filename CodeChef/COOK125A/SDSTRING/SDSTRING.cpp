#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		string S; cin >> S;
		cout << [&]() -> int {
			int N = int(S.size());
			if (N % 2 != 0) return -1;
			int cnt[2] = {0,0};
			for (char c : S) cnt[c - '0']++;
			if (min(cnt[0], cnt[1]) == 0) return -1;
			return abs(cnt[0] - cnt[1]) / 2;
		}() << '\n';
	}

	return 0;
}
