#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int A, B; cin >> A >> B;
	if (A != 6 || B == 1) {
		cout << "No" << '\n';
	} else if (B & 1) {
		cout << "Yes" << '\n' << "aaba" << std::string((B-3)/2, 'b') << "abaab" << std::string((B-3)/2, 'b') << '\n';
	} else {
		cout << "Yes" << '\n' << "aaa" << std::string(B/2, 'b') << "aaa" << std::string(B/2, 'b') << '\n';
	}

	return 0;
}
