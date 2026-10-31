#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int B; cin >> B;
	int P = 5 * B - 400;
	cout << P << '\n';
	cout << (P < 100) - (P > 100) << '\n';

	return 0;
}
