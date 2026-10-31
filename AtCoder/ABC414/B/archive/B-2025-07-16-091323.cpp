#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::string S; S.reserve(100);
	for (int i = 0; i < N; i++) {
		char c; int l; cin >> c >> l;
		if (int(S.size()) + l > 100) {
			cout << "Too Long" << '\n';
			exit(0);
		} else {
			S.insert(S.end(), l, c);
		}
	}
	cout << S << '\n';

	return 0;
}
