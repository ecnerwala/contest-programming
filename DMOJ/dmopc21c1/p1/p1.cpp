#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int64_t v = 0;
	while (N--) {
		int A; cin >> A;
		if (A & 1) {
			v -= (A+1) / 2;
		} else {
			v += A/2;
		}
	}
	if (v > 0) {
		cout << "Duke" << '\n';
	} else {
		cout << "Alice" << '\n';
	}

	return 0;
}
