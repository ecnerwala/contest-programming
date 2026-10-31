#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int64_t prv = 0;
	for (int i = 0; i < N; i++) {
		int t; cin >> t;
		prv = (prv >> t) + 1;
		if (!(prv & 1)) prv++;
		prv <<= t;
	}
	cout << prv << '\n';

	return 0;
}
