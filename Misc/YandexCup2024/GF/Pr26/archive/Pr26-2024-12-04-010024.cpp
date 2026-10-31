#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	std::vector<int> A(N);
	for (auto& a : A) { cin >> a; a--; }
	auto solve = [&]() -> std::pair<int, int> {
		for (int l = 1; true; l++) {
			std::set<std::vector<int>> has;
			for (int i = 0; i + l <= N; i++) {
				std::vector<int> cnd(A.begin() + i, A.begin() + i + l);
				auto [_, did_insert] = has.insert(cnd);
				if (i + l == N && did_insert) {
					return {l, 0};
				}
			}
			for (int off = 1; off <= l; off++) {
				std::vector<int> cnd(l - off, 0);
				while (true) {
					{
						// check cnd
						if (!has.count(cnd)) {
							std::set<std::vector<int>> new_has;
							std::vector<int> cur; cur.insert(cur.end(), A.begin() + std::max(0, N - l), A.end());
							for (int z = 0; z < int(cnd.size()); z++) {
								// append cnd[z]
								cur.push_back(cnd[z]);
								while (cur.size() > l) cur.erase(cur.begin());
								new_has.insert(cur);
							}
							if (!new_has.count(cnd)) {
								return {l, off};
							}
						}
					}
					{
						int cur = 0;
						while (cur < int(cnd.size()) && cnd[cur] == K-1) {
							cnd[cur] = 0;
							cur++;
						}
						if (cur == int(cnd.size())) break;
						cnd[cur]++;
					}
				}
			}
		}
	};

	auto [d, s] = solve();
	cout << d << ' ' << s << '\n';

	return 0;
}
