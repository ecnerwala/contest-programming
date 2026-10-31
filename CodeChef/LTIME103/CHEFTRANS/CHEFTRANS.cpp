#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int X, Y, Z; cin >> X >> Y >> Z;
		if (X+Y < Z) {
			cout << "PLANEBUS" << '\n';
		} else if (X+Y > Z) {
			cout << "TRAIN" << '\n';
		} else {
			cout << "EQUAL" << '\n';
		}
	}

	return 0;
}
