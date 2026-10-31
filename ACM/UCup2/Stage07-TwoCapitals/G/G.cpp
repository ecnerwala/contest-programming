#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int A, C; cin >> A >> C;
	cout << std::max({(A+3) / 4, (C+3) / 4, (A + C + 6) / 7}) * 2 - 1 << '\n';

	return 0;
}
