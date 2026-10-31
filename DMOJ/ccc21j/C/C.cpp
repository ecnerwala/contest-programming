#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	bool last = false;
	while (true) {
		int v; cin >> v;
		if (v == 99999) break;
		int l = v % 1000;
		int a = v / 1000;
		a = a%10 + a/10;
		if (a & 1) last = false;
		else if (a) last = true;
		cout << (last ? "right" : "left") << ' ' << l << '\n';
	}

	return 0;
}
