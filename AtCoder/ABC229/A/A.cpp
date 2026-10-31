#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	string A, B; cin >> A >> B;

	string C = A+B;
	if (C == ".##." || C == "#..#") {
		cout << "No" << '\n';
	} else {
		cout << "Yes" << '\n';
	}

	return 0;
}
