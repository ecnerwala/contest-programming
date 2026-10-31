#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	// at most 9 digits
	std::vector<std::vector<int>> mtns;
	for (int d = 1; d <= 9; d++) {
		mtns.push_back(std::vector<int>(1, d));
	}
	for (int z = 0; z < int(mtns.size()); z++) {
		if (int(mtns[z].size()) >= 17) break;
		if (z % 1000 == 0) cerr << z << '\n';
		for (int a = 1; a <= (mtns[z].front() - (int(mtns.size()) == 1)); a++) {
			for (int b = 1; b <= (mtns[z].back() - int(mtns.size() == 1)); b++) {
				std::vector<int> nxt = mtns[z];
				nxt.insert(nxt.begin(), a);
				nxt.insert(nxt.end(), b);
				mtns.push_back(nxt);
			}
		}
	}

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		cout << "Case #" << case_num << ": ";
	}

	return 0;
}
