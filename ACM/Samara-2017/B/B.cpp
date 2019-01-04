#include<bits/stdc++.h>
using namespace std;

const string H = "happiness";

int main() {
	ios_base::sync_with_stdio(0);
	string S;
	cin >> S;
	assert(S.size() >= 2);

	size_t h1 = S.find(H);
	if (h1 == string::npos) {
		swap(S[0], S[1]);
		if (S.find(H) == string::npos) {
			cout << "YES" << '\n';
			cout << 1 << ' ' << 2 << '\n';
			return 0;
		} else {
			swap(S[0], S[1]);
			assert(S.size() >= 3);
			swap(S[0], S[2]);
			assert(S.find(H) == string::npos);
			cout << "YES" << '\n';
			cout << 1 << ' ' << 3 << '\n';
			return 0;
		}
	} else {
		size_t h2 = S.find(H, h1+1);
		if (h2 == string::npos) {
			swap(S[h1 + 1], S[h1 + 2]);
			assert(S.find(H) == string::npos);
			cout << "YES" << '\n';
			cout << h1 + 2 << ' ' << h1 + 3 << '\n';
			return 0;
		}
		size_t h3 = S.find(H, h2 + 1);
		if (h3 == string::npos) {
			swap(S[h1+1], S[h2+2]);
			assert(S.find(H) == string::npos);
			cout << "YES" << '\n';
			cout << h1 + 2 << ' ' << h2 + 3 << '\n';
			return 0;
		}
		cout << "NO\n";
		return 0;
	}
	assert(false);

	return 0;
}
