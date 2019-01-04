#include<bits/stdc++.h>
using namespace std;

const int MAXN = 3e5;
int N;
const int MAXM = 3e5;
int M;

bool alive[MAXN];
bool type[MAXN];
vector<int> adj[MAXN];
int cnt[MAXN][2];

int main() {
	ios_base::sync_with_stdio(0); cin.tie(nullptr);
	cin >> N >> M;
	{
		string s; cin >> s;
		assert(int(s.size()) == N);
		for (int i = 0; i < N; i++) {
			char c = s[size_t(i)];
			assert(c == 'A' || c == 'B');
			type[i] = c - 'A';
		}
	}
	for (int i = 0; i < M; i++) {
		int a, b; cin >> a >> b;
		a--, b--;

		adj[a].push_back(b);
		cnt[b][type[a]] ++;

		adj[b].push_back(a);
		cnt[a][type[b]] ++;
	}

	for (int i = 0; i < N; i++) {
		alive[i] = true;
	}

	vector<int> rem;
	for (int i = 0; i < N; i++) {
		if (cnt[i][0] == 0 || cnt[i][1] == 0) {
			alive[i] = false;
			rem.push_back(i);
		}
	}
	for (int z = 0; z < int(rem.size()); z++) {
		int cur = rem[z];
		assert(!alive[cur]);
		for (int nxt : adj[cur]) {
			if (!alive[nxt]) continue;
			if ((--cnt[nxt][type[cur]]) == 0) {
				alive[nxt] = false;
				rem.push_back(nxt);
			}
		}
	}
	if (int(rem.size()) == N) {
		cout << "No" << '\n';
	} else {
		cout << "Yes" << '\n';
	}


	return 0;
}
