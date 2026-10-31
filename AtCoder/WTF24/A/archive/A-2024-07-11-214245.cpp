#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		std::array<int, 3> A;
		for (auto& a : A) cin >> a;
		for (int z = 0; z < 5; z++) {
			++*std::min_element(A.begin(), A.end());
		}
		cout << A[0] * A[1] * A[2] << '\n';
	}

	return 0;
}
