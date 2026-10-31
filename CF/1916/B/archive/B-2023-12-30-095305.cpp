#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int A, B; cin >> A >> B;
		if (B % A == 0) {
			cout << B * (B / A) << '\n';
		} else {
			cout << std::lcm(A, B) << '\n';
		}
	}

	return 0;
}
