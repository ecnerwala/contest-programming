#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;

	std::vector<std::set<string>> lists(N);
	while (Q--) {
		int op; int i; string s; cin >> op >> i >> s; i--;
		if (op == 1) {
			cout << lists[i].count(s) << '\n';
		} else {
			lists[i].insert(s);
		}
	}

	return 0;
}
