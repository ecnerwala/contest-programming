#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXNLEAVES = 8.1e5;
const int MAXNVERTS = MAXNLEAVES * 2;
const int MAXNEDGES = 8.1e5;
const int MAXNTIME = 8.1e5;
const int MAXNQUERY = 8.1e5;
int NLEAVES;
int NVERTS, NEDGES, NTIME, NQUERY;
pair<int, int> edges[MAXNEDGES];
bool collapsed[MAXNEDGES];
pair<int, int> evts[MAXNTIME];
int startTime[MAXNLEAVES];
int endTime[MAXNLEAVES];

int par[MAXNVERTS][25];
int depth[MAXNVERTS];
int ch[MAXNVERTS][2];
int sz[MAXNVERTS];
int splitTime[MAXNVERTS];

vector<int> startValues[MAXNVERTS];
vector<int> endValues[MAXNVERTS];
int parValue[MAXNVERTS];
int pathValue[MAXNVERTS];
int forkValue[MAXNVERTS];

namespace dsu {
int compPar[MAXNVERTS];
void reset() {
	for (int i = 1; i <= NVERTS; i++) {
		memset(par[i], 0, sizeof(par[i]));
		ch[i][0] = ch[i][1] = 0;
		sz[i] = 1;
		compPar[i] = i;
	}
}

int getPar(int a) {
	assert(1 <= a && a <= NVERTS);
	return (compPar[a] == a) ? a : (compPar[a] = getPar(compPar[a]));
}
bool merge(int a, int b) {
	a = getPar(a), b = getPar(b);
	if (a == b) return false;
	int v = ++NVERTS;
	compPar[a] = compPar[b] = compPar[v] = v;
	memset(par[v], 0, sizeof(par[v]));
	par[a][0] = par[b][0] = v;
	ch[v][0] = a;
	ch[v][1] = b;
	sz[v] = sz[a] + sz[b];
	return true;
}
}

void dfsPar(int cur) {
	depth[cur] = depth[par[cur][0]] + 1;

	for (int i = 0; par[cur][i]; i++) {
		par[cur][i+1] = par[par[cur][i]][i];
	}
	if (cur > NLEAVES) {
		dfsPar(ch[cur][0]);
		dfsPar(ch[cur][1]);
	}
}

int vertAt(int x, int t) {
	assert(1 <= x && x <= NLEAVES);
	int z = 0;
	assert(splitTime[x] >= t);
	while (splitTime[par[x][z]] >= t) {
		z++;
	}
	assert(t > splitTime[par[x][z]]);
	while (z > 0) {
		z--;
		if (splitTime[par[x][z]] >= t) {
			x = par[x][z];
		}
	}
	assert(t > splitTime[par[x][0]]);
	assert(splitTime[x] >= t);
	return x;
}

void dfsTotValue(int cur) {
	if (cur > NLEAVES) {
		dfsTotValue(ch[cur][0]);
		dfsTotValue(ch[cur][1]);
	}

	sort(startValues[cur].begin(), startValues[cur].end());
	sort(endValues[cur].begin(), endValues[cur].end());
	parValue[cur] += int(endValues[cur].size());
	parValue[cur] -= int(startValues[cur].size());
	if (cur > NLEAVES) {
		parValue[cur] += parValue[ch[cur][0]];
		parValue[cur] += parValue[ch[cur][1]];
	}

	pathValue[cur] = int(startValues[cur].size());
	if (cur > NLEAVES) {
		pathValue[cur] += max(pathValue[ch[cur][0]], pathValue[ch[cur][1]]);
	}

	forkValue[cur] = int(startValues[cur].size());
	if (cur > NLEAVES) {
		forkValue[cur] += max(pathValue[ch[cur][0]] + pathValue[ch[cur][1]], max(forkValue[ch[cur][0]], forkValue[ch[cur][1]]));
	}
}

