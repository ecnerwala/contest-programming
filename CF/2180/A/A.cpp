#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int L, A, B; cin >> L >> A >> B;
		int g = std::gcd(L, B);
		cout << L - g + (A % g) << '\n';
	}

	return 0;
}
