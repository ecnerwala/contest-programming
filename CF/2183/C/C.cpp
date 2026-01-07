#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, K; cin >> N >> M >> K;
		M++;
		// we get M+1 soldiers
		cout << std::min(2*M/3 + 1, N) << '\n';
	}

	return 0;
}
