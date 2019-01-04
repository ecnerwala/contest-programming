#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e6;
int N, K;
char S[2][MAXN];
vector<pair<char, int>> val[2];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> K;
	for (int z = 0; z < 2; z++) {
		cin >> S[z];
		for (int i = 0; i < N; i++) {
			char c = S[z][i];
			if (val[z].empty() || val[z].back().first != c) {
				val[z].emplace_back(c, 0);
			}
			assert(val[z].back().first == c);
			if (K == ++val[z].back().second) {
				val[z].pop_back();
			}
		}
	}
	cout << (val[0] == val[1] ? "Yes" : "No") << '\n';

	return 0;
}
