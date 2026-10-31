#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, Q; cin >> N >> Q;
		int a = -1;
		for (int i = 0; i < N; i++) { int v; cin >> v; a = std::max(a, v); }
		for (int q = 0; q < Q; q++) {
			char op; int l, r; cin >> op >> l >> r;
			if (l <= a && a <= r) {
				if (op == '+') a++;
				else if (op == '-') a--;
				else assert(false);
			}
			cout << a << " \n"[q+1==Q];
		}
	}

	return 0;
}
