#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int cnt_odd = 0;
		for (int i = 0; i < N; i++) {
			int a; cin >> a;
			if (a & 1) cnt_odd++;
		}
		cout << (cnt_odd == N ? N-1 : cnt_odd + 1) << '\n';
	}

	return 0;
}
