#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e6;
int N;
char S[MAXN];
int prv[MAXN];
int nxt[MAXN];
bool ded[MAXN];

unordered_set<int> toRem[3];
int cnt[3];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	string _S; cin >> _S;
	S[0] = '^';
	for (char c : _S) {
		if (c != S[N]) {
			S[++N] = c;
		}
	}
	S[N+1] = '$';
	S[N+2] = 0;
	
	//cerr << S << '\n';

	for (int i = 1; i <= N; i++) {
		prv[i] = i-1;
		nxt[i] = i+1;
	}

	auto canRemove = [&](int i) {
		return (1 <= i && i <= N) && S[prv[i]] != S[i] && S[nxt[i]] != S[i] && S[prv[i]] != S[nxt[i]];
	};

	for (int i = 1; i <= N; i++) {
		cnt[S[i] - 'A'] ++;
		if (canRemove(i)) toRem[S[i] - 'A'].insert(i);
	}

	while (*max_element(cnt, cnt + 3) > *min_element(cnt, cnt + 3)) {
		vector<pair<int, int>> cnds;
		for (int z = 0; z < 3; z++) {
			if (!toRem[z].empty()) {
				cnds.emplace_back(cnt[z], *toRem[z].begin());
			}
		}
		int i = max_element(cnds.begin(), cnds.end())->second;

		toRem[S[i] - 'A'].erase(i);
		if (canRemove(prv[i])) {
			toRem[S[prv[i]] - 'A'].erase(prv[i]);
		}
		if (canRemove(nxt[i])) {
			toRem[S[nxt[i]] - 'A'].erase(nxt[i]);
		}

		nxt[prv[i]] = nxt[i];
		prv[nxt[i]] = prv[i];
		ded[i] = true;
		cnt[S[i] - 'A'] --;

		if (canRemove(prv[i])) {
			toRem[S[prv[i]] - 'A'].insert(prv[i]);
		}
		if (canRemove(nxt[i])) {
			toRem[S[nxt[i]] - 'A'].insert(nxt[i]);
		}
	}

	for (int i = 1; i <= N; i++) {
		if (!ded[i]) cout << S[i];
	}
	cout << '\n';

	return 0;
}
