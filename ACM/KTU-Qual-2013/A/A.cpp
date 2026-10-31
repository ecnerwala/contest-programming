#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		int N; cin >> N;
		for (int i = 0; i < N-1; i++) { int a, b; cin >> a >> b; }
		cout << "Case #" << t << ": " << min(N, 2) << '\n';
	}

	return 0;
}
