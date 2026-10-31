#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::string S; cin >> S;
	assert(int(S.size()) == N+1);

	std::vector<bool> taken(N+1);
	int ai = 0;
	int bi = 0;
	int ans = 0;
	for (int i = 0; i < N; i++) {
		if (i & 1) {
			while (bi <= N && S[bi] != 'A') bi++;
			if (bi <= N) {
				taken[bi] = true;
				bi++;
			}
		} else {
			while (ai <= N && S[ai] != 'B') ai++;
			if (ai <= N) {
				taken[ai] = true;
				ai++;
			}
		}
		while (taken[ans]) ans++;
		cout << (S[ans] == 'A' ? "Alice" : "Bob") << '\n';
	}

	return 0;
}
