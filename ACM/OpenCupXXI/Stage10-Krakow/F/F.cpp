#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; char c; cin >> N >> c;
		string S, T; cin >> S >> T;
		if (S >= T) goto bad;
		for (int z = 0; z < 26; z++) {
			// increment S
			for (int a = N-1; true; a--) {
				if (S[a] == 'z') S[a] = 'a';
				else {
					S[a]++;
					break;
				}
			}
			if (S == T) goto bad;
			if (S.find(c) != std::string::npos) {
				cout << S << '\n';
				goto found;
			}
		}
bad: cout << "NO" << '\n';
found: ;
	}

	return 0;
}
