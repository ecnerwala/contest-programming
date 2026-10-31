#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, A, B; cin >> N >> A >> B;
	for (int i = 0; i < N; i++) {
		for (int z = 0; z < A; z++) {
			for (int j = 0; j < N; j++) {
				for (int y = 0; y < B; y++) {
					cout << ".#"[(i+j)&1];
				}
			}
			cout << '\n';
		}
	}

	return 0;
}
