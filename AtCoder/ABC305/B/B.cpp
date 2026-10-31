#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::vector<int> pos{0, 3, 4, 8, 9, 14, 23};
	char a, b; cin >> a >> b;
	std::cout << std::abs(pos[a - 'A'] - pos[b - 'A']) << '\n';

	return 0;
}
