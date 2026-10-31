#include<bits/stdc++.h>
using namespace std;

void my_sleep(double d) {
	auto start = clock();
	while (double(clock() - start) < d * CLOCKS_PER_SEC) {
	}
}

#define TEST 0

const int MAXN = 2.1e3;
const int MAXM = 3.1e3;

int N, M;
vector<int> outEdges[MAXN];
vector<int> inEdges[MAXN];

pair<int, int> edges[MAXM];

int distEnd[MAXN];

const int MAXL = MAXN;
int L;
bool canDist[MAXN][MAXL];

namespace dsu {
int par[MAXL];
void reset() { memset(par, -1, sizeof(par)); }
int getPar(int a) { return par[a] < 0 ? a : (par[a] = getPar(par[a])); }
bool merge(int a, int b) {
	a = getPar(a), b = getPar(b);
	if (a == b) return false;
	assert(a != b);
	if (par[a] > par[b]) swap(a,b);
	par[a] += par[b];
	par[b] = a;
	return true;
}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
#if TEST
	{
		//string S = "bcadabcabc";
		//string S = "abdc";
		string S;
		mt19937 mt(chrono::steady_clock::now().time_since_epoch().count());
		for (int i = 0; i < 4; i++) {
			S += char('a' + mt() % 4);
		}
		map<set<string>, int> mp;
		auto getNode = [&] (string s) -> int {
			set<string> res;
			for (int i = 0; i+int(s.size()) <= int(S.size()); i++) {
				if (S.substr(i,int(s.size())) == s) {
					for (int j = 0; i+int(s.size())+j <= int(S.size()); j++) {
						res.insert(S.substr(i+int(s.size()), j));
					}
				}
			}
			if (mp.count(res)) {
				return mp[res];
			} else {
				int v = int(mp.size());
				mp.emplace(res, v);
				return v;
			}
		};

		int root = getNode("");
		set<pair<int, int>> genEdges;
		for (int i = 0; i < int(S.size()); i++) {
			int cur = root;
			for (int j = 1; i+j <= int(S.size()); j++) {
				int nxt = getNode(S.substr(i,j));
				genEdges.insert({cur, nxt});
				cur = nxt;
			}
		}

		N = int(mp.size());
		vector<int> perm(N);
		iota(perm.begin(), perm.end(), 0);
		shuffle(perm.begin(), perm.end(), mt);
		M = 0;
		for (auto it : genEdges) {
			edges[M++] = {perm[it.first], perm[it.second]};
		}

		cerr << N << ' ' << M << '\n';
		for (int e = 0; e < M; e++) {
			cerr << edges[e].first << ' ' << edges[e].second << '\n';
		}
	}
#else
	cin >> N >> M;
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		assert(u != v);
		edges[e] = {u,v};
	}
