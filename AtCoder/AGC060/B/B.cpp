#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, K; string S; cin >> N >> M >> K >> S;
		int need = 0;
		for (int i = 0; i+1 < int(S.size()); i++) {
			if (S[i] != S[i+1]) {
				need++;
				i++;
			}
		}
		cout << (need <= K ? "Yes" : "No") << '\n';
	}

	return 0;
}
