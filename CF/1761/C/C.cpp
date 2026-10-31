#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::string> B(N);
		for (auto& s : B) cin >> s;
		for (int i = 0; i < N; i++) {
			B[i][i] = '1';
			int cnt = 0;
			for (int j = 0; j < N; j++) {
				if (B[j][i] == '1') cnt++;
			}
			cout << cnt;
			for (int j = 0; j < N; j++) {
				if (B[j][i] == '1') cout << ' ' << j+1;
			}
			cout << '\n';
		}
	}

	return 0;
}
