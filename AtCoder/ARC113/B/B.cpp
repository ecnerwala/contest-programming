#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int A, B, C; cin >> A >> B >> C;
	A %= 10;
	if (C == 1) {
		// nothing
		B = B % 4;
	} else if (C % 2 == 0) {
		B = B % 2;
	} else {
		B = B % 4;
		B = B * B * B % 4;
	}
	if (B == 0) B = 4;
	int r = 1;
	while (B--) r *= A;
	cout << r % 10 << '\n';

	return 0;
}
