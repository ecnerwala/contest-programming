#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	string S; cin >> S;
	int N = int(S.size());
	int K; cin >> K;

	int ans = 0;
	int cur = 0;
	for (int i = 0, j = 0; true; ) {
		if (cur <= K) {
			ans = max(ans, j - i);
			if (j == N) break;
			cur += (S[j] == '.');
			j++;
		} else {
			cur -= (S[i] == '.');
			i++;
		}
	}

	cout << ans << '\n';

	return 0;
}
