#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int S, D, K; cin >> S >> D >> K;

		bool is_good = (S + 2 * D >= K) && (2 * S + 2 * D >= K+1);

		cout << "Case #" << case_num << ": " << (is_good ? "YES" : "NO") << "\n";
	}

	return 0;
}
