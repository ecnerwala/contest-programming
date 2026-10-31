#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::multiset<int> ends;
	std::multiset<int> starts;
	int Q; cin >> Q;
	while (Q--) {
		char op; cin >> op;
		int l, r; cin >> l >> r;
		if (op == '+') {
			ends.insert(r);
			starts.insert(l);
		} else if (op == '-') {
			ends.erase(ends.find(r));
			starts.erase(starts.find(l));
		}
		cout << (!ends.empty() && *ends.begin() < *starts.rbegin() ? "YES" : "NO") << '\n';
	}

	return 0;
}
