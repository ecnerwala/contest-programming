#include<bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

vi val, comp, z, cont;
int Time, ncomps;
template<class G, class F> int dfs(int j, G& g, F& f) {
	int low = val[j] = ++Time, x; z.push_back(j);
	trav(e,g[j]) if (comp[e] < 0)
		low = min(low, val[e] ?: dfs(e,g,f));

	if (low == val[j]) {
		do {
			x = z.back(); z.pop_back();
			comp[x] = ncomps;
			cont.push_back(x);
		} while (x != j);
		f(cont); cont.clear();
		ncomps++;
	}
	return val[j] = low;
}
template<class G, class F> void scc(G& g, F f) {
	int n = sz(g);
	val.assign(n, 0); comp.assign(n, -1);
	Time = ncomps = 0;
	rep(i,0,n) if (comp[i] < 0) dfs(i, g, f);
}

const int MAXN = 1010;
const int MAXM = 1010;
int N, M;
const int MAXV = MAXN * MAXM;
int V;

char G[MAXN][MAXM];

const char mv[4] = {'L', 'R', 'U', 'D'};
const int mx[4] = {0, 0, -1, 1};
const int my[4] = {-1, 1, 0, 0};

int C;
int ccNum[MAXV];
int ccSz[MAXV];
vector<int> ccAdj[MAXV];
int ccVis[MAXV];
pair<int16_t, int16_t> ccPt[MAXV];
struct {
	int16_t minx, maxx, miny, maxy;
} ccRect[MAXV];
vector<pair<pair<int16_t, int16_t>, int>> ccReach[MAXV];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int Q;
	cin >> N >> M >> Q;

	for (int i = 1; i <= N; i++) {
		
		G[i][0] = 'R';
		cin >> (G[i]+1);
		G[i][M+1] = 'L';
	}

	G[0][0] = G[0][M+1] = 'U';
	G[N+1][0] = G[N+1][M+1] = 'D';
	for (int j = 1; j <= M; j++) {
		G[0][j] = 'D';
		G[N+1][j] = 'U';
	}

	{
		V = (N+2) * (M+2);

		vector<vector<int>> adj(V);

		for (int i = 0; i <= N+1; i++) {
			for (int j = 0; j <= M+1; j++) {
				for (int d = 0; d < 4; d++) {
					if (mv[d] == G[i][j]) continue;
					int nx = i + mx[d], ny = j + my[d];
					if (0 <= nx && nx <= N+1 && 0 <= ny && ny <= M+1) {
						int cur = i * (M+2) + j;
						int nxt = nx * (M+2) + ny;
						adj[cur].push_back(nxt);
					}
				}
			}
		}

		for (int v = 0; v < V; v++) {
			ccNum[v] = -1;
		}

		scc(adj, [&](vector<int>& verts) {
			int curCC = C++;

			ccSz[curCC] = int(verts.size());

			ccRect[curCC].minx = int16_t(N+1);
			ccRect[curCC].maxx = 0;
			ccRect[curCC].miny = int16_t(M+1);
			ccRect[curCC].maxy = 0;

			for (int v : verts) {
				assert(ccNum[v] == -1);
				ccNum[v] = curCC;

				int16_t x = int16_t(v / (M+2));
				int16_t y = int16_t(v % (M+2));
				ccRect[curCC].minx = min(ccRect[curCC].minx, x);
				ccRect[curCC].maxx = max(ccRect[curCC].maxx, x);
				ccRect[curCC].miny = min(ccRect[curCC].miny, y);
				ccRect[curCC].maxy = max(ccRect[curCC].maxy, y);
				ccPt[curCC] = {x,y};
			}
			unordered_set<int> canReach;
			for (int v : verts) {
				for (int nxt : adj[v]) {
					if (ccNum[nxt] == curCC) continue;
					assert(ccNum[nxt] != -1);
					canReach.insert(ccNum[nxt]);
				}
			}

			ccAdj[curCC] = {canReach.begin(), canReach.end()};

			//cerr << curCC << ": ";
			//for (int v : verts) { cerr << "(" << v/(M+2) << "," << v%(M+2) << ")" << ' '; } cerr << '\n';
			//for (int nxt : ccAdj[curCC]) { cerr << nxt << ' '; } cerr << '\n';
		});
	}

	int epoch = 0;
	for (int S = 0; S < C; S++) {
		epoch++;
		vector<int> q;
		q.push_back(S);
		ccVis[S] = epoch;
		for (int i = 0; i < int(q.size()); i++) {
			int cur = q[i];
			ccReach[S].emplace_back(ccPt[cur], ccSz[cur]);
			ccRect[cur].minx = min(ccRect[cur].minx, ccRect[S].minx);
			ccRect[cur].maxx = max(ccRect[cur].maxx, ccRect[S].maxx);
			ccRect[cur].miny = min(ccRect[cur].miny, ccRect[S].miny);
			ccRect[cur].maxy = max(ccRect[cur].maxy, ccRect[S].maxy);
			for (int nxt : ccAdj[cur]) {
				if (ccVis[nxt] != epoch) {
					ccVis[nxt] = epoch;
					q.push_back(nxt);
				}
			}
		}
	}

	while (Q--) {
		int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
		int S = ccNum[x1 * (M+2) + y1];
		int T = ccNum[x2 * (M+2) + y2];

		int ans = 0;
		for (auto it : ccReach[S]) {
			auto pt = it.first;
			if (ccRect[T].minx <= pt.first && pt.first <= ccRect[T].maxx && ccRect[T].miny <= pt.second && pt.second <= ccRect[T].maxy) {
				ans += it.second;
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
