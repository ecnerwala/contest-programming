#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	constexpr bool TESTING = false;
	std::mt19937_64 mt(48);

	int T;
	if (TESTING) {
		T = 1000;
	} else {
		cin >> T;
	}
	while (T--) {
		int N;
		if (TESTING) {
			N = std::uniform_int_distribution<int>(2, 10)(mt);
		} else {
			cin >> N;
		}
		std::vector<int> testing_ans;
		if (TESTING) {
			testing_ans.resize(N*N);
			std::iota(testing_ans.begin(), testing_ans.end(), 0);
			std::shuffle(testing_ans.begin(), testing_ans.end(), mt);
		}

		int num_queries = 0;
		// returns the index of v
		auto query = [&](const std::vector<int>& v) -> int {
			num_queries++;
			assert(int(v.size()) == N);
			cout << '?';
			for (int i = 0; i < N; i++) {
				cout << ' ' << v[i]+1;
			}
			cout << '\n' << flush;
			int res;
			if (TESTING) {
				res = v[0];
				for (int i = 1; i < N; i++) {
					if (testing_ans[v[i]] < testing_ans[res]) {
						res = v[i];
					}
				}
				cerr << res+1 << '\n';
			} else {
				cin >> res; res--;
			}
			auto it = std::find(v.begin(), v.end(), res);
			assert(it != v.end());
			return int(it - v.begin());
		};

		std::vector<int> ans; ans.reserve(N*N - N + 1);

		std::vector<std::vector<int>> groups(N, std::vector<int>(N));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				groups[i][j] = i * N + j;
			}
			int idx = query(groups[i]);
			std::swap(groups[i][0], groups[i][idx]);
		}

		std::vector<int> buf; buf.reserve(N);
		for (int t = 0; t < N * N - N + 1; t++) {
			buf.clear();
			for (int i = 0; i < N; i++) buf.push_back(groups[i][0]);
			int i = query(buf);
			ans.push_back(groups[i][0]);
			groups[i].erase(groups[i].begin());
			if (t + 1 == N * N - N + 1) break;
			int tgt = (groups[i].size() <= 1) ? 1 : N;
			if (tgt == 1) {
				if (groups[i].empty()) {
					for (int j = 0; j < N; j++) {
						if (j == i) continue;
						if (int(groups[i].size()) < tgt && groups[j].size() > 2) {
							groups[i].push_back(groups[j].back());
							groups[j].pop_back();
						}
					}
					for (int j = 0; j < N; j++) {
						if (j == i) continue;
						if (int(groups[i].size()) < tgt) {
							groups[i].push_back(groups[j].back());
							groups[j].pop_back();
						}
					}
				}
				continue;
			}
			//cerr << groups[i].size() << ' '; for (int j = 0; j < N; j++) { if (j == i) continue; cerr << groups[j].size() << ' '; } cerr << '\n';
			std::vector<int> src; src.reserve(N);
			for (int j = 0; j < N; j++) {
				if (j == i) continue;
				if (int(groups[i].size()) < tgt) {
					groups[i].push_back(groups[j].back());
					groups[j].pop_back();
					src.push_back(j);
				}
			}
			assert(int(groups[i].size()) == tgt);
			int idx = query(groups[i]);
			std::swap(groups[i][0], groups[i][idx]);
			while (!src.empty()) {
				int j = src.back();
				src.pop_back();
				groups[j].push_back(groups[i].back());
				groups[i].pop_back();
			}
		}

		cout << '!';
		for (int v : ans) { cout << ' ' << v+1; }
		cout << '\n' << flush;

		if (TESTING) {
			for (int i = 0; i < int(ans.size()); i++) {
				assert(testing_ans[ans[i]] == i);
			}
			assert(num_queries <= 2 * N * N - 2 * N + 1);
		}
	}

	return 0;
}
