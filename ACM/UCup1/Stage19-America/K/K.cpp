#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int depth = 0;
	std::vector<std::array<int, 3>> eqs; eqs.reserve(N);
	for (int i = 0; i < N; i++) {
		string s; cin >> s;
		assert(s.back() == '{' || s.back() == '}');
		char v = s.back(); s.pop_back();
		if (v == '}') depth--;

		int num_spaces = 0;
		int num_tabs = 0;
		for (char c : s) {
			if (c == 's') {
				num_spaces++;
			} else if (c == 't') {
				num_tabs++;
			} else assert(false);
		}

		if (depth == 0) {
			if (num_tabs || num_spaces) {
				cout << -1 << '\n';
				exit(0);
			} else {
				// noop
			}
		} else {
			eqs.push_back({depth, num_tabs, num_spaces});
		}

		if (v == '{') depth ++;
	}
	if (eqs.empty()) {
		cout << 1 << '\n';
		exit(0);
	}
	assert(eqs[0][0] == 1);
	int ans = -1;
	for (int z = 1; z < int(eqs.size()); z++) {
		int c = eqs[z][0];
		for (int d = 0; d < 3; d++) {
			eqs[z][d] -= c * eqs[0][d];
		}
		assert(eqs[z][0] == 0);
		if (eqs[z][1] == 0 && eqs[z][2] == 0) {
			continue;
		}
		if (eqs[z][2] == 0 && eqs[z][1] != 0) {
			cout << -1 << '\n';
			exit(0);
		}
		if (eqs[z][2] != 0 && eqs[z][1] == 0) {
			cout << -1 << '\n';
			exit(0);
		}
		if (eqs[z][2] % eqs[z][1] != 0) {
			cout << -1 << '\n';
			exit(0);
		}
		int cnd = -eqs[z][2] / eqs[z][1];
		if (ans == -1) {
			ans = cnd;
		}
		if (ans != cnd) {
			cout << -1 << '\n';
			exit(0);
		}
	}
	if (ans == -1) {
		cout << 1 << '\n';
	} else {
		cout << ans << '\n';
	}

	return 0;
}
