#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	cout << 105 << ' ';
	N--;
	for (int v = 2; N > 0; v += 2) {
		if (v % 3 == 0 || v % 5 == 0 || v % 7 == 0) {
			cout << v << " \n"[--N == 0];
		}
	}

	return 0;
}

// We'll 15, and anything that's even and a multiple of 3 or 5
// 7 / 30 too low
