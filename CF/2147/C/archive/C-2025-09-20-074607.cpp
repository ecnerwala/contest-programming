#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::string S; cin >> S;
		bool good = true;
		// if the alternating length is 3 mod 4 and starts with 1 then it's bad
		for (int i = 0, j = i; i < N; i = j) {
			j = i+1;
			while (j < N && S[j] != S[j-1]) j++;
			if (S[i] == '1' && (j - i) % 4 == 3) {
				good = false;
			}
		}
		cout << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
