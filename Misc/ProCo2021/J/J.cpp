#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	while (N--) {
		int64_t a, b, c; cin >> a >> b >> c;
		cout << (2 * max({a, b, c}) < a + b + c ? "Yes" : "No") << '\n';
	}

	return 0;
}
