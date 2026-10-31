#include<bits/stdc++.h>
using namespace std;

const int MAXN = 5e3;
const int MAXM = 1.1e4;
int N, M;
pair<int, int> X[MAXM];

int par[MAXN];
int sz[MAXN];
void reset() {
	memset(par, -1, sizeof(par));
	for (int i = 0; i < N; i++) sz[i] = 1;
}
int getPar(int a) {
	return par[a] == -1 ? a : (par[a] = getPar(par[a]));
}

bool merge(int a, int b) {
	a = getPar(a);
	b = getPar(b);
	if (a == b) return false;
	if (sz[a] < sz[b]) swap(a, b);
	// sz[a] >= sz[b]
	par[b] = a;
	sz[a] += sz[b];
	return true;
}

int knap[MAXN][MAXN];

string go() {
	cin >> N >> M;
	for (int i = 0; i < M; i++) {
		cin >> X[i].first >> X[i].second;
		X[i].first --, X[i].second --;
	}

	reset();

	unordered_map<int, int> mp;
	for (int i = 0; i < M; i++) {
		int x = X[i].first, y = X[i].second;
		mp[x+y] = max(mp[x+y], y-x+1); // max length
	}
	for (auto it : mp) {
		int sum = it.first, diff = it.second - 1;
		assert((sum & 1) == (diff & 1));
		int x = (sum - diff) / 2;
		int y = (sum + diff) / 2;
		//cerr << x << ' ' << y << '\n';
		for (int a = x, b = y; a < b; a++, b--) {
			merge(a, b);
		}
	}

	vector<pair<int, int>> groups;
	for (int i = 0; i < N; i++) {
		if (getPar(i) != i) continue;
		groups.emplace_back(i, sz[i]);
		//cerr << "group " << i << ' ' << sz[i] << '\n';
	}
	memset(knap, -1, sizeof(knap)); // -1 is impossible, 0 is take noone, 1 is take one
	knap[0][0] = 0;
	int maxLen = 0;
	for (int z = 0; z < int(groups.size()); z++) {
		for (int i = 0; i <= maxLen; i++) {
			if (knap[z][i] == -1) continue;
			knap[z+1][i] = 0;
			knap[z+1][i + groups[z].second] = 1;
		}
		maxLen += groups[z].second;
	}
	assert(maxLen == N);

	assert(knap[int(groups.size())][0] == 0);
	for (int i = 0; i <= N-i; i++) {
		assert((knap[int(groups.size())][i] != -1) == (knap[int(groups.size())][N-i] != -1));
	}

	int bestOnes = N/2;
	while (knap[int(groups.size())][bestOnes] == -1) {
		bestOnes--;
		assert(bestOnes >= 0);
	}
	//cerr << bestOnes << '\n';

	vector<int> vals(N);
	for (int z = int(groups.size()) - 1, curOnes = bestOnes; z >= 0; z--) {
		int v = knap[z+1][curOnes];
		assert(v == 0 || v == 1);
		vals[groups[z].first] = v;
		curOnes -= v * groups[z].second;
	}
	string ans(N, '?');
	for (int i = 0; i < N; i++) {
		ans[i] = char('0' + vals[getPar(i)]);
	}
	for (int i = 0; i < M; i++) {
		for (int x = X[i].first, y = X[i].second; x < y; x++, y--) {
			assert(ans[x] == ans[y]);
		}
	}
	int cnt[2] = {0, 0};
	for (int i = 0; i < N; i++) {
		cnt[ans[i] - '0'] ++;
	}
	assert(cnt[1] == bestOnes);
	return ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		//cerr << "Case #" << t << ":" << '\n';
		string ans = go();
		cout << "Case #" << t << ": " << ans << '\n';
	}

	return 0;
}
