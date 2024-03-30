#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int cur_mex = 0;
		for (int i = 0; i < N; i++) {
			int a; cin >> a;
			assert(a != 0);
			if (a > 0) {
				cur_mex += a;
			}
			int p = cur_mex - a;
			cout << p << " \n"[i+1==N];
		}
	}

	return 0;
}
