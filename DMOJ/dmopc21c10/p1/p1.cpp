#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		string S; cin >> S;
		cout << ([&]() -> bool {
			for (int i = 0; i < int(S.size()); i++) {
				char c = S[i];
				if (c == 'a' || c == 'i' || c == 'u' || c == 'e' || c == 'o') continue;

				i++;
				if (i == int(S.size())) return false;
				char d = S[i];

				if (!(d == 'a' || d == 'i' || d == 'u' || d == 'e' || d == 'o')) {
					return false;
				}

				if (!(c == 'k' || c == 'n' || c == 'h' || c == 'm' || c == 'r' || c == 'f')) return false;

				if (c == 'h' && d == 'u') return false;
				if (c == 'f' && d != 'u') return false;
			}
			return true;
		}() ? "YES" : "NO") << '\n';
	}

	return 0;
}
