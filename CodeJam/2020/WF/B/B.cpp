#include<bits/stdc++.h>

// whether A wins
bool solve_runs(int num_1, int num_2, std::vector<int> runs) {
	using namespace std;
	assert(num_1 + num_2 * 2 == std::accumulate(runs.begin(), runs.end(), 0));
	sort(runs.begin(), runs.end());
	reverse(runs.begin(), runs.end());
	while (!runs.empty() && runs.back() == 0) runs.pop_back();
	reverse(runs.begin(), runs.end());

	static map<tuple<int, int, vector<int>>, int> mem;
	tuple<int, int, std::vector<int>> key(num_1, num_2, runs);
	{ auto it = mem.find(key); if (it != mem.end()) return it->second; }

	bool ans = [&num_1, &num_2, &runs]() -> bool {
		if (!num_2) return false;

		assert(num_2 >= 1);

		for (int i = 0; i < int(runs.size()); i++) {
			for (int v = 0; v < runs[i]; v++) {
				for (int j = 0; j < int(runs.size()); j++) {
					for (int w = 0; w < runs[j]; w++) {
						if (i == j && v == w) continue;
						vector<int> n_runs = runs;
						if (i == j) {
							int a = min(v, w), b = max(v, w);
							assert(a < b);
							n_runs[i] -= b+1;
							n_runs.push_back(a);
							n_runs.push_back(b-a-1);
						} else {
							n_runs[i] -= v+1;
							n_runs.push_back(v);
							n_runs[j] -= w+1;
							n_runs.push_back(w);
						}

						// 2 -> same 2; always safe because it just strictly shrunk
						if (!(i == j && abs(v - w) == 1)) {
							// these 2 go in the same bucket
							if (!solve_runs(num_1, num_2-1, n_runs)) {
								goto A_loss_2;
							}
						}

						if (runs[i] == 1 || (i == j && runs[i] == 2)) {
							// it's safe to fill somewhere else

							// different 2 is only safe if i == j || runs[j] == 1
							if (num_2 >= 2 && (i == j || runs[j] == 1)) {
								if (!solve_runs(num_1+2, num_2-2, n_runs)) {
									goto A_loss_2;
								}
							}

							// different 1 is always safe
							if (num_1 >= 1) {
								if (!solve_runs(num_1, num_2-1, n_runs)) {
									goto A_loss_2;
								}
							}
						}
					}
				}
				return true;
A_loss_2:
				;
			}
		}

		if (num_1 >= 1) {
			for (int i = 0; i < int(runs.size()); i++) {
				for (int v = 0; v < runs[i]; v++) {
					for (int j = 0; j < int(runs.size()); j++) {
						for (int w = 0; w < runs[j]; w++) {
							if (i == j && v == w) continue;
							vector<int> n_runs = runs;
							if (i == j) {
								int a = min(v, w), b = max(v, w);
								assert(a < b);
								n_runs[i] -= b+1;
								n_runs.push_back(a);
								n_runs.push_back(b-a-1);
							} else {
								n_runs[i] -= v+1;
								n_runs.push_back(v);
								n_runs[j] -= w+1;
								n_runs.push_back(w);
							}

							// 1 -> different 2
							if (runs[j] == 1 || (i == j && runs[j] == 2)) {
								if (!solve_runs(num_1, num_2-1, n_runs)) {
									goto A_loss_1;
								}
							}
							// 1 -> other 1
							if (num_1 >= 2) {
								if (!solve_runs(num_1-2, num_2, n_runs)) {
									goto A_loss_1;
								}
							}
						}
					}
					return true;
A_loss_1:
					;
				}
			}
		}

		return false;
	}();

	if (num_2 == 0) {
		assert(ans == false);
	} else if (num_1 == 0 && num_2 == 1) {
		assert(ans == (runs.back() == 2));
	} else {
		int num_odd = num_1;
		for (int r : runs) num_odd += (r % 2 == 1);
		assert(ans == (num_1 % 2 == 1 && num_odd < num_1 + 2 * num_2));
	}

	return (mem[key] = ans);
}

