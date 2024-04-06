#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		std::string S; cin >> S;
		[&]() -> void {
			int N = int(S.size());
			auto is_pal = [&](int l, int r) -> bool {
				for (int i = l, j = r-1; i < j; i++, j--) {
					if (S[i] != S[j]) return false;
				}
				return true;
			};

			if (N == 1) {
				cout << "NO" << '\n';
			} else if (S[1] == S[0]) {
				int s = 0;
				while (s < N && S[s] == S[0]) s++;
				if (s == N) {
					cout << "NO" << '\n';
				} else if (!is_pal(0, N)) {
					cout << "YES" << '\n';
					cout << 1 << '\n';
					cout << S << '\n';
				} else if (N == 2*s+1) {
					cout << "NO" << '\n';
				} else if (!is_pal(s+1, N)) {
					cout << "YES" << '\n';
					cout << 2 << '\n';
					cout << S.substr(0, s+1) << ' ' << S.substr(s+1) << '\n';
				} else {
					cout << "YES" << '\n';
					cout << 2 << '\n';
					cout << S.substr(0, s+2) << ' ' << S.substr(s+2) << '\n';
				}
			} else {
				if (!is_pal(0, N)) {
					cout << "YES" << '\n';
					cout << 1 << '\n';
					cout << S << '\n';
				} else if (!is_pal(2, N)) {
					cout << "YES" << '\n';
					cout << 2 << '\n';
					cout << S.substr(0, 2) << ' ' << S.substr(2) << '\n';
				} else {
					cout << "NO" << '\n';
				}
			}
		}();
	}

	return 0;
}
