#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, Q; cin >> N >> Q;
		int a = -1;
		while (N--) { int v; cin >> v; a = std::max(a, v); }
		while (Q--) {
			char op; int l, r; cin >> op >> l >> r;
			if (l <= a && a <= r) a += 44 - op;
			cout << a << " \n"[!Q];
		}
	}

	return 0;
}
