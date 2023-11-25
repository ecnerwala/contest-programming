#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::string S; cin >> S;
		int st = 0;
		while (st < N && S[st] == 'B') st++;
		int en = N;
		while (en > 0 && S[en-1] == 'A') en--;
		cout << std::max(en - st - 1, 0) << '\n';
	}

	return 0;
}