#endif

	if (N == 1) {
		// maybe this should be empty-string?
		cout << -1 << '\n';
		exit(0);
	}

	assert(N >= 2);

	{ // do the toposort
		vector<int> topoSort; topoSort.reserve(N);
		vector<int> topoInd(N, -1);
		vector<int> inDeg(N, 0);
		for (int e = 0; e < M; e++) {
			outEdges[edges[e].first].push_back(edges[e].second);
			inDeg[edges[e].second]++;
		}
		for (int i = 0; i < N; i++) {
			if (!inDeg[i]) topoSort.push_back(i);
		}
		for (int z = 0; z < N; z++) {
			int i = topoSort[z];
			topoInd[i] = z;
			for (int j : outEdges[i]) {
				if (!(--inDeg[j])) {
					topoSort.push_back(j);
				}
			}
			outEdges[i] = {};
		}

		for (int e = 0; e < M; e++) {
			edges[e].first = topoInd[edges[e].first];
			edges[e].second = topoInd[edges[e].second];
			assert(edges[e].first < edges[e].second);
			outEdges[edges[e].first].push_back(edges[e].second);
			inEdges[edges[e].second].push_back(edges[e].first);
		}
	}

	for (int i = 0; i < N; i++) {
		if (outEdges[i].empty() != (i == N-1)) {
			cout << -1 << '\n';
			exit(0);
		}
		if (inEdges[i].empty() != (i == 0)) {
			cout << -1 << '\n';
			exit(0);
		}
	}

	distEnd[N-1] = 0;
	canDist[N-1][0] = true;
	for (int i = N-2; i >= 0; i--) {
		distEnd[i] = 0;
		for (int j : outEdges[i]) {
			assert(j > i);
			distEnd[i] = max(distEnd[i], distEnd[j]+1);
			for (int l = 0; l <= distEnd[j]; l++) {
				if (canDist[j][l]) {
					canDist[i][l+1] = true;
				}
			}
		}
	}

	L = distEnd[0];

	int L0 = 0;
	while (!canDist[0][L0]) L0++;
	assert(L0 > 0);

	for (int l = L0; l <= L; l++) {
		if (!canDist[0][l]) {
			cout << -1 << '\n';
			exit(0);
		}
	}

	dsu::reset();
	for (int i = N-1; i > 0; i--) {
		for (int l = 0; l < distEnd[i]; l++) {
			if (canDist[i][l]) {
				dsu::merge(l, distEnd[i]);
			}
		}
	}

	for (int i = 0; i < N-1; i++) {
		unordered_set<int> st;
		for (int j : outEdges[i]) {
			int v = dsu::getPar(distEnd[j]);
			if (st.count(v)) {
				cout << -1 << '\n';
				exit(0);
			}
			st.insert(v);
		}
	}

	vector<vector<int>> lenVecs;
	for (int i = 0; i < N; i++) {
		vector<int> a;
		for (int l = 0; l <= distEnd[i]; l++) {
			if (canDist[i][l]) {
				a.push_back(l);
			}
		}
		lenVecs.push_back(a);
	}
	sort(lenVecs.begin(), lenVecs.end());
	if (unique(lenVecs.begin(), lenVecs.end()) != lenVecs.end()) {
		my_sleep(0.05);
		cout << -1 << '\n';
		exit(0);
	}

	string ans;
	vector<char> assignedChars(L, '\0');
	char nxtChar = 'a';
	for (int i = L; i >= L0; i--) {
		int v = dsu::getPar(i-1);
		if (!assignedChars[v]) {
			if (nxtChar > 'z') {
				cout << -1 << '\n';
				exit(0);
			}
			assignedChars[v] = nxtChar++;
		}
		ans += assignedChars[v];
	}

	int L1 = L0-1;
	while (L1 > 0) {
		int v = dsu::getPar(L1-1);
		if (!assignedChars[v]) {
			break;
		}
		ans += assignedChars[v];
		L1 --;
	}

	//cerr << L << ' ' << L0 << ' ' << L1 << ' ' << ans << '\n';

	for (int i = 0; i < L1; i++) {
		int v = dsu::getPar(i);
		if (dsu::par[v] != -1) {
			cout << -1 << '\n';
			exit(0);
		}
	}

	string maximalSub = ans.substr(int(ans.size()) - (L0-L1), (L0-L1));
	unordered_set<char> forbidden;
	for (int st = 0; true; st++) {
		st = int(ans.find(maximalSub, st));
		if (st == int(ans.size()) - (L0-L1)) break;
		forbidden.insert(ans[st+(L0-L1)]);
	}
	if (L1) {
		string bestAnswer;
		string matchSub = maximalSub.substr(1);
		for (int st = 0; st + (L0-L1-1) < int(ans.size()); st++) {
			if (ans.substr(st, L0-L1-1) == matchSub && !forbidden.count(ans[st + (L0-L1-1)])) {
				string cndAnswer = ans;
				for (int i = 0, p = st+(L0-L1-1); i < L1; i++, p++) {
					cndAnswer += cndAnswer[p];
				}
				if (bestAnswer.empty() || cndAnswer < bestAnswer) bestAnswer = cndAnswer;
			}
		}

		if (bestAnswer.empty()) {
			cout << -1 << '\n';
			exit(0);
		}
		ans = bestAnswer;
	}

	my_sleep(0.1);
	cout << ans << '\n';
	return 0;
}
