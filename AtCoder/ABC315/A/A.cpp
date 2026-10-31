#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	string S; cin >> S;
	for (auto c : S) {
		if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
			cout << c;
		}
	}
	cout << '\n';

	return 0;
}
