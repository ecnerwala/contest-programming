#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> P(N);
	P[0] = -1;
	for (int i = 1; i < N; i++) { cin >> P[i]; P[i] --; }

	std::vector<std::vector<int>> ch(N);
	for (int i = 1; i < N; i++) {
		ch[P[i]].push_back(i);
	}

	auto join = [&](const std::vector<int>& a, const std::vector<int>& b) {
		std::vector<int> res; res.reserve((a.size() + 1) * (b.size() + 1) - 1);
		res.insert(res.end(), b.begin(), b.end());
		for (int i = 0; i < int(a.size()); i++) {
			res.push_back(a[i]);
			if (i & 1) {
				res.insert(res.end(), b.begin(), b.end());
			} else {
				res.insert(res.end(), b.rbegin(), b.rend());
			}
		}
		return res;
	};
	std::vector<std::vector<int>> seq(N);
	for (int i = N-1; i >= 0; i--) {
		for (int c : ch[i]) {
			seq[i] = join(seq[i], seq[c]);
		}
		seq[i].insert(seq[i].begin(), i);
	}

	cout << seq[0].size()-1 << '\n';
	for (int z = 1; z < int(seq[0].size()); z++) {
		cout << seq[0][z]+1 << " \n"[z+1==int(seq[0].size())];
	}

	return 0;
}
