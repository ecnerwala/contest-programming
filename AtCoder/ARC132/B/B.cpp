#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N); for (auto& a : A) { cin >> a; a--; }
	// optionally reverse, optionally shift some number of times, optionally reverse

	int ans = N+3;
	for (int z = 0; z < 2; z++, std::reverse(A.begin(), A.end())) {
		for (int w = 0; w < 2; w++) {
			std::vector<int> B = A;
			int loc = int(std::find(B.begin(), B.end(), w ? N-1 : 0) - B.begin());
			std::rotate(B.begin(), B.begin()+loc, B.end());
			if (w) std::reverse(B.begin(), B.end());
			if (std::is_sorted(B.begin(), B.end())) {
				ans = std::min(ans, z + w + loc);
			}
		}
	}
	cout << ans << '\n';

	return 0;
}
