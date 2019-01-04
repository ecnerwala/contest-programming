#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;

const int MAXF = 400;

int F, K;
map<pii, vector<pair<double, int>>> edges;
vector<int> adj[MAXF * 2];

bool prot[MAXF];
bool done[MAXF * 2];
vector<int> res;

void dfs(int i) {
	if (done[i]) return;
	done[i] = true;
	if (!prot[i%F]) {
		prot[i%F] = true;
		res.push_back(i%F);
	}
	for (int n : adj[i]) {
		dfs(n);
	}
}

void go() {
	for (auto it : edges) {
		sort(it.second.begin(), it.second.end());
		assert(!it.second.empty());
		it.second.push_back(it.second[0]);
		for (int i = 0; i+1 < int(it.second.size()); i++) {
			int a = it.second[i].second;
			int b = it.second[i+1].second;
			adj[a].push_back((b+F) % (2*F));
			adj[(b+F) % (2*F)].push_back(a);
		}
	}
	assert(F);
	if (K == 0) K = 1;
	for (int i = 0; i < K; i++) {
		prot[i] = true;
	}
	for (int i = F-1; i >= K; i--) {
		adj[i].push_back(i+F);
		adj[i+F].push_back(i);
		prot[i] = false;
	}
	memset(done, false, sizeof(done));
	for (int i = K-1; i >= 0; i--) {
		adj[i].push_back(i+F);
		adj[i+F].push_back(i);
		done[i] = false;
		done[i+F] = false;
		prot[i] = false;

		dfs(i);
	}

	reverse(res.begin(), res.end());
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		cin >> F >> K;
		for (int i = 0; i < F; i++) {
			int a, b, c, d; cin >> a >> b >> c >> d;
			edges[pii(a,b)].emplace_back(atan2(d-b,c-a), i);
			edges[pii(c,d)].emplace_back(atan2(b-d,a-c), i+F);
		}

		go();

		cout << "Case #" << case_num << ":";
		for (int it : res) {
			cout << ' ' << it + 1;
		}
		cout << '\n';

		res.clear();
		edges.clear();
		for (int i = 0; i < 2*F; i++) {
			adj[i].clear();
		}

	}

	return 0;
}
