#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	vector<string> words;
	string W;
	while (cin >> W) {
		if ('A' <= W[0] && W[0] <= 'Z') {
			W[0] += 'a' - 'A';
			reverse(W.begin(), W.end());
			W[0] -= 'a' - 'A';
		} else {
			reverse(W.begin(), W.end());
		}
		words.push_back(W);
	}
	reverse(words.begin(), words.end());
	for (int i = 0; i < int(words.size()); i++) {
		cout << words[i] << " \n"[i+1==int(words.size())];
	}

	return 0;
}
