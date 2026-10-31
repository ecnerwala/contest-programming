#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<int> B;
	for (int i = 0; i < N; i++) {
		int b; cin >> b; b--;
		B.push_back(b);
	}

	vector<int> ops;
	while (!B.empty()) {
		int cur = -1;
		for (int i = 0; i < int(B.size()); i++) {
			if (B[i] == i) {
				cur = i;
			}
		}
		if (cur == -1) {
			cout << -1 << '\n';
			exit(0);
		}
		assert(B[cur] == cur);
		ops.push_back(cur);
		B.erase(B.begin() + cur);
	}
	reverse(ops.begin(), ops.end());
	for (int i : ops) {
		cout << i+1 << '\n';
	}

	return 0;
}
