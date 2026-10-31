#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		for (int z = 1; z <= 1000; z++) {
			cout << z*z << '\n' << flush;
			int resp; cin >> resp;
			if (resp == -1) exit(0);
			else if (resp == 0) continue;
			else if (resp == 1) break;
			else assert(false);
		}
	}

	return 0;
}
