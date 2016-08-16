#include<bits/stdc++.h>
using namespace std;

typedef double ld;

const ld EPS = 1e-6;
const ld INF = 1e9;

const int MAXN = 3000;
const int MAXS = 100;

struct pt {
	ld x, y, z;
	pt(ld a, ld b, ld c) : x(a), y(b), z(c) {}
	pt(): x(0), y(0), z(0) {}
};

inline ld sq(ld a) { return a * a; }
inline ld norm(const pt &p) { return sq(p.x) + sq(p.y) + sq(p.z); }
inline ld abs(const pt &p) { return sqrt(norm(p)); }

pt operator + (const pt &a, const pt &b) {
	return pt(a.x + b.x, a.y + b.y, a.z + b.z);
}
pt operator - (const pt &a, const pt &b) {
	return pt(a.x - b.x, a.y - b.y, a.z - b.z);
}
pt operator * (const pt &a, const ld &v) {
	return pt(a.x * v, a.y * v, a.z * v);
}

pt cross(const pt &a, const pt &b) {
	return pt(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
}

ld dot(const pt &a, const pt &b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

int N;
int S;
pt P[MAXN], V[MAXN];

ld M;

ld dist[MAXN];
bool vis[MAXN];

// when are a and b close
pair<ld, ld> get_near(int cur, int nxt) {
	pt p = P[nxt] - P[cur], v = V[nxt] - V[cur];
	if(norm(v) < EPS) {
		if(norm(p) <= M) {
			return pair<ld, ld>(-2, INF);
		} else {
			return pair<ld, ld>(-2, -1);
		}
	}

	ld md = - dot(p, v) / norm(v);
	ld sp = M - norm(cross(p, v)) / norm(v);
	if(sp < 0) return pair<ld, ld>(-2, -1);
	sp /= norm(v);
	sp = sqrt(sp);

	ld a = md - sp, b = md + sp;
	//cerr << norm(p + v * a) - M << '\n';
	//cerr << norm(p + v * b) - M << '\n';
	//assert(abs(norm(p + v * a) - M) < EPS);
	//assert(abs(norm(p + v * b) - M) < EPS);

	assert(a <= b);

	return make_pair(a, b);
}

typedef pair<int, int> pii;
typedef pair<ld, pii> pdp;

bool can_vis[MAXN];
unordered_set<int> adj[MAXN];

bool timeout[MAXN];

void flood(int a) {
	if(can_vis[a]) return;
	can_vis[a] = true;
	for(int i : adj[a]) {
		flood(i);
	}
}
bool dijkstra() {
	vector<pdp> evts;
	for(int i = 0; i < N; i++) adj[i].clear();
	for(int i = 0; i < N; i++) {
		for(int j = i + 1; j < N; j++) {
			auto p = get_near(i, j);
			if(p.second < 0) continue;
			evts.emplace_back(p.second, pii(i, j));
			if(p.first <= 0) {
				adj[i].insert(j);
				adj[j].insert(i);
			}
			else evts.emplace_back(p.first, pii(-1-i, -1-j));
		}
	}
	sort(evts.begin(), evts.end());

	memset(can_vis, 0, sizeof(can_vis));
	flood(0);
	if(can_vis[1]) return true;

	memset(timeout, 0, sizeof(timeout));
	priority_queue<pair<ld, int>, vector<pair<ld, int>>, greater<pair<ld, int>>> timeouts;
	if(adj[0].empty()) {
		timeout[0] = true;
		timeouts.emplace(S, 0);
	}

	for(auto evt : evts) {
		if(can_vis[1]) return true;
		ld t = evt.first;
		while(!timeouts.empty() && timeouts.top().first < t) {
			int cur = timeouts.top().second;
			timeouts.pop();
			if(timeout[cur]) {
				assert(adj[cur].empty());
				timeout[cur] = false;
				can_vis[cur] = false;
			}
		}
		int a = evt.second.first;
		int b = evt.second.second;
		if(evt.second.first >= 0) {
			// remove
			adj[a].erase(b);
			adj[b].erase(a);
			if(adj[a].empty()) {
				timeout[a] = true;
				timeouts.emplace(t + S, a);
			}
			if(adj[b].empty()) {
				timeout[b] = true;
				timeouts.emplace(t + S, b);
			}
		} else {
			a = -1 - a;
			b = -1 - b;
			// add
			adj[a].insert(b);
			adj[b].insert(a);
			timeout[a] = timeout[b] = false;
			if(can_vis[a]) flood(b);
			if(can_vis[b]) flood(a);
		}
		if(can_vis[1]) return true;
	}
	return false;
}

ld binary_search() {
	ld mi = 0, ma = 1e9;
	for(int i = 0; i < 50; i++) {
		M = (mi + ma) / 2;
		if(dijkstra()) {
			ma = M;
		} else {
			mi = M;
		}
	}
	return sqrt(ma);
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {

		cin >> N >> S;
		for(int i = 0; i < N; i++) {
			cin >> P[i].x >> P[i].y >> P[i].z >> V[i].x >> V[i].y >> V[i].z;
		}

		ld res = binary_search();
		
		cout << "Case #" << case_num << ": ";
		cout << fixed << setprecision(7) << res << '\n';
		cerr << "Finish " << case_num << ' ' << res << '\n';
	}

	return 0;
}
