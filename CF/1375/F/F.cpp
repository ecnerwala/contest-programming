#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	array<int64_t, 3> A;
	for (auto& a : A) cin >> a;
	cout << "First" << '\n';
	auto move = [&](int64_t v) {
		cout << v << '\n' << flush;
		int x; cin >> x;
		if (!x) exit(0);
		x--;
		A[x] += v;
	};

	move(max({A[0], A[1], A[2]}) - min({A[0], A[1], A[2]}));
	move(max({A[0], A[1], A[2]}) * 3 - (A[0] + A[1] + A[2]));
	move((max({A[0], A[1], A[2]}) - min({A[0], A[1], A[2]})) >> 1);
	assert(false);

	return 0;
}
