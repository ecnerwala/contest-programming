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

		// we have a bunch of intervals which must be all distinct
		std::vector<std::vector<std::pair<int, int>>> pts(N);
		std::vector<std::vector<int>> ans(M);
		// fill from the top downwards
		for (int m = 0; m < M; m++) {
			ans[m].resize(S[m], -1);
			for (int i = 0; i < D[m]; i++) {
				for (int j = S[m] / D[m] * D[m] + i; j >= 0; j -= D[m]) {
					if (j < S[m]) {
						cerr << m << ' ' << j << '\n';
						pts[i].push_back({m, j});
					}
				}
			}
		}
		std::vector<int> freq(N);
		for (auto a : A) freq[a]++;

		std::vector<std::pair<int, int>> by_freq(N);
		for (int i = 0; i < N; i++) {
			if (freq[i]) by_freq.push_back({freq[i], i});
		}
		std::sort(by_freq.begin(), by_freq.end(), std::greater<>());

		std::vector<std::pair<int, int>> all_pts; all_pts.reserve(N);
		for (int i = 0; i < N; i++) {
			all_pts.insert(all_pts.end(), pts[i].begin(), pts[i].end());
		}
		auto it = all_pts.begin();
		for (auto [f, v] : by_freq) {
			for (int z = 0; z < f; z++) {
				ans[it->first][it->second] = v;
				++it;
			}
		}
		bool good = true;
		std::vector<int> cur_freq(N);
		for (int m = 0; m < M; m++) {
			int l = 0;
			for (int r = 0; r < S[m]; r++) {
				if (l == r - D[m]) {
					freq[ans[m][l]]--;
					l++;
				}
				if (freq[ans[m][r]]) good = false;
				freq[ans[m][r]]++;
			}
			for (; l < S[m]; l++) {
				freq[ans[m][l]]--;
			}
		}
			for (int m = 0; m < M; m++) {
				for (int j = 0; j < S[m]; j++) {
					cerr << ans[m][j] + 1 << " \n"[j+1==S[m]];
				}
			}
		if (good) {
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
