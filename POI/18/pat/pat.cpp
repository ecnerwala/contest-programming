#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::vector<pair<int, int>> sticks; sticks.reserve(int(1e6) + 20);
	int K; cin >> K;
	for (int k = 1; k <= K; k++) {
		int N; cin >> N;
		while (N--) {
			int l; cin >> l;
			sticks.push_back({l, k});
		}
	}
	sort(sticks.begin(), sticks.end());
	vector<pair<int, int>> opt; opt.reserve(4);
	for (auto s : sticks) {
		for (int i = 0; i < int(opt.size()); i++) {
			if (opt[i].second == s.second) {
				opt.erase(opt.begin()+i);
				break;
			}
		}
		if (opt.size() == 3) {
			opt.erase(opt.begin());
		}
		opt.push_back(s);
		if (opt.size() == 3) {
			if (opt[0].first + opt[1].first > opt[2].first) {
				vector<int> res; res.reserve(6);
				for (auto r : opt) {
					res.push_back(r.second);
					res.push_back(r.first);
				}
				for (int i = 0; i < 6; i++) {
					cout << res[i] << " \n"[i+1==6];
				}
				exit(0);
			}
		}
	}
	cout << "NIE" << '\n';

	return 0;
}
