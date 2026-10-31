#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int K, X; cin >> K >> X;
	std::string pass(K, '0');
	std::iota(pass.begin(), pass.end(), '0');
	do {
		do {
			cout << pass << '\n';
		} while (next_permutation(pass.begin() + X, pass.end()));
	} while (next_permutation(pass.begin(), pass.begin() + X));

	return 0;
}
