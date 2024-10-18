#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::string S; cin >> S;
	int ans = 0;
	for (int dir : {'L', 'R'}) {
		int cnd = 0;
		int cur = 0;
		for (char c : S) {
			if (c == dir || c == '?') {
				cur++;
				cnd = std::max(cnd, cur);
			} else {
				cur--;
				cur = std::max(cur, 0);
			}
		}
		ans = std::max(ans, cnd);
	}
	cout << ans << '\n';

	return 0;
}
