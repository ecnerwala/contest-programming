#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
		std::vector<int> S(M);
		for (auto& s : S) cin >> s;
		std::vector<int> D(M);
		for (auto& d : D) cin >> d;

		std::vector<std::vector<int>> ans(M);
		auto solve = [&]() -> bool {
			// we have a bunch of intervals which must be all distinct
			std::priority_queue<std::pair<int, int>> rows;
			// fill from the top downwards
			for (int m = 0; m < M; m++) {
				ans[m].reserve(S[m]);
				for (int i = 0; i < S[m]; i += D[m]) {
					rows.push({std::min(S[m] - i, D[m]), m});
				}
			}

			std::vector<int> freq(N);
			for (auto a : A) freq[a]++;
			std::vector<std::pair<int, int>> by_freq; by_freq.reserve(N);
			for (int i = 0; i < N; i++) {
				if (freq[i]) by_freq.push_back({freq[i], i});
			}
			std::sort(by_freq.begin(), by_freq.end(), std::greater<>());

			{
				std::vector<std::pair<int, int>> buf; buf.reserve(N);
				for (auto [f, v] : by_freq) {
					if (int(rows.size()) < f) return false;
					for (int z = 0; z < f; z++) {
						ans[rows.top().second].push_back(v);
						if (rows.top().first - 1 > 0) buf.push_back({rows.top().first - 1, rows.top().second});
						rows.pop();
					}
					for (auto a : buf) rows.push(a);
					buf.clear();
				}
			}

			std::vector<std::pair<int, int>> buf; buf.reserve(N);
			for (int m = 0; m < M; m++) {
				assert(int(ans[m].size()) == S[m]);
				buf.clear();
				for (int i = 0; i < S[m]; i++) {
					if (buf.empty() || ans[m][i] != buf.back().second) {
						buf.push_back({1, ans[m][i]});
					} else {
						buf.back().first++;
					}
				}
				std::sort(buf.begin(), buf.end(), std::greater<>());
				auto it = buf.begin();
				for (int i = 0; i < D[m]; i++) {
					for (int j = (S[m] / D[m] + (i < S[m] % D[m])) - 1; j >= 0; j--) {
						ans[m][j * D[m] + i] = it->second;
						--it->first;
						if (it->first == 0) {
							++it;
						}
					}
				}
			}
			return true;
		};

		if (solve()) {
			for (int m = 0; m < M; m++) {
				for (int j = 0; j < S[m]; j++) {
					cout << ans[m][j] + 1 << " \n"[j+1==S[m]];
				}
			}
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
