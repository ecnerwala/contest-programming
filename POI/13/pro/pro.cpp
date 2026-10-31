#include<iostream>
#include<cstring>
#include<vector>
#include<cassert>

using namespace std;

const int INF = 36501;

const int MAXN = int(1e6) + 20;
const int MAXM = int(1e6) + 20;

int N;
int M;
pair<int, int> edges[MAXM];
int st[MAXN];
bool isGood[MAXN];

int inDeg[MAXN];
uint16_t numWays[MAXN];

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	cin >> N >> M;
	N++;
	for(int e = 0; e < M; e++) {
		int a, b; cin >> a >> b;
		if(a == N) a = 0;
		if(b == N) b = 0;
		edges[e] = {b, a};
		st[b]++;
	}
	edges[M] = {-1, -1};
	for (int i = 0; i < N; i++) {
		st[i+1] += st[i];
	}
	assert(st[N-1] == M && st[N] == M);
	// because we're so cramped on memory, we'll use inDeg as a buffer
	{
		for(int e = M-1; e >= 0; e--) {
			int b = edges[e].first;
			int a = edges[e].second;
			inDeg[--st[b]] = a;
		}
		for (int e = 0; e < M; e++) {
			edges[e].first = inDeg[e];
			inDeg[e] = 0;
		}
	}
	vector<int> q; q.reserve(N);
	isGood[0] = true;
	q.push_back(0);
	while(!q.empty()) {
		int cur = q.back(); q.pop_back();
		for (int z = st[cur]; z < st[cur+1]; z++) {
			int nxt = edges[z].first;
			inDeg[nxt]++;
			if(isGood[nxt]) continue;
			isGood[nxt] = true;
			q.push_back(nxt);
		}
	}
	for(int i = 0; i < N; i++) {
		if(!isGood[i]) continue;
		if(inDeg[i] == 0) q.push_back(i);
	}
	numWays[0] = 1;
	while(!q.empty()) {
		int cur = q.back(); q.pop_back();
		for (int z = st[cur]; z < st[cur+1]; z++) {
			int nxt = edges[z].first;
			if(!isGood[nxt]) continue;
			numWays[nxt] = uint16_t(min(INF, numWays[nxt] + numWays[cur]));
			inDeg[nxt]--;
			if(inDeg[nxt] == 0) {
				q.push_back(nxt);
			}
		}
	}

	for(int i = 0; i < N; i++) {
		if(!isGood[i]) continue;
		if(inDeg[i] != 0) numWays[i] = INF;
	}

	//for(int i = 0; i < N; i++) cerr << numWays[i] << ' '; cerr << '\n';

	int maxVal = 0;
	for(int i = 1; i < N; i++) {
		if(!isGood[i]) continue;
		if(numWays[i] > maxVal) maxVal = numWays[i];
	}
	if(maxVal == INF) cout << "zawsze" << '\n';
	else cout << maxVal << '\n';

	int cntWays = 0;
	for(int i = 1; i < N; i++) {
		if(!isGood[i]) continue;
		if(numWays[i] == maxVal) cntWays ++;
	}
	cout << cntWays << '\n';

	for(int i = 1; i < N; i++) {
		if(!isGood[i]) continue;
		if(numWays[i] == maxVal) {
			cout << i << ((--cntWays) ? ' ' : '\n');
		}
	}
}
