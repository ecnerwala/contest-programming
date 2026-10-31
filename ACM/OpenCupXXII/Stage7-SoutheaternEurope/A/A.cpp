#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	string S; cin >> S; assert(int(S.size()) == 2*N);
	std::array<int, 3> cnt{0,0,0};
	for (char c : S) {
		cnt[c-'A']++;
	}
	if (cnt[0] > N || cnt[1] > N || cnt[2] > N) {
		cout << "NO" << '\n';
		exit(0);
	}

	int num_AB = N - cnt[2];
	int num_AC = N - cnt[1];
	int num_BC = N - cnt[0];

	std::vector<std::array<int, 2>> ops; ops.reserve(N);

	std::vector<bool> used(2*N);
	// first, match the num_AB B's
	{
		std::vector<int> stk; stk.reserve(N);
		int num_B = 0;
		for (int i = 0; i < 2*N; i++) {
			if (S[i] == 'A') {
				stk.push_back(i);
			} else if (S[i] == 'B') {
				num_B++;
				if (num_B > num_BC) {
					if (stk.empty()) {
						cout << "NO" << '\n';
						exit(0);
					}
					int j = stk.back(); stk.pop_back();
					ops.push_back({j, i});
					used[i] = used[j] = true;
				}
			}
		}
	}

	{
		std::vector<int> stk; stk.reserve(N);
		for (int i = 0; i < 2*N; i++) {
			if (used[i]) continue;
			if (S[i] == 'A' || S[i] == 'B') {
				stk.push_back(i);
			} else {
				if (stk.empty()) {
					cout << "NO" << '\n';
					exit(0);
				}
				int j = stk.back(); stk.pop_back();
				ops.push_back({j, i});
			}
		}
		if (!stk.empty()) {
			cout << "NO" << '\n';
			exit(0);
		}
	}

	cout << "YES" << '\n';
	for (auto [x, y] : ops) {
		cout << x+1 << ' ' << y+1 << '\n';
	}

	return 0;
}
