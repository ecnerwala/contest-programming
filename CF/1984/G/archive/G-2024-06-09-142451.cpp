#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto is_cycle = [&](std::vector<int> P) {
		int N = int(P.size());
		for (int i = 0; i < N; i++) {
			if (P[(i+1)%N] != (P[i]+1)%N) {
				return false;
			}
		}
		return true;
	};

	auto perm_parity = [&](std::vector<int> P) {
		int N = int(P.size());
		bool res = 0;
		for (int i = 0; i < N; i++) {
			while (P[i] != i) {
				std::swap(P[i], P[P[i]]);
				res ^= 1;
			}
		}
		return res;
	};

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> P(N);
		for (auto& p : P) { cin >> p; p--; }
		int K = -1;
		std::vector<std::array<int, 2>> ops; ops.reserve(5*N*N);
		auto do_op = [&](int i, int j) -> void {
			assert(K != -1);
			assert(0 <= i && i <= N-K);
			assert(0 <= j && j <= N-K);
			assert(i != j);
			if (i < j) {
				std::rotate(P.begin() + i, P.begin() + i + K, P.begin() + j + K);
			} else {
				std::rotate(P.begin() + j, P.begin() + i, P.begin() + i + K);
			}
			ops.push_back({i, j});
		};
		auto solve = [&]() -> void {
			if (is_cycle(P)) {
				if (P[0] == 0) {
					K = N;
					return;
				}
				K = N-1;
				while (P[0] != 0) {
					do_op(1, 0);
				}
				assert(!ops.empty());
				return;
			}
			if (N % 2 == 0) {
				if (perm_parity(P)) {
					K = N-3;
					// Just put N-1 at the end, and then solve the odd case
					assert(N >= 6);
					if (P[N-1] != N-1) {
						if (P[N-2] != N-1) {
							while (P[N-3] != N-1) {
								do_op(1, 0);
							}
							assert(P[N-3] == N-1);
							do_op(1, 2);
						}
						assert(P[N-2] == N-1);
						do_op(2, 3);
					}
					assert(P[N-1] == N-1);
					N--;
					P.pop_back();
				} else {
					K = N-2;
				}
			} else {
				K = N-2;
			}
			assert(K == N-2);
			assert(N == int(P.size()));
			auto cyc_whole = [&](int new_pos_0) -> void {
				if (N % 2 == 0) {
					assert(new_pos_0 % 2 == 0);
				}
				std::rotate(P.begin(), P.begin() + new_pos_0, P.end());
				int cnt = (new_pos_0 + ((new_pos_0 & 1) ? N : 0)) / 2;
				for (int z = 0; z < cnt; z++) {
					ops.push_back({2, 0});
				}
			};
			auto cyc_suff = [&](int new_pos_1) -> void {
				assert(new_pos_1 >= 1);
				std::rotate(P.begin() + 1, P.begin() + new_pos_1, P.end());
				int cnt = new_pos_1 - 1;
				for (int z = 0; z < cnt; z++) {
					ops.push_back({2, 1});
				}
			};
			auto cyc_pref = [&](int new_pos_0) -> void {
				assert(new_pos_0 < N-1);
				std::rotate(P.begin(), P.begin() + new_pos_0, P.end() - 1);
				int cnt = new_pos_0;
				for (int z = 0; z < cnt; z++) {
					ops.push_back({1, 0});
				}	
			};
			if (N % 2 == 1) {
				// We'll just do selection sort
				for (int v = N-2; v >= 0; v--) {
					cyc_whole(int(std::find(P.begin(), P.end(), v) - P.begin()));
					if (v == 0) assert(P[1] == 1);
					cyc_suff(int(std::find(P.begin(), P.end(), v+1) - P.begin()));
				}
			} else {
				// Cycle the 2nd half
				for (int v = N-2; v >= 0; v--) {
					int pos_v = int(std::find(P.begin(), P.end(), v) - P.begin());
					if (pos_v % 2 == 1) {
						assert(v != 0);
						// put pos_v at the end
						cyc_whole((pos_v+1) % N);
						// now cyclic shift
						cyc_pref(int(std::find(P.begin(), P.end(), v+1) - P.begin()));
					} else {
						cyc_whole(pos_v);
						cyc_suff(int(std::find(P.begin(), P.end(), v+1) - P.begin()));
					}
				}
			}
		};
		solve();
		cout << K << '\n';
		cout << ops.size() << '\n';
		for (auto [i, j] : ops) {
			cout << i+1 << ' ' << j+1 << '\n';
		}
	}

	return 0;
}
