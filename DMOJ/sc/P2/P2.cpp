#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::string S; cin >> S;
	if (S[0] != S[N-1]) {
		cout << N << '\n';
	} else {
		// number of distinct runs
		int cnt = 0;
		for (int i = 0; i+1 < N; i++) {
			if (S[i] != S[i+1]) cnt++;
		}
		cout << std::max(cnt, 1) << '\n';
	}

	return 0;
}
