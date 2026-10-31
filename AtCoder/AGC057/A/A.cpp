#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int L, R; cin >> L >> R;
		string lo = std::to_string(R);
		if (lo.front() == '1') {
			lo.erase(lo.begin());
		} else {
			lo = std::string(lo.size()-1, '9');
		}
		if (lo.empty()) lo = "0";
		cout << R - std::max({std::stoi(lo), R / 10, L-1}) << '\n';
	}

	return 0;
}
