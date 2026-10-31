#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A){ cin >> a;}
	int64_t tot = 0;
	int64_t mx = 0;
	for (int64_t a : A) {
		tot += a;
		mx = std::max(mx, a);
	}
	cout << std::min(tot / 3, N - mx) << '\n';

	return 0;
}
