#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	for (int i = 0; i < N; i++) {
		cout << (2*i+0)%N+1 << ' ' << (2*i+1)%N+1 << '\n';
	}

	// i -> 2i + 0/1 -> 4i + 0/2 + 0/1 -> 8i + 0/4 + 0/2 + 0/1 ...
	// 1024i + 0..1023 % N

	return 0;
}
