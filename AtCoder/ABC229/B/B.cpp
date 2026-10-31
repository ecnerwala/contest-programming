#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t A, B; cin >> A >> B;
	auto dsum = [&](int64_t a) -> int {
		int r = 0;
		for (; a; a /= 10) r += int(a / 10);
		return r;
	};
	cout << (dsum(A+B) != dsum(A) + dsum(B) ? "Hard" : "Easy") << '\n';

	return 0;
}
