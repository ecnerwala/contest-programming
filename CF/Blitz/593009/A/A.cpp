#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;
	std::reverse(A.begin(), A.end());
	std::set<int> s;
	int ans = -1;
	for (auto a : A) {
		auto [_, did_ins] = s.insert(a);
		if (did_ins) ans = a;
	}
	cout << ans << '\n';

	return 0;
}
