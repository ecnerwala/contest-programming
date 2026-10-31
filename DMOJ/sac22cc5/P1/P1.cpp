#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, C; cin >> N >> C;
	for (int i = 0; i < N; i++) {
		int x; cin >> x;
		cout << x + 2 * C << " \n"[i+1==N];
	}

	return 0;
}
