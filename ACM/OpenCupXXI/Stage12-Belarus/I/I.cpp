#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, M, K; cin >> N >> M >> K;
	int res = 0;
	for (int i = 0; i < N; i++) {
		int a; cin >> a;
		if (a == 0) res++;
	}
	for (int j = 0; j < M; j++) {
		int b; cin >> b;
		if (b == 1) res--;
	}
	cout << abs(res) << '\n';

	return 0;
}
