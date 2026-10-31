#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		cerr << "Case #" << case_num << '\n';

		int N; cin >> N; N /= 4;
		cerr << "N = " << N << '\n';
		std::vector<int> A1(N);
		for (auto& a : A1) cin >> a;
		std::vector<int> B1(N);
		for (auto& a : B1) cin >> a;
		std::vector<int> A2(N);
		for (auto& a : A2) cin >> a;
		std::vector<int> B2(N);
		for (auto& a : B2) cin >> a;

		std::vector<std::pair<int, bool>> inps(N, {-1, true});
		auto play_round = [&](std::vector<int> vals) {
			std::sort(vals.begin(), vals.end(), std::greater<>());
			std::sort(inps.begin(), inps.end(), std::greater<>());
			std::vector<std::pair<int, bool>> outs; outs.reserve(N);
			int j = 0;
			for (int i = 0; i < N; i++) {
				assert(j <= i);
				if (!inps[i].second) {
					continue;
				} else if (vals[j] > inps[i].first) {
					outs.push_back({vals[j], true});
					j++;
				} else {
					outs.push_back({inps[i].first, false});
				}
			}
			for (int i = N-1; i >= 0; i--) {
				if (inps[i].second) continue;
				assert(j < N);
				outs.push_back({std::max(inps[i].first, vals[j]), true});
				j++;
			}
			assert(int(outs.size()) == N);
			inps = outs;
			std::sort(inps.begin(), inps.end(), std::greater<>());
			//for (auto [x, y] : inps) { cerr << x << ' ' << y << '\n'; } cerr << '\n';
		};

		play_round(A1);
		play_round(B1);
		play_round(A2);
		play_round(B2);

		int ans = 0;
		for (int i = 0; i < N; i++) {
			ans += !inps[i].second;
		}
		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
