#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::vector<int> a;
	std::vector<int> b;
	b.push_back(50);
	a.push_back(50);
	a.push_back(25);
	for (int i = 1; i <= 20; i++) {
		a.push_back(i);
		a.push_back(2*i);
		a.push_back(3*i);
		b.push_back(2*i);
	}

	int N; cin >> N;
	for (auto x : b) {
		if (x == N) {
			cout << "YES" << '\n';
			cout << 1 << '\n';
			cout << x << '\n';
			exit(0);
		}
		for (int y : a) {
			if (x + y == N) {
				cout << "YES" << '\n';
				cout << 2 << '\n';
				cout << x << ' ' << y << '\n';
				exit(0);
			}
			for (int z : a) {
				if (x + y + z == N) {
					cout << "YES" << '\n';
					cout << 2 << '\n';
					cout << x << ' ' << y << '\n';
					exit(0);
				}
			}
		}
	}
	cout << "NO" << '\n';

	return 0;
}
