#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; char _seq; cin >> T >> _seq;
	assert(_seq == 'Y' || _seq == 'N');
	bool CONSTRUCT_SEQUENCE = _seq == 'Y';
	while (T--) {
		std::string A, B; cin >> A >> B;
		std::array<int, 10> freq_A; freq_A.fill(0);
		std::array<int, 10> freq_B; freq_B.fill(0);
		for (char c : A) { assert('0' <= c && c <= '9'); freq_A[c - '0']++; }
		for (char c : B) { assert('0' <= c && c <= '9'); freq_B[c - '0']++; }

		auto solve = [&](std::array<int, 10> deltas, int adj) -> std::pair<int, std::vector<std::string>> {
			for (auto& v : deltas) {
				v += adj;
			}

			bool add_op = false;

			if (deltas[9] == 0) {
				// always fine
			} else if (deltas[9] < 0) {
				bool has_neg = false;
				for (int i = 0; i < 9; i++) {
					if (deltas[i] < 0) {
						has_neg = true;
					}
				}
				if (!has_neg) {
					add_op = true;
				}
			} else if (deltas[9] > 0) {
				bool has_pos = false;
				for (int i = 0; i < 9; i++) {
					if (deltas[i] > 0) {
						has_pos = true;
					}
				}
				if (!has_pos) {
					add_op = true;
				} else {
					// let's make sure that we can do it with our prereqs and stuff
					bool has_extra = false;
					for (int i = 9; i > 0; i--) {
						assert(freq_A[i] + deltas[i-1] - deltas[i] == freq_B[i]);
						if (deltas[i-1] > 0 && freq_A[i] + deltas[i-1] > deltas[i]) {
							// we have an extra op here
							has_extra = true;
						}
					}
					if (!has_extra) {
						add_op = true;
					}
				}
			}

			int64_t cost = 0;
			for (int i = 0; i < 9; i++) cost += std::abs(deltas[i]);
			if (add_op) cost += 2;

			std::array<int, 10> pos_ops; pos_ops.fill(0);
			std::array<int, 10> neg_ops; neg_ops.fill(0);
			for (int i = 0; i < 10; i++) {
				if (deltas[i] > 0) pos_ops[i] = deltas[i];
				else if (deltas[i] < 0) neg_ops[i] = -deltas[i];
			}
			if (add_op) {
				pos_ops[0]++;
				neg_ops[0]++;
			}
			assert(std::accumulate(pos_ops.begin(), pos_ops.begin() + 9, 0) + std::accumulate(neg_ops.begin(), neg_ops.begin() + 9, 0) == cost);

			if (!CONSTRUCT_SEQUENCE) {
				return {cost, {}};
			}

			std::vector<std::string> ops;
			ops.reserve(500);
			ops.push_back(A);

			auto cur_freq = freq_A;
			auto do_op = [&](int src, int dst) -> void {
				assert(abs(dst - src) == 1);
				if (dst - src == 1) {
					assert(pos_ops[src] > 0);
					pos_ops[src]--;
				} else if (dst - src == -1) {
					assert(neg_ops[dst] > 0);
					neg_ops[dst]--;
				} else assert(false);
				if (src == 0 && cur_freq[src] == 0) cur_freq[src]++;
				assert(cur_freq[src] > 0);
				cur_freq[src]--;
				std::string s;
				for (int i = 1; i <= 9; i++) {
					s.append(cur_freq[i], char('0' + i));
				}
				s.push_back(char('0' + src));
				ops.push_back(s);
				s.back() = char('0' + dst);
				ops.push_back(s);
			};

			if (pos_ops[9] > 0) {
				// First, do all positive ops which are necessary for 9->0
				for (int i = 1; i < 9; i++) {
					// only do the ones which are necessary
					int num_do = std::min(pos_ops[i-1], pos_ops[i] - freq_A[i]);
					assert(i-1 == 0 || cur_freq[i-1] >= num_do);
					for (int z = 0; z < num_do; z++) {
						do_op(i-1, i);
					}
				}

				assert(cur_freq[9] >= pos_ops[9]);
				{
					int i = 1;
					while (pos_ops[i] <= 0) i++;
					assert(i < 9);
				}
			}

			ops.push_back(B);

			return {cost, ops};
		};

		std::array<int, 10> deltas;
		deltas[0] = 0;
		for (int i = 1; i < 10; i++) {
			deltas[i] = deltas[i-1] + freq_A[i] - freq_B[i];
		}

		{
			// don't include the last element
			std::vector<int> cur(deltas.begin(), deltas.begin() + 9);
			std::sort(cur.begin(), cur.end());
			assert(int(cur.size()) == 9);
			int mid = cur[(int(cur.size()) - 1) / 2];
			for (auto& v : deltas) v -= mid;
		}

		auto c0 = solve(deltas, 0);
		auto c1 = solve(deltas, +1);
		auto c2 = solve(deltas, -1);
		auto best = std::min({c0, c1, c2});
		cout << best.first << '\n';
		if (CONSTRUCT_SEQUENCE) {
			cout << int(best.second.size())-1 << '\n';
			for (int z = 0; z < int(best.second.size()); z++) {
				cout << best.second[z] << " \n"[z+1==int(best.second.size())];
			}
		}
	}

	return 0;
}
