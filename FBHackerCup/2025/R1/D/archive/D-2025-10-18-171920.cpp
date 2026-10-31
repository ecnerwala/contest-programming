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
			int val = 0;
			for (int i = N-1; i >= 0; i--) {
				if (S[i] == 'A') {
					if (!val) return true;
					val--;
				} else {
					val++;
				}
			}
			return false;
		}();

		cout << "Case #" << case_num << ": " << (alice_wins ? "Alice" : "Bob") << '\n';
	}

	return 0;
}

// If the far end is the current player, then they win
