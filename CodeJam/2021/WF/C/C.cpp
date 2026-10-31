#include <bits/stdc++.h>

std::mt19937 mt(48);

#ifdef ECNERWALA
constexpr bool TEST = true;
#else
constexpr bool TEST = false;
#endif

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T, N, W;
	if constexpr (TEST) {
		T = 100;
		N = 50;
		W = 1;
	} else {
		cin >> T >> N >> W;
	}
	int num_wins = 0;
	for (int case_num = 1; case_num <= T; case_num ++) {
		vector<int> top_used(2*N);
		vector<int> bot_used(2*N);
		vector<array<int, 2>> lines; lines.reserve(2*N);

		int my_score = 0;
		int their_score = 0;

		auto record_move = [&](array<int, 2> m) -> int {
			int v = 0;
			for (auto l : lines) {
				v += (m[0] > l[0]) != (m[1] > l[1]);
			}
			assert(!top_used[m[0]]);
			top_used[m[0]] = true;
			assert(!bot_used[m[1]]);
			bot_used[m[1]] = true;
			lines.push_back(m);
			return v;
		};

		for (int t = 0; t < N; t++) {
			pair<tuple<int, int, int>, array<int, 2>> move{{-1, -1, -1}, {-1, -1}};
			for (int i = 0; i < 2*N; i++) {
				if (top_used[i]) continue;
				for (int j = 0; j < 2*N; j++) {
					if (bot_used[j]) continue;
					int v = 0;
					for (auto l : lines) {
						v += (i > l[0]) != (j > l[1]);
					}
					move = max(move, {{v, i, j}, {i, j}});
				}
			}
			if (t == 0) move.second = {2*N-10, 2*N-10};
			//cerr << "doing " << move.second[0] << ' ' << move.second[1] << '\n';
			if constexpr (TEST) {
			} else {
				cout << move.second[0]+1 << ' ' << move.second[1]+1 << '\n' << flush;
			}
			my_score += record_move(move.second);
			if constexpr(TEST) {
				int best_val = -1;
				vector<array<int, 2>> moves;
				for (int i = 0; i < 2*N; i++) {
					if (top_used[i]) continue;
					for (int j = 0; j < 2*N; j++) {
						if (bot_used[j]) continue;
						int v = 0;
						for (auto l : lines) {
							v += (i > l[0]) != (j > l[1]);
						}
						if (v > best_val) {
							best_val = v;
							moves = {};
						}
						if (v == best_val) {
							moves.push_back({i, j});
						}
					}
				}
				their_score += record_move(moves[int(mt() % moves.size())]);
			} else {
				array<int, 2> o; cin >> o[0] >> o[1]; o[0]--, o[1]--;
				their_score += record_move(o);
			}
		}
		//cerr << "won " << won << '\n';
		int won;
		if constexpr (TEST) {
			won = my_score > their_score;
			cerr << "Won " << won << ' ' << my_score << ' ' << their_score << '\n';
		} else {
			cin >> won;
		}
		assert(won == 0 || won == 1);
		num_wins += won;
		if constexpr (TEST) {
			cerr << "num_wins " << num_wins << '/' << case_num << ' ' << double(num_wins)/case_num << '\n';
		}
	}

	return 0;
}
