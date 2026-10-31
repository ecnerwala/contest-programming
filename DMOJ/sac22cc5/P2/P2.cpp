#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	string S; cin >> S;
	if (N & 1) {
		cout << S << '\n';
	} else {
		if (S == "left") cout << "right" << '\n';
		else cout << "left" << '\n';
	}

	return 0;
}
