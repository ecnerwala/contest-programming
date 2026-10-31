#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> A(N); for (auto& a : A) cin >> a;
	sort(A.begin(), A.end());

	int ans = -1;
	map<int, int> gaps;
	for (int i = N-3; i >= 4; i--) {
		gaps.insert({i+1, A[i+2] - A[i+1]});
		int s4 = A[i-1] + A[i-2] + A[i-3] + A[i-4];
		int v3 = A[i];
		// P2 < s4 - v3
		int i2 = int(lower_bound(A.begin() + i + 1, A.end() - 1, s4 - v3) - A.begin()) - 1;
		if (i2 == i) continue;
		int v2 = A[i2];
		{
			// we need to find a gap before i2
			while (true) {
				auto it = gaps.upper_bound(i2);
				if (it == gaps.begin()) break;
				--it;
				if (it->second >= v3) {
					gaps.erase(it);
					continue;
				}
				ans = max(ans, s4 + v3 + A[it->first] + A[it->first+1]);
				break;
			}
		}
		int i1 = int(lower_bound(A.begin() + i2 + 1, A.end(), v2 + v3) - A.begin()) - 1;
		if (i1 == i2) continue;
		ans = max(ans, s4 + v3 + v2 + A[i1]);
	}
	cout << ans << '\n';

	return 0;
}
