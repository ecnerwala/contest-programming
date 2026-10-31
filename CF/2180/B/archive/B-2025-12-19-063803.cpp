#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::string s;
		for (int i = 0; i < N; i++) {
			std::string a; cin >> a;
			s = std::min(s + a, a + s);
		}
		cout << s << '\n';
	}

	return 0;
}
