#include<bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

struct V {
	vector<pii> outs; // (dest, edge index)
	int nins = 0;
};

vi euler_walk(vector<V>& nodes, int nedges, int src=0) {
	int c = 0;
	trav(n, nodes) c += abs(n.nins - sz(n.outs));
	if (c > 2) return {};
	vector<vector<pii>::iterator> its;
	trav(n, nodes)
		its.push_back(n.outs.begin());
	vector<bool> eu(nedges);
	vi ret, s = {src};
	while(!s.empty()) {
		int x = s.back();
		auto& it = its[x], end = nodes[x].outs.end();
		while(it != end && eu[it->second]) ++it;
		if(it == end) { ret.push_back(x); s.pop_back(); }
		else { s.push_back(it->first); eu[it->second] = true; }
	}
	if(sz(ret) != nedges+1)
		ret.clear(); // No Eulerian cycles/paths.
	// else, non-cycle if ret.front() != ret.back()
	reverse(all(ret));
	return ret;
}

const int MAXW = 200, MAXH = 200;
int W, H;
bool G[MAXW][MAXH];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> W >> H;
	vector<V> nodes((W+1)*(H+1));
	int eInd = 0;
	auto addEdge = [&](int i1, int j1, int i2, int j2) {
		int v1 = i1 * (H+1) + j1;
		int v2 = i2 * (H+1) + j2;
		// decide the direction
		if ((j1 & 1) ^ abs(i1-i2)) {
			swap(v1, v2);
		}
		nodes[v1].outs.emplace_back(v2, eInd++);
		nodes[v2].nins ++;
	};
	bool seen = false;
	int i0, j0;
	for (int j = 1; j <= H; j++) {
		string S; cin >> S;
		for (int i = 1; i <= W; i++) {
			if (S[i-1] == 'X') {
				if (seen) {
					addEdge(i-1, j-1, i-1, j);
				} else {
					i0 = i-1, j0 = ((j-1)&1) ? j-1 : j;
				}
				addEdge(i-1, j-1, i, j);
				addEdge(i-1, j, i, j-1);
				addEdge(i, j-1, i, j);
				seen = true;
			}
		}
	}
	vector<int> res = euler_walk(nodes, eInd, i0 * (H+1) + j0);
	cout << int(res.size())-1 << '\n';
	for (auto v : res) {
		cout << v / (H+1) << ' ' << v % (H+1) << '\n';
	}

	return 0;
}
