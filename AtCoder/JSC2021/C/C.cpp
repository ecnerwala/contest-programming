#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int A, B; cin >> A >> B;
	A--;
	for (int v = B; v >= 1; v--) {
		if (B / v - A / v >= 2) {
			cout << v << '\n';
			exit(0);
		}
	}

	return 0;
}
