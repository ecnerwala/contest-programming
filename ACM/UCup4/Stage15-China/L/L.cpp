#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int R, T; cin >> R >> T;
	auto rev_graph = [&](std::vector<int>& v) {
		int N = int(v.size());
		std::reverse(v.begin(), v.end());
		for (auto& a : v) { a = (a == -1) ? a : N-1-a; }
	};
	auto involute = [&](std::vector<int> P) -> std::vector<int> {
		int N = int(P.size());
		assert(P[0] == -1);
		// First, transform into "numeric form"
		std::vector<std::pair<int, std::array<int, 2>>> vals(N, {-1, {-1, -1}});
		{
			std::vector<std::vector<int>> cur_dots; cur_dots.reserve(N);
			std::vector<int> stk; stk.reserve(N);
			std::vector<bool> is_par(N, false);
			for (int i = N-1; i >= 0; i--) {
				bool is_one = is_par[i];
				bool is_dot = (i > 0 && !is_par[P[i]]);
				if (i > 0) { is_par[P[i]] = true; }

				vals[i].first = is_one * 2 + is_dot;
				if (is_one) {
					// pick which of the dots connects here
					int z;
					for (z = 0; z < int(cur_dots.size()); z++) {
						if (P[cur_dots[z][0]] == i) break;
					}
					assert(z < int(cur_dots.size()));
					vals[i].second[0] = z;
					cur_dots.erase(cur_dots.begin() + z);
				}

				if (is_dot) {
					// push back a single vector
					cur_dots.emplace_back(1, i);
				} else {
					// need to write down which of cur_dots matches us
					int z;
					if (i == 0) {
						z = -2;
					} else {
						for (z = 0; z < int(cur_dots.size()); z++) {
							if (P[cur_dots[z][0]] == P[i]) break;
						}
						assert(z < int(cur_dots.size()));
						// doesn't matter
						//cur_dots[z].push_back(i);
					}
					vals[i].second[1] = z;
				}

				if (is_dot && !is_one) {
					stk.push_back(i);
				} else if (!is_dot && is_one) {
					int j = stk.back();
					stk.pop_back();
					vals[j].second[1] = vals[i].second[1];
					vals[j].second[0] = vals[i].second[0];
				}
			}
			assert(cur_dots.empty());
			assert(stk.empty());
		}
		// This is the involution
		std::reverse(vals.begin(), vals.end());
		for (auto& a : vals) {
			a.first ^= 3;
			std::swap(a.second[0], a.second[1]);
		}
		// Now, transform back into parent form
		std::vector<int> Q(N, -2);
		{
			std::vector<std::vector<int>> cur_dots; cur_dots.reserve(N);
			for (int i = N-1; i >= 0; i--) {
				bool is_one = vals[i].first & 2;
				bool is_dot = vals[i].first & 1;

				if (is_one) {
					int z = vals[i].second[0];
					assert(z != -1);
					assert(0 <= z && z < int(cur_dots.size()));
					for (auto j : cur_dots[z]) Q[j] = i;
					cur_dots.erase(cur_dots.begin() + z);
				}
				
				if (is_dot) {
					cur_dots.emplace_back(1, i);
				} else {
					int z = vals[i].second[1];
					if (i == 0) {
						assert(z == -2);
						Q[i] = -1;
					} else {
						assert(z != -1);
						assert(0 <= z && z < int(cur_dots.size()));
						cur_dots[z].push_back(i);
					}
				}
			}
		}
		return Q;
	};
	while (T--) {
		int N; cin >> N;
		std::vector<int> P(N);
		for (auto& p : P) { cin >> p; p--; }
		if (R == 2) rev_graph(P);
		P = involute(P);
		if (R == 1) rev_graph(P);
		for (int i = 0; i < N; i++) cout << P[i]+1 << " \n"[i+1==N];
	}

	return 0;
}
