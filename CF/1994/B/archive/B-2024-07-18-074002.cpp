#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int Q; cin >> Q;
	while (Q--) {
		int N; cin >> N;
		std::string S, T; cin >> S >> T;
		bool good = true;
		for (int i = 0; i < N; i++) {
			if (S[i] == '1') break;
			if (T[i] == '1') {
				good = false;
				break;
			}
		}
		cout << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
