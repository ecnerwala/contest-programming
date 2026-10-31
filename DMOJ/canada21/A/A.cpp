#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		string S; cin >> S;
		int v0 = 0;
		int v1 = N+1;
		int v2 = N+1;
		int v3 = N+1;
		for (char c : S) {
			v3 = min(v3 + (c != 'R'), v2 + (c != 'R'));
			v2 = min(v2 + (c != 'W'), v1 + (c != 'W'));
			v1 = min(v1 + (c != 'R'), v0 + (c != 'R'));
			v0 = N+1;
		}
		cout << v3 << '\n';
	}

	return 0;
}
