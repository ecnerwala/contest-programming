#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int mx = 0;
		int g = 0;
		while (N--) {
			int a; cin >> a;
			g = std::gcd(g, a);
			mx = std::max(mx, a);
		}
		cout << mx / g << '\n';
	}

	return 0;
}
