#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::array<std::string, 2> vals;
	for (int z = 0; z < 2; z++) {
		std::string inp; cin >> inp;
		std::string v;
		for (char c : inp) {
			if (c == 'n') v += '1';
			else if (c == 'z') v += '0';
		}
		vals[z] = std::string(2000 - int(v.size()), '0') + v;
	}
	if (vals[0] < vals[1]) cout << '<' << '\n';
	else if (vals[0] > vals[1]) cout << '>' << '\n';
	else cout << '=' << '\n';

	return 0;
}
