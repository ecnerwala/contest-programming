#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		std::string S; cin >> S;
		assert(int(S.size()) == N);
		bool alice_wins = [&]() -> bool {
			if (S.back() == 'A') return true;
			int val = 0;
			for (int i = N-2; i >= 0; i--) {
				if (S[i] == S[i+1]) {
					if (S[i] == 'B') {
						val++;
					} else if (S[i] == 'A') {
						val--;
						if (val == 0) return true;
					} else assert(false);
				}
			}
			return false;
		}();

		cout << "Case #" << case_num << ": " << (alice_wins ? "Alice" : "Bob") << '\n';
	}

	return 0;
}

// If the far end is the current player, then they win