void precomp() {
	NLEAVES = NVERTS;
	for (int e = 1; e <= NEDGES; e++) {
		collapsed[e] = false;
	}
	for (int i = 1; i <= NLEAVES; i++) {
		startTime[i] = NTIME + 51;
		endTime[i] = NTIME + 52;
		splitTime[i] = NTIME + 53;
	}

	for (int t = 1; t <= NTIME; t++) {
		int op, v; tie(op, v) = evts[t];
		if (op == 1) {
			assert(1 <= v && v <= NEDGES);
			collapsed[v] = true;
		} else if (op == 2) {
			assert(1 <= v && v <= NLEAVES);
			startTime[v] = t;
		} else if (op == 3) {
			assert(1 <= v && v <= NLEAVES);
			endTime[v] = t;
		} else assert(false);
	}

	dsu::reset();
	splitTime[0] = -60; // never go here

	for (int e = 1; e <= NEDGES; e++) {
		if (!collapsed[e]) {
			if (dsu::merge(edges[e].first, edges[e].second)) {
				splitTime[NVERTS] = NTIME + 53;
			}
		}
	}
	for (int t = NTIME; t >= 1; t--) {
		int op, v; tie(op, v) = evts[t];
		if (op == 1) {
			assert(1 <= v && v <= NEDGES);
			if (dsu::merge(edges[v].first, edges[v].second)) {
				splitTime[NVERTS] = t;
			}
		}
	}

	assert(NVERTS == 2 * NLEAVES - 1);
	int ROOT = NVERTS;
	dfsPar(ROOT);

	for (int i = 1; i <= NVERTS; i++) {
		startValues[i] = endValues[i] = {};
		parValue[i] = 0;
	}
	for (int i = 1; i <= NLEAVES; i++) {
		assert(startTime[i] < endTime[i]);
		if (startTime[i] > NTIME) {
			continue;
		}
		startValues[vertAt(i, startTime[i])].push_back(startTime[i]);
		endValues[vertAt(i, endTime[i])].push_back(endTime[i]);
	}
	dfsTotValue(ROOT);
	/*
	for (int i = 1; i <= NVERTS; i++) {
		cerr << "vert " << i << ": ";
		cerr << "par=" << par[i][0] << ' ';
		cerr << "ch=" << ch[i][0] << ',' << ch[i][1] << ' ';
		cerr << "sz=" << sz[i] << ' ';
		cerr << "splitTime=" << splitTime[i] << ' ';
		cerr << "startValues=" << "["; for (int v : startValues[i]) { cerr << v << ","; } cerr << "]" << ' ';
		cerr << "endValues=" << "["; for (int v : endValues[i]) { cerr << v << ","; } cerr << "]" << ' ';
		cerr << "parValue=" << parValue[i] << ' ';
		cerr << "pathValue=" << pathValue[i] << ' ';
		cerr << "forkValue=" << forkValue[i] << ' ';
		cerr << '\n';
	}
	*/
}

int query(int x, int y) {
	//cerr << "query " << x << ' ' << y << '\n';
	assert(1 <= x && x <= NLEAVES);
	assert(1 <= y && y <= NTIME);
	int vOld = vertAt(x, y - 24);
	int vNew = vertAt(x, y);
	//cerr << "vOld=" << vOld << ' ';
	//cerr << "vNew=" << vNew << ' ';
	//cerr << '\n';
	int ans = forkValue[vNew];
	for (int cur = vNew; cur != vOld; cur = par[cur][0]) {
		ans += int(startValues[par[cur][0]].size());
	}
	if (vOld != vNew) {
		int leftCnt = pathValue[vNew];
		int hiAns = -int(1e9);
		for (int cur = vNew; cur != vOld; cur = par[cur][0]) {
			for (int t : startValues[cur]) {
				assert(cur == vNew || t < y);
				if (cur == vNew && t >= y) break;
				int v = evts[t].second;
				if (cur == vNew) {
					assert(vertAt(v, y) == vNew);
					if (endTime[v] < y) {
						leftCnt--;
					}
				} else {
					if (vertAt(v, y) == vNew && endTime[v] >= y) {
						leftCnt++;
					}
				}
			}

			int dir = ch[par[cur][0]][1] == cur;
			hiAns = max(hiAns, leftCnt + pathValue[ch[par[cur][0]][!dir]]);
			hiAns += int(startValues[par[cur][0]].size());
		}
		ans = max(ans, hiAns);
	}

	ans += parValue[vOld]
		- int(lower_bound(endValues[vOld].begin(), endValues[vOld].end(), y - 24) - endValues[vOld].begin());
	//cerr << "answer " << ans << '\n';
	return ans;
}

ll go() {
	cin >> NVERTS >> NEDGES >> NTIME >> NQUERY;
	for (int e = 1; e <= NEDGES; e++) {
		int u, v; cin >> u >> v;
		edges[e] = {u,v};
	}

	for (int t = 1; t <= NTIME; t++) {
		int op, v; cin >> op >> v;
		evts[t] = {op, v};
	}

	precomp();

	int lastAns = 0;
	ll ans = 0;
	while (NQUERY--) {
		int x, y; cin >> x >> y;
		x ^= lastAns, y ^= lastAns;
		ans += (lastAns = query(x, y));
	}
	return ans;
}

void cleanup() {
	for (int i = 1; i <= NVERTS; i++) {
		startValues[i] = endValues[i] = {};
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		cerr << "Case #" << t << '\n';
		ll ans = go();
		cout << "Case #" << t << ": " << ans << '\n';
		cleanup();
	}

	return 0;
}
