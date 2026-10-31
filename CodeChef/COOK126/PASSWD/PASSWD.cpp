#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		string S; cin >> S;
		int N = int(S.size());
		bool r1 = false, r2 = false, r3 = false, r4 = false, r5 = (N >= 10);
		for (int i = 0; i < N; i++) {
			if ('a' <= S[i] && S[i] <= 'z') r1 = true;
		}
		for (int i = 1; i+1 < N; i++) {
			char c = S[i];
			if ('A' <= c && c <= 'Z') r2 = true;
			if ('0' <= c && c <= '9') r3 = true;
			if (c == '@' || c == '#' || c == '%' || c == '&' || c == '?') r4 = true;
		}
		cout << (r1&&r2&&r3&&r4&&r5 ? "YES" : "NO") << '\n';
	}

	return 0;
}
