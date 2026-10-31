#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<vector<int>> vals(int(2.1e5));
	for (int i = 0; i < N; i++) {
		int a, c; cin >> a >> c;
		vals[c].push_back(a);
	}
	bool isGood = true;
	for (const auto& it : vals) {
		if (!is_sorted(it.begin(), it.end())) {
			isGood = false;
			break;
		}
	}
	cout << (isGood ? "YES" : "NO") << '\n';

	return 0;
}
