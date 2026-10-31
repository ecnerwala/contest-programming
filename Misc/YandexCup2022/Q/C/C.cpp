#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t N; cin >> N;
	assert(!(N&1));
	auto sq = [&](int a) -> int64_t { return int64_t(a) * int64_t(a+1); };
	int mk = 0;
	while (sq(mk) <= N) mk++;
	mk--;
	if (sq(mk) == N) {
		cout << 1 << '\n';
		exit(0);
	}
	for (int i = 1, j = mk; j >= 0; ) {
		if (sq(i) + sq(j) > N) j--;
		else if (sq(i) + sq(j) < N) i++;
		else {
			cout << 2 << '\n';
			exit(0);
		}
	}
	cout << 3 << '\n';
	exit(0);

	return 0;
}
