#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int K, J; int N; cin >> K >> J >> N;
	int ans = N;
	for (int z = 0; z < 2; z++, std::swap(K, J)) {
		int a = K, b = J;
		int cnd = 0;
		while (a <= N || b <= N) {
			if (a == b) a--;
			cnd++;
			if (a < b) a += K;
			else b += J;
		}
		if (cnd < ans) ans = cnd;
	}
	cout << ans << '\n';
	return 0;
}
