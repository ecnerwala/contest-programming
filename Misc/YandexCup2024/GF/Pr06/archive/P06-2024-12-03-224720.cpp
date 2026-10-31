#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	if (N * N % K == 0) {
		cout << "Yes" << '\n';
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cout << 1 + (i+j)%K << " \n"[j+1==N];
			}
		}
	} else {
		cout << "No" << '\n';
	}

	return 0;
}
