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

	std::vector<std::pair<int, bool>> stk; stk.reserve(N);
	auto is_good = [&](int num_turns) -> bool {
		if (num_turns * 2 > std::min(int(pos0.size()), int(pos1.size()))) return false;
		stk.clear();
		bool good = false;
		int idx = 0;
		for (int i = 0; i < N; i++) {
			if (S[i] == '0' && i <= pos0[2*num_turns - 1]) {
				stk.push_back({idx, false});
				idx++;
			} else if (S[i] == '1' && i >= pos1[int(pos1.size()) - 2*num_turns]) {
				if (stk.empty()) return false;
				bool good_range = (idx - stk.back().first - 1 >= 4 * (num_turns - int(stk.size())))
					&& (int(stk.size()) >= num_turns || stk.back().second);
				stk.pop_back();
				if (good_range) {
					if (stk.empty()) good = true;
					else stk.back().second = true;
				}
				idx++;
			}
		}
		if (!stk.empty()) return false;
		assert(idx == 4 * num_turns);
		return good;
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
