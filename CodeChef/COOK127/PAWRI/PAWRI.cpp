#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		string s; cin >> s;
		for (int i = 0; i+5 <= int(s.size()); i++) {
			if (s[i] == 'p' && s[i+1] == 'a' && s[i+2] == 'r' && s[i+3] == 't' && s[i+4] == 'y') {
				s[i+2] = 'w';
				s[i+3] = 'r';
				s[i+4] = 'i';
			}
		}
		cout << s << '\n';
	}

	return 0;
}
