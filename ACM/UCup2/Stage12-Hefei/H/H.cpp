#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	for (int i = 1; i <= 100; i++) {
		if (i / 2 + i / 3 + i / 5 + i / 7 < i) cerr << i << '\n';
	}

	return 0;
}
