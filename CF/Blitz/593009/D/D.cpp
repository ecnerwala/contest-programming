#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	assert(N & 1);

	cout << "YES" << '\n';
	cout << std::string(N, '.') << '\n';
	if (K & 1) {
		std::array<std::string, 2> S;
		S[0] = std::string(N, '.');
		S[1] = std::string(N, '.');
		for (int z = 0; z < 2; z++) {
			for (int i = 1; i < N-1-i; i++) {
				if (K >= 2) {
					K -= 2;
					S[z][i] = S[z][N-1-i] = '#';
				}
			}
		}
		assert(K == 1);
		S[0][N/2] = '#';
		cout << S[0] << '\n';
		cout << S[1] << '\n';
	} else {
		for (int z = 0; z < 2; z++) {
			for (int i = 0; i < N; i++) {
				if (i >= 1 && i <= K/2) {
					cout << '#';
				} else {
					cout << '.';
				}
			}
			cout << '\n';
		}
	}
	cout << std::string(N, '.') << '\n';

	return 0;
}
