#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		string S; cin >> S;
		array<int, 2> cnt{0,0};
		for (char c : S) cnt[c-'A']++;
		bool ans = abs(cnt[0] - cnt[1]) == 1;

		cout << "Case #" << case_num << ": " << (ans ? 'Y' : 'N') << '\n';
	}

	return 0;
}
