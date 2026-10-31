#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int X; cin >> X;
	vector<int> facs;
	for (int f = 1; f * f <= X; f++) {
		if (X % f == 0) {
			facs.push_back(f);
			facs.push_back(X/f);
		}
	}
	using i64 = int64_t;
	auto p5 = [](i64 a) { return a * a * a * a * a; };
	for (int f : facs) {
		for (int v = 0; )
	}

	return 0;
}
