#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		std::string S; cin >> S;
		cout << [&]() -> std::string {
			for (int i = 0; i+1 < int(S.size()); i++) {
				if (S[i] == S[i+1]) return S.substr(i, 2);
			}
			for (int i = 0; i+2 < int(S.size()); i++) {
				if (S[i] != S[i+2]) return S.substr(i, 3);
			}
			return "-1";
		}() << '\n';
	}

	return 0;
}

// 3 distinct characters => abc, ab, bc, a, b, c
