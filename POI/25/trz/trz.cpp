#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	string S; cin >> S;

	int ans = -1;

	using dat_t = array<int, 3>;
	array<std::optional<std::pair<dat_t, int>>, 3> d1;
	array<array<std::optional<std::pair<dat_t, int>>, 3>, 3> d2;

	dat_t cur = {0, 0, 0};
	for (int i = 0; i < N; i++) {
		{
			int v;
			switch(S[i]) {
				case 'B': v = 0; break;
				case 'C': v = 1; break;
				case 'S': v = 2; break;
				default: assert(false);
			}
			cur[(v+2)%3]++;
			cur[(v+1)%3]--;
		}

		for (int m0 = 0; m0 < 3; m0++) {
			if (cur[m0] != 0) {
				if (!d1[m0]) {
					d1[m0] = {cur, i};
				} else {
					for (int m1 = 0; m1 < 3; m1++) {
						if (m0 == m1) continue;
						if (d2[m0][m1]) continue;
						if (cur[m1] != d1[m0]->first[m1] && cur[3-m0-m1] != -d1[m0]->first[m1]) {
							d2[m0][m1] = {cur, i};
						}
					}
				}
			}
		}

		int m0 = 0;
		while (m0 < 3 && cur[m0] != 0) m0++;
		if (m0 == 3) {
			ans = max(ans, i + 1);
			continue;
		}
		assert(cur[m0] == 0);
		if (!d1[m0]) continue;
		int m1 = 0;
		while (m1 < 3 && cur[m1] != d1[m0]->first[m1]) m1++;
		if (m1 == 3) {
			ans = max(ans, i - d1[m0]->second);
			continue;
		}
		assert(cur[m0] == 0 && cur[m1] == d1[m0]->first[m1]);
		if (!d2[m0][m1]) continue;
		ans = max(ans, i - d2[m0][m1]->second);
	}

	if (ans == -1) cout << "NIE" << '\n';
	else cout << ans << '\n';

	return 0;
}
