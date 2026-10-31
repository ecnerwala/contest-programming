#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	string S; cin >> S;
	int N = int(S.size());
	cout << 3 << '\n';
	cout << 'R' << ' ' << N-1 << '\n';
	cout << 'L' << ' ' << N << '\n';
	cout << 'L' << ' ' << 2 << '\n';

	return 0;
}

// abac
// abaca
// cababaca
// acababaca
