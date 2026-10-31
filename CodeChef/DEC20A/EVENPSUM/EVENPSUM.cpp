#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t A, B; cin >> A >> B;
		cout << (A/2) * (B/2) + (A-A/2) * (B-B/2) << '\n';
	}

	return 0;
}
