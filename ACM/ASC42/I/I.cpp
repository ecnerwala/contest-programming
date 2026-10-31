#include<bits/stdc++.h>
#include<cstring>
using namespace std;

struct SuffixAutomaton {
	vector<map<char,int>> edges; // edges[i]  : the labeled edges from node i
	vector<int> link;            // link[i]   : the parent of i
	vector<int> length;          // length[i] : the length of the longest string in the ith class
	int last;                    // the index of the equivalence class of the whole string

	SuffixAutomaton(string s) {
		// add the initial node
		edges.push_back(map<char,int>());
		link.push_back(-1);
		length.push_back(0);
		last = 0;

		for(int i=0;i<int(s.size());i++) {
			// construct r
			edges.push_back(map<char,int>());
			length.push_back(i+1);
			link.push_back(0);
			int r = int(edges.size()) - 1;

			// add edges to r and find p with link to q
			int p = last;
			while(p >= 0 && edges[p].find(s[i]) == edges[p].end()) {
				edges[p][s[i]] = r;
				p = link[p];
			}
			if(p != -1) {
				int q = edges[p][s[i]];
				if(length[p] + 1 == length[q]) {
					// we do not have to split q, just set the correct suffix link
					link[r] = q;
				} else {
					// we have to split, add q'
					edges.push_back(edges[q]); // copy edges of q
					length.push_back(length[p] + 1);
					link.push_back(link[q]); // copy parent of q
					int qq = int(edges.size())-1;
					// add qq as the new parent of q and r
					link[q] = qq;
					link[r] = qq;
					// move short classes pointing to q to point to q'
					while(p >= 0 && edges[p][s[i]] == q) {
						edges[p][s[i]] = qq;
						p = link[p];
					}
				}
			}
			last = r;
		}
	}
};

#include<bits/extc++.h>
template <typename flow_t = int, typename cost_t = long long>
struct MCMF_SSPA {
	int N;
	vector<vector<int>> adj;
	struct edge_t {
		int dest;
		flow_t cap;
		cost_t cost;
		char label;
	};
	vector<edge_t> edges;

	vector<char> seen;
	vector<cost_t> pi;
	vector<int> prv;

	explicit MCMF_SSPA(int N_) : N(N_), adj(N), pi(N, 0), prv(N) {}

	void addEdge(int from, int to, flow_t cap, cost_t cost, char label) {
		assert(cap >= 0);
		int e = int(edges.size());
		edges.emplace_back(edge_t{to, cap, cost, label});
		edges.emplace_back(edge_t{from, 0, -cost, label});
		adj[from].push_back(e);
		adj[to].push_back(e+1);
	}

	const cost_t INF_COST = numeric_limits<cost_t>::max() / 4;
	const flow_t INF_FLOW = numeric_limits<flow_t>::max() / 4;
	vector<cost_t> dist;
	__gnu_pbds::priority_queue<pair<cost_t, int>> q;
	vector<typename decltype(q)::point_iterator> its;
	void path(int s) {
		dist.assign(N, INF_COST);
		dist[s] = 0;

		its.assign(N, q.end());
		its[s] = q.push({0, s});

		while (!q.empty()) {
			int i = q.top().second; q.pop(); its[i] = q.end();
			cost_t d = dist[i];
			for (int e : adj[i]) {
				if (edges[e].cap) {
					int j = edges[e].dest;
					cost_t nd = d + edges[e].cost;
					if (nd < dist[j]) {
						dist[j] = nd;
						prv[j] = e;
						if (its[j] == q.end()) {
							its[j] = q.push({-(dist[j] - pi[j]), j});
						} else {
							q.modify(its[j], {-(dist[j] - pi[j]), j});
						}
					}
				}
			}
		}

		swap(pi, dist);
	}

	pair<flow_t, cost_t> maxflow(int s, int t) {
		assert(s != t);
		flow_t totFlow = 0; cost_t totCost = 0;
		while (path(s), pi[t] < INF_COST) {
			flow_t curFlow = numeric_limits<flow_t>::max();
			for (int cur = t; cur != s; ) {
				int e = prv[cur];
				int nxt = edges[e^1].dest;
				curFlow = min(curFlow, edges[e].cap);
				cur = nxt;
			}
			totFlow += curFlow;
			totCost += pi[t] * curFlow;
			for (int cur = t; cur != s; ) {
				int e = prv[cur];
				int nxt = edges[e^1].dest;
				edges[e].cap -= curFlow;
				edges[e^1].cap += curFlow;
				cur = nxt;
			}
		}
		return {totFlow, totCost};
	}
};

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	if (fopen("incomparable.in", "r")) {
		freopen("incomparable.in", "r", stdin);
		freopen("incomparable.out", "w", stdout);
	}

	while (true) {
		string S; cin >> S;
		if (S.empty()) exit(0);
		SuffixAutomaton sam(S + '$');
		MCMF_SSPA<> mcmf(2*int(sam.edges.size()));
		for (int i = 0; i < int(sam.edges.size()); i++) {
			mcmf.addEdge(2*i, 2*i+1, 1, -1, '\0');
			for (auto it : sam.edges[i]) {
				mcmf.addEdge(2*i + 1, 2 * it.second, 1, 0, it.first);
			}
		}
		int source = 1, sink = 2 * sam.last;
		auto v = mcmf.maxflow(source, sink);
		cout << v.first - 1 << ' ' << -v.second << '\n';
		for (auto e : mcmf.adj[source]) {
			if (mcmf.edges[e].dest != source-1 && mcmf.edges[e].cap == 0) {
				// this is taken
				string ans; ans += mcmf.edges[e].label;
				for (int cur = mcmf.edges[e].dest + 1; cur != sink+1; ) {
					assert(cur & 1);
					int curE = -1;
					for (int cndE : mcmf.adj[cur]) {
						if (mcmf.edges[cndE].dest != cur-1 && mcmf.edges[cndE].cap == 0) {
							curE = cndE;
							break;
						}
					}
					assert(curE != -1);
					ans += mcmf.edges[curE].label;
					cur = mcmf.edges[curE].dest+1;
				}
				assert(ans.back() == '$');
				ans.pop_back();
				if (ans.empty()) continue;
				cout << ans << '\n';
			}
		}
	}

	return 0;
}
