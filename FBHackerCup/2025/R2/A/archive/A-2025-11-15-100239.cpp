#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, M; cin >> N >> M;
		bool good = N >= M && (N <= 2 * M - 2 || N % 2 == 0);

		cout << "Case #" << case_num << ": " << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
