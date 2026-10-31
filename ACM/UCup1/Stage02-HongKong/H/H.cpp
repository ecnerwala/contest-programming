#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t L, R, B, K; cin >> L >> R >> B >> K;
	cout << (L+B-1)/B*B * K << '\n';

	return 0;
}
