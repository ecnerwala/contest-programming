#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::string S; cin >> S;
		int r = 0;
		for (char c : S) {
			if (c == '+') r++;
			else if (c == '-') r--;
			else assert(false);
		}
		cout << std::abs(r) << '\n';
	}

	return 0;
}
