#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> T(N);
	for (auto& t : T) {
		std::string s; int a, b, c; cin >> c >> a >> b >> c;
		if (a == -1) a = 301;
		if (b == -1) b = 301;
		if (c == -1) c = 301;
		t = std::min({a, b, c});
	}
	std::sort(T.begin(), T.end());
	int ans = 0;
	int cur = 300;
	for (auto t : T) {
		if (t <= cur) {
			cur -= t;
			ans++;
		}
	}
	cout << ans << '\n';

	return 0;
}
