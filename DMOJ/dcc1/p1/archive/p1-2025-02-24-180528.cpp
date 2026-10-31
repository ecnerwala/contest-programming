#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t N, A, B; cin >> N >> A >> B;
	if (A <= B && !((B - A) & 1)) {
		cout << "YES" << '\n';
	} else {
		cout << "NO" << '\n';
	}

	return 0;
}
