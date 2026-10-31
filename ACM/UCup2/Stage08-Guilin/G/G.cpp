#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		std::string S; cin >> S;
		std::string ops; ops.reserve(2 * int(S.size()));
		std::string stk; stk.reserve(S.size());
		for (char c : S) {
			if (c == '(') {
				ops.push_back(c);
				stk.push_back(')');
			} else if (c == ')') {
				ops.push_back(c);
				if (!stk.empty() && stk.back() == ')') {
					stk.pop_back();
				}
			} else assert(false);
		}
		if (stk.empty()) {
			cout << ops << '\n';
		} else {
			cout << "impossible" << '\n';
		}
	}

	return 0;
}
