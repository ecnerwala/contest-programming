#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int64_t G; cin >> G; G *= 2;
		int ans = 0;
		for (int64_t i = 1; i * i < G; i++) {
			if (G % i == 0) {
				if ((G / i) % 2 != i % 2) {
					ans ++;
				}
			}
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}

// If you run the machine for d days starting at K, you get
// G = (K + (d-1)/2) * d
// 2G = (2K + d-1) * d with K >= 1
// 2G = a * d where a > d and a % 2 != d % 2
