#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::string S; cin >> S;
		assert(int(S.size()) == N);
		cout << ([&]() -> bool {
			int cnt = 0;
			for (int i = 0; i < N; i++) {
				if (S[i] == '1') cnt++;
			}
			if (cnt & 1) return false;
			if (cnt != 2) return true;
			if (cnt == 2) {
				for (int i = 0; i+1 < N; i++) {
					if (S[i] == '1' && S[i+1] == '1') return false;
				}
				return true;
			}
		}() ? "YES" : "NO") << '\n';
	}

	return 0;
}
