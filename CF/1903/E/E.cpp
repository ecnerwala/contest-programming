#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		if (N == -1) exit(0);
		int sx, sy; cin >> sx >> sy;
		std::array<std::vector<int>, 2> pts;
		std::array<int, 2> cnt{0, 0};
		for (int i = 0; i < N; i++) {
			int x, y; cin >> x >> y;
			int v = (x ^ y ^ sx ^ sy) & 1;
			pts[v].push_back(i);
			cnt[v]++;
		}
		// player 1 deletes odds, player 2 deletes evens
		int p2_turns = (N-1) / 2;
		int p1_turns = (N-1) - p2_turns;
		bool we_are_second = (p2_turns >= cnt[0]);
		assert((p2_turns >= cnt[0]) != (p1_turns >= cnt[1]));
		cout << (we_are_second ? "Second" : "First") << '\n' << flush;
		std::vector<bool> used(N);
		std::vector<int> to_delete = pts[!we_are_second];
		to_delete.insert(to_delete.begin(), pts[we_are_second].begin(), pts[we_are_second].end());
		for (int z = 0; z < N; z++) {
			if ((z & 1) == we_are_second) {
				// our turn
				while (used[to_delete.back()]) to_delete.pop_back();
				used[to_delete.back()] = true;
				cout << to_delete.back()+1 << '\n' << flush;
			} else {
				// their turn
				int v; cin >> v;
				if (v == -1) exit(0);
				v--;
				used[v] = true;
			}
		}
	}

	return 0;
}
