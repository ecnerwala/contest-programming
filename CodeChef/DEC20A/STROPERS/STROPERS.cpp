#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		string S; cin >> S;
		int N = int(S.size());
		set<pair<array<int, 2>, int>> seen;
		for (int i = 0; i < N; i++) {
			array<int, 2> c0{0,0};
			int c1 = 0;
			for (int j = i; j < N; j++) {
				if (S[j] == '0') c0[c1&1]++;
				else if (S[j] == '1') c1++;
				else assert(false);
				seen.insert({c0,c1});
			}
		}
		cout << seen.size() << '\n';
	}

	return 0;
}
