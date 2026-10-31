#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int c[2] = {0, 0};
		while (N--) {
			char v; cin >> v;
			c[v&1]++;
		}
		cout << std::max(std::min(c[0], c[1]), 1) - 1 << '\n';
	}

	return 0;
}
