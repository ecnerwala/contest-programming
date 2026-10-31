#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int Q; cin >> Q;
	while (Q--) {
		std::string S; cin >> S;
		std::reverse(S.begin(), S.end());
		int N = int(S.size());
		bool good = false;
		std::vector<int> fail(N);
		fail[0] = -1;
		for (int i = 0; i < N-1; i++) {
			int cur = fail[i];
			while (cur >= 0 && S[i] != S[cur]) {
				cur = fail[cur];
			}
			fail[i+1] = cur + 1;
			if (fail[i+1] * 2 >= (i+1)) {
				good = true;
				break;
			}
		}
		cout << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
