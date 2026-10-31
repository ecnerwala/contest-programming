#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int num_no_overtakes = 0;
	int ans = 0;
	std::vector<int> speed_signs; speed_signs.reserve(N);
	int cur_speed = -1;
	for (int i = 0; i < N; i++) {
		int t; cin >> t;
		if (t == 1) {
			int s; cin >> s;
			while (!speed_signs.empty() && s > speed_signs.back()) {
				ans ++;
				speed_signs.pop_back();
			}
			cur_speed = s;
		} else if (t == 2) {
			ans += num_no_overtakes;
			num_no_overtakes = 0;
		} else if (t == 3) {
			int s; cin >> s;
			assert(cur_speed != -1);
			if (cur_speed > s) {
				ans++;
			} else {
				speed_signs.push_back(s);
			}
		} else if (t == 4) {
			num_no_overtakes = 0;
		} else if (t == 5) {
			speed_signs.clear();
		} else if (t == 6) {
			num_no_overtakes++;
		}
	}
	cout << ans << '\n';

	return 0;
}
