#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		int v = -1;
		bool bad = false;
		for (auto& a : A) {
			cin >> a;
			if (v == -1) v = a;
			if (a != -1 && a != v) bad = true;
		}
		if (v == 0) bad = true;
		cout << (bad ? "NO" : "YES") << '\n';
	}

	return 0;
}
