#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int A, B, C; cin >> A >> B >> C;
		C %= 2;
		A += C;
		if (A > B) {
			cout << "First" << '\n';
		} else {
			cout << "Second" << '\n';
		}
	}

	return 0;
}
