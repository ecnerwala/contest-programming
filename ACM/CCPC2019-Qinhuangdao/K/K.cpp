#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e6;
int N;
int P[MAXN];
vector<int> ch[MAXN];

bool hasOdd;
void dfs(int cur, int len) {
	len++;
	if (ch[cur].size() == 0) {
		if (len % 2 == 1) {
			hasOdd = true;
		}
		return;
	}

	if (ch[cur].size() != 1) {
		len = 0;
	}
	for (int nxt : ch[cur]) {
		dfs(nxt, len);
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	while (T--) {
		cin >> N;
		P[1] = 0;
		for (int i = 2; i <= N; i++) {
			cin >> P[i];
		}
		P[N+1] = 0;
		P[N+2] = N+1;
		N += 2;

		for (int i = 1; i <= N; i++) {
			ch[P[i]].push_back(i);
		}

		hasOdd = false;
		dfs(0, 0);

		if (hasOdd) {
			cout << "Takeru" << '\n';
		} else {
			cout << "Meiya" << '\n';
		}

		for (int i = 0; i <= N; i++) { ch[i] = {}; }
	}

	return 0;
}
