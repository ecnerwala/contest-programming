#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	string N; cin >> N;
	reverse(N.begin(), N.end());
	int K; cin >> K;
	int ans = 0;
	for (; K >= 1; K--) {
		while (!N.empty() && N.back() == '0') N.pop_back();
		int L = int(N.size());
		if (N.empty()) break;
		if (K == 1) {
			ans += N.back() - '0' + (L-1) * 9;
		} else if (K == 2) {
			ans += (N.back() - '0' - 1) * (L-1) * 9 + (L-1) * (L-2) / 2 * 9 * 9;
		} else if (K == 3) {
			ans += (N.back() - '0' - 1) * (L-1) * (L-2) / 2 * 9 * 9 + (L-1) * (L-2) * (L-3) / 6 * 9 * 9 * 9;
		} else assert(false);
		N.pop_back();
	}
	cout << ans << '\n';

	return 0;
}
