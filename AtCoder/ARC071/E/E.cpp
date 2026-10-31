#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	string S, T; cin >> S >> T;
	vector<int8_t> X(S.size()+1);
	vector<int8_t> Y(T.size()+1);
	for (int i = 0; i < int(S.size()); i++) X[i+1] = (X[i] + (S[i] == 'A' ? 1 : 2)) % 3;
	for (int i = 0; i < int(T.size()); i++) Y[i+1] = (Y[i] + (T[i] == 'A' ? 1 : 2)) % 3;

	int Q; cin >> Q;
	while (Q--) {
		int s1, s2, t1, t2; cin >> s1 >> s2 >> t1 >> t2; s1--, t1--;
		cout << ((X[s2] - X[s1] - Y[t2] + Y[t1]) % 3 == 0 ? "YES" : "NO") << '\n';
	}

	return 0;
}
