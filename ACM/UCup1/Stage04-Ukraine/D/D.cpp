#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::string> S(N);
		int num_e = 0;
		for (int i = 0; i < N; i++) {
			cin >> S[i];
			for (int j = i+1; j < N; j++) {
				if (S[i][j] == '1') num_e++;
			}
		}

		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N; j++) {
				if (S[i][j] == '0') {
					bool found = false;
					for (int k = 0; k < N; k++) {
						if (S[i][k] == '1' && S[j][k] == '1') {
							found = true;
							break;
						}
					}
					if (!found) {
						goto bad;
					}
				}
			}
		}
		cout << "YES" << '\n';
		cout << num_e << '\n';
		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N; j++) {
				if (S[i][j] == '1') cout << i+1 << ' ' << j+1 << '\n';
			}
		}
		continue;
bad:
		cout << "NO" << '\n';
	}

	return 0;
}
