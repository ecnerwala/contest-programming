#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, A, B; cin >> N >> A >> B;
		if (A == N && B == N) {
			cout << "Yes" << '\n';
		} else if (A + B + 2 <= N) {
			cout << "Yes" << '\n';
		} else {
			cout << "No" << '\n';
		}
	}

	return 0;
}