bool is_A_winning(int N, std::vector<std::array<int, 2>> moves) {
	using namespace std;
	array<vector<bool>, 2> used{vector<bool>(N), vector<bool>(N)};
	vector<int> board(N, -2);
	for (auto it : moves) {
		used[0][it[0]] = true;
		used[1][it[1]] = true;
		board[it[0]] = it[1];
	}

	// check for already won
	for (int i = 0; i+1 < N; i++) {
		if (abs(board[i] - board[i+1]) == 1) return true;
	}

	// out of moves
	if (int(moves.size()) == N) return false;

	// check if there's an immediate win, i.e. adjacent to an existing move
	for (auto it : moves) {
		for (int dx : {-1, 1}) {
			if (0 <= it[0] + dx && it[0] + dx < N && !used[0][it[0] + dx]) {
				for (int dy : {-1, 1}) {
					if (0 <= it[1] + dy && it[1] + dy < N && !used[1][it[1] + dy]) {
						return true;
					}
				}
			}
		}
	}

	array<vector<int>, 2> runs;
	for (int z : {0, 1}) {
		runs[z].reserve(N);
		int cur = 0;
		for (int i = 0; i < N; i++) {
			if (used[z][i]) {
				if (cur) runs[z].push_back(cur);
				cur = 0;
			} else {
				cur++;
			}
		}
		if (cur) runs[z].push_back(cur);
		sort(runs[z].begin(), runs[z].end());
	}

	assert(!runs[0].empty());
	assert(!runs[1].empty());
	if (runs[0].back() >= 3 && runs[1].back() >= 3) return true;

	if (runs[0].back() > runs[1].back()) {
		swap(runs[0], runs[1]);
	}
	assert(runs[0].back() <= runs[1].back());
	assert(runs[0].back() <= 2);

	int num_1 = 0, num_2 = 0;
	for (int r : runs[0]) {
		if (r == 1) num_1++;
		else if (r == 2) num_2++;
		else assert(false);
	}

	return solve_runs(num_1, num_2, runs[1]);
}

bool is_B_winning(int N, std::vector<std::array<int, 2>> moves) {
	using namespace std;
	array<vector<bool>, 2> used{vector<bool>(N), vector<bool>(N)};
	vector<int> board(N, -2);
	for (auto it : moves) {
		used[0][it[0]] = true;
		used[1][it[1]] = true;
		board[it[0]] = it[1];
	}

	// check for already lost
	for (int i = 0; i+1 < N; i++) {
		if (abs(board[i] - board[i+1]) == 1) return false;
	}

	// out of moves, we win!
	if (int(moves.size()) == N) return true;

	for (int i = 0; i < N; i++) {
		if (used[0][i]) continue;
		for (int j = 0; j < N; j++) {
			if (used[1][j]) continue;
			moves.push_back({i, j});
			if (!is_A_winning(N, moves)) return true;
			moves.pop_back();
		}
	}
	return false;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	for (int n_1 = 0; n_1 <= 50; n_1++) {
		for (int n_2 = 0; n_2 <= 50; n_2++) {
			if (n_1 == 0 && n_2 == 0) continue;
			if (n_1 + 2 * n_2 + (n_1 + n_2 - 1) > 50) continue;
			solve_runs(n_1, n_2, vector<int>({n_1 + 2 * n_2}));
		}
	}

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		vector<array<int, 2>> moves(N);
		for (int i = 0; i < N; i++) {
			cin >> moves[i][0] >> moves[i][1];
			moves[i][0]--, moves[i][1]--;
		}

		vector<bool> state(N);
		for (int i = 0; i < N; i++) {
			if (i & 1) {
				state[i] = is_B_winning(N, {moves.begin(), moves.begin()+i});
			} else {
				state[i] = is_A_winning(N, {moves.begin(), moves.begin()+i});
			}
		}
		int a_mistakes = 0, b_mistakes = 0;
		for (int i = 0; i+1 < N; i++) {
			if (state[i] && state[i+1]) {
				if (i & 1) {
					b_mistakes++;
				} else {
					a_mistakes++;
				}
			}
		}

		cout << "Case #" << case_num << ": " << a_mistakes << ' ' << b_mistakes << '\n';
	}

	return 0;
}

// Player A essentially always wins...
// Maybe A is always winning up till move ~N/2?
// We'll just solve winningness on A's turn
// If A has a playable tile, A wins
// else if there are 3 empty tiles in a row && 3 empty spaces in a row, A wins
// else if there are no adjacent tiles or no adjacent spaces in a row, A loses
// else wlog there no 3 adjacent tiles; only runs of length <= 2
// it's weird; (2,1) x (3) is winning
// Partition # dp???
