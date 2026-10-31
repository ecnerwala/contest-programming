#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		std::string S; cin >> S;
		int N = int(S.size());
		bool is_pal = true;
		for (int i = 0; i < N-1-i; i++) {
			if (S[i] != S[N-1-i]) {
				is_pal = false;
				break;
			}
		}

		if (!is_pal) {
			cout << "YES" << '\n';
			cout << 1 << '\n';
			cout << S << '\n';
		} else {
			int s = 0;
			while (s < N/2 && S[s] == S[0]) s++;
			if (s == N/2) {
				cout << "NO" << '\n';
			} else if (S[2*s+1] != S[s]) {
				cout << "YES" << '\n';
				cout << 2 << '\n';
				std::string_view V(S);
				cout << V.substr(0, s+1) << ' ' << V.substr(s+1) << '\n';
			} else if (s == 1) {
				bool alt = true;
				for (int i = (N-1)/2; i > 0; i--) {
					if (S[i] != S[i&1]) {
						alt = false;
						break;
					}
				}
				if (!alt) {
					cout << "YES" << '\n';
					cout << 2 << '\n';
					std::string_view V(S);
					cout << V.substr(0, 2) << ' ' << V.substr(2) << '\n';
				} else {
					cout << "NO" << '\n';
				}
			} else {
				cout << "YES" << '\n';
				cout << 2 << '\n';
				std::string_view V(S);
				cout << V.substr(0, s+2) << ' ' << V.substr(s+2) << '\n';
			}
		}
	}

	return 0;
}
