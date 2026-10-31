#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N, A, B; cin >> N >> A >> B;
		assert(1 <= A);

		cout << "Case #" << case_num << ": ";
		for (int i = 1; i <= 2*N-1; i++) cout << 1 << ' ';
		cout << B << '\n';
	}

	return 0;
}
