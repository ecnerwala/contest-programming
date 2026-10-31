#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int R, C, A, B; cin >> R >> C >> A >> B;


		cout << "Case #" << case_num << ": ";
		cout << (R > C ? "YES" : "NO") << '\n';
	}

	return 0;
}
