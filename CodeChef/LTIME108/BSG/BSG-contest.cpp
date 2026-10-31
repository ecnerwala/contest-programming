#include <bits/stdc++.h>

int solve(std::string S) {
	int N = int(S.size());
	std::vector<int> pos0; pos0.reserve(N);
	std::vector<int> pos1; pos1.reserve(N);
	for (int i = 0; i < N; i++) {
		if (S[i] == '0') pos0.push_back(i);
		else if (S[i] == '1') pos1.push_back(i);
		else assert(false);
	}

	auto is_good = [&](int num_turns) -> bool {
		if (num_turns * 2 > std::min(int(pos0.size()), int(pos1.size()))) return false;
		for (int c0 = 0; c0 <= num_turns; c0++) {
			int c1 = num_turns - c0;
			// num_turns + c0 on the left, num_turns + c1 on the right
			int cur_0 = num_turns + c0 - 1;
			int cur_1 = int(pos1.size()) - (num_turns + c1);
			int p1 = int(pos1.size()) - 1 - (num_turns + c1);
			int p0 = num_turns + c0;

			int num_0 = c0;
			int num_1 = c1;

			bool split_good = true;
			for (int t = 0; t < num_turns; t++) {
				// B's turn
				if (pos0[cur_0] < pos1[cur_1]) {
					cur_0--;
					cur_1++;
				} else {
					split_good = false;
					break;
				}
				// A's turn
				if (num_1 > 0 && pos0[p0] < pos1[cur_1]) {
					p0++;
					cur_1++;
					num_1 --;
				} else if (num_0 > 0 && pos0[cur_0] < pos1[p1]) {
					cur_0--;
					p1--;
					num_0 --;
				} else {
					split_good = false;
					break;
				}
			}
			if (split_good) {
				return true;
			}
		}
		return false;
	};

	int mi = 0, ma = std::min(int(pos0.size()), int(pos1.size())) / 2 + 1;
	while (ma - mi > 1) {
		int md = (mi + ma) / 2;
		if (is_good(md)) {
			mi = md;
		} else {
			ma = md;
		}
	}

	return mi;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		string S; cin >> S;
		cout << solve(std::move(S)) << '\n';
	}

	return 0;
}
