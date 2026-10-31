#include "walk.h"

#include<bits/stdc++.h>

namespace {

namespace min_distance_set {

using namespace std;
using ll = long long;
using height_t = pair<ll, int>;

const ll INF = 1e18;

struct height_container {
	set<height_t> alive;
	map<height_t, ll> value;

	ll query(height_t h) {
		ll ans = INF;
		auto it = value.lower_bound(h);
		if (it != value.end()) {
			ans = min(ans, it->second + abs(h.first - it->first.first));
		}
		if (it != value.begin()) {
			--it;
			ans = min(ans, it->second + abs(h.first - it->first.first));
		}
		return ans;
	}

	void insert_line(height_t h, ll v = INF) {
		assert(!alive.count(h));
		alive.insert(h);
		if (v == INF || v >= query(h)) {
			return;
		}
		assert(!value.count(h));
		auto emp = value.emplace(h, v);
		assert(emp.second);
		while (true) {
			auto it = emp.first;
			++it;
			if (it == value.end()) break;
			if (it->second >= emp.first->second + abs(it->first.first - emp.first->first.first)) {
				value.erase(it);
			} else {
				break;
			}
		}
		while (true) {
			auto it = emp.first;
			if (it == value.begin()) break;
			--it;
			if (it->second >= emp.first->second + abs(it->first.first - emp.first->first.first)) {
				value.erase(it);
			} else {
				break;
			}
		}
	}

	void delete_line(height_t h) {
		assert(alive.count(h));

		if (value.count(h)) {
			auto pt = alive.find(h);
			auto nt = pt;
			if (pt != alive.begin()) {
				--pt;
				ll v = query(*pt);
				value.emplace(*pt, v);
			}

			if (nt != alive.end()) {
				++nt;
				if (nt != alive.end()) {
					ll v = query(*nt);
					value.emplace(*nt, v);
				}
			}
			value.erase(h);
		}

		alive.erase(h);
	}

	void insert_all(vector<height_t> v) {
		for (height_t h : v) insert_line(h);
	}
	void delete_all(vector<height_t> v) {
		for (height_t h : v) delete_line(h);
	}

	friend std::ostream& operator << (std::ostream& o, const height_container& h) {
		o << "[";
		for (const auto& it : h.alive) {
			o << "(" << it.first << "," << it.second << ")";
			o << ": ";
			if (h.value.count(it)) {
				o << h.value.at(it);
			} else {
				o << "_";
			}
			o << ", ";
		}
		o << "]";
		return o;
	}
};

ll min_distance(vector<int> X_, vector<int> H_, vector<int> L, vector<int> R, vector<int> Y_, int S, int T) {
	int N = int(X_.size());
	int M = int(Y_.size());

	vector<ll> X(X_.begin(), X_.end());
	vector<height_t> H(N);
	for (int i = 0; i < N; i++) {
		H[i] = {H_[i], i};
	}

	vector<height_t> Y(M);
	for (int e = 0; e < M; e++) {
		Y[e] = {Y_[e], -1-e};
	}

	if (S > T) swap(S, T);
	assert(S < T);

	// peak index
	int P = int(max_element(H.begin() + S, H.begin() + T + 1) - H.begin());
	assert(H[P] >= H[S] && H[P] >= H[T]);

	const height_t BOTTOM = {0, -M-1};
	const height_t TOP = {INF, N};

	vector<vector<height_t>> lefts(N);
	vector<vector<height_t>> rights(N);
	for (int e = 0; e < M; e++) {
		lefts[L[e]].push_back(Y[e]);
		rights[R[e]].push_back(Y[e]);
	}

	priority_queue<height_t, vector<height_t>, greater<height_t>> highEdges;
	for (int e = 0; e < M; e++) {
		highEdges.push(Y[e]);
	}

	if (false) { // left->right only
		if (S == 0 && T == N-1) {
			height_container hc;
			hc.insert_line(BOTTOM, 0);
			rights[S].push_back(BOTTOM);
			lefts[T].push_back(BOTTOM);
			for (int i = 0; i < N; i++) {
				hc.insert_all(lefts[i]);
				hc.delete_all(rights[i]);
			}


			ll ans = hc.query(BOTTOM);
			if (ans == INF) {
				return -1;
			}
			return ans + (X[T] - X[S]);
		} else {
			return -2;
		}
	}

	height_container sLeft, sRight, tLeft, tRight;
	sLeft.insert_line({0, -M-1}, 0);
	sRight.insert_line({0, -M-1}, 0);
	tLeft.insert_line({0, -M-1}, 0);
	tRight.insert_line({0, -M-1}, 0);

	lefts[S].push_back({0, -M-1});
	rights[S].push_back({0, -M-1});
	lefts[T].push_back({0, -M-1});
	rights[T].push_back({0, -M-1});

	int sl = S, sr = S, tl = T, tr = T;

	ll ans = INF;

	bool crossedMid = false;

	while (true) {
		height_t evtHeight = TOP;
		if (!highEdges.empty()) {
			evtHeight = min(evtHeight, highEdges.top());
		}
		if (sl > 0) {
			evtHeight = min(evtHeight, H[sl]);
		}

		if (!crossedMid) {
			evtHeight = min(evtHeight, H[sr]);
			evtHeight = min(evtHeight, H[tl]);
		}

		if (tr < N-1) {
			evtHeight = min(evtHeight, H[tr]);
		}

		if (evtHeight == TOP) break;

		if (!highEdges.empty() && evtHeight == highEdges.top()) {
			// do the thing
			int e = -1-highEdges.top().second;
			highEdges.pop();

			if (crossedMid) {
				assert(Y[e] > H[P]);
				assert(L[e] < S || L[e] > T);
				assert(R[e] < S || R[e] > T);
				if (L[e] < P && P < R[e]) {
					// just connect the two, it crosses
					ans = min(ans, sLeft.query(Y[e]) + tRight.query(Y[e]) + 2 * (X[S] - X[sl]) + 2 * (X[tr] - X[T]));

					// it's never optimal to go further, so we could just break
					//break;

					sLeft.insert_line(Y[e]);
					tRight.insert_line(Y[e]);
				}
			} else {
				assert(Y[e] <= H[P]);
				if (L[e] <= S && S <= R[e]) {
					ll rval = sRight.query(Y[e]) + 2 * (X[sr] - X[S]);
					ll lval = sLeft.query(Y[e]) + 2 * (X[S] - X[sl]);

					sLeft.insert_line(Y[e], rval);
					sRight.insert_line(Y[e], lval);
				}
				if (L[e] <= T && T <= R[e]) {
					ll rval = tRight.query(Y[e]) + 2 * (X[tr] - X[T]);
					ll lval = tLeft.query(Y[e]) + 2 * (X[T] - X[tl]);

					tLeft.insert_line(Y[e], rval);
					tRight.insert_line(Y[e], lval);
				}
			}
		} else if (!crossedMid && evtHeight == H[P]) {
			assert(sr == P && tl == P);

			for (auto it : sRight.value) {
				ans = min(ans, it.second + tLeft.query(it.first));
			}

			crossedMid = true;
		} else if (sl > 0 && evtHeight == H[sl]) {
			sLeft.delete_all(lefts[sl]);
			--sl;
			sLeft.insert_all(rights[sl]);
		} else if (sr < P && evtHeight == H[sr]) {
			sRight.delete_all(rights[sr]);
			++sr;
			sRight.insert_all(lefts[sr]);
		} else if (tl > P && evtHeight == H[tl]) {
			tLeft.delete_all(lefts[tl]);
			--tl;
			tLeft.insert_all(rights[tl]);
		} else if (tr < N-1 && evtHeight == H[tr]) {
			tRight.delete_all(rights[tr]);
			++tr;
			tRight.insert_all(lefts[tr]);
		} else assert(false);
	}

	//cerr << ans << '\n';

	if (ans == INF) return -1;
	else return ans + (X[T] - X[S]);
}

} // namespace min_distance_set

namespace min_distance_dijk {

using namespace std;
using ll = long long;
using height_t = pair<int, int>;

ll min_distance(vector<int> X_, vector<int> H_, vector<int> L, vector<int> R, vector<int> Y_, int S, int T) {
	int N = int(X_.size());
	int M = int(Y_.size());

	vector<ll> X(X_.begin(), X_.end());
	vector<height_t> H(N);
	for (int i = 0; i < N; i++) {
		H[i] = {H_[i], i};
	}

	vector<height_t> Y(M);
	for (int e = 0; e < M; e++) {
		Y[e] = {Y_[e], -1-e};
	}

	vector<vector<height_t>> needs(N);

	vector<height_t> heights;
	heights.insert(heights.end(), H.begin(), H.end());
	heights.insert(heights.end(), Y.begin(), Y.end());
	sort(heights.begin(), heights.end());
	reverse(heights.begin(), heights.end());

	set<int> posts;
	for (height_t h : heights) {
		if (h.second >= 0) {
			// it's a post
			posts.insert(h.second);
		} else {
			int e = -1-h.second;
			set<int> eNeeds;
			eNeeds.insert(L[e]);
			eNeeds.insert(R[e]);
			for (int p : {S, T}) {
				if (L[e] <= p && p <= R[e]) {
					{
						auto it = posts.lower_bound(p);
						if (it != posts.end()) {
							eNeeds.insert(*it);
						}
					}
					{
						auto it = posts.upper_bound(p);
						if (it != posts.begin()) {
							eNeeds.insert(*--it);
						}
					}
				}
			}
			for (int i : eNeeds) {
				needs[i].push_back(h);
			}
		}
	}

	int V = 0;
	vector<vector<pair<int, ll>>> adj;

	map<height_t, pair<int, int>> prvVert;
	set<height_t> inters;
	vector<vector<height_t>> lefts(N);
	vector<vector<height_t>> rights(N);
	for (int e = 0; e < M; e++) {
		lefts[L[e]].push_back(Y[e]);
		rights[R[e]].push_back(Y[e]);
	}

	const height_t SBOTTOM(0, -1-M);
	lefts[S].push_back(SBOTTOM);
	rights[S].push_back(SBOTTOM);
	needs[S].push_back(SBOTTOM);

	const height_t TBOTTOM(0, -1-(M+1));
	lefts[T].push_back(TBOTTOM);
	rights[T].push_back(TBOTTOM);
	needs[T].push_back(TBOTTOM);


	int Svert = -1, Tvert = -1;
	for (int i = 0; i < N; i++) {
		for (height_t h : lefts[i]) {
			inters.insert(h);
		}

		reverse(needs[i].begin(), needs[i].end()); // should now be sorted

		vector<height_t> realNeeds;
		for (height_t h : needs[i]) {
			auto it = inters.find(h);
			assert(it != inters.end());

			auto kt = it;
			if (kt != inters.begin()) {
				--kt;
				if (realNeeds.empty() || realNeeds.back() < *kt) {
					realNeeds.push_back(*kt);
				}
			}

			if (realNeeds.empty() || realNeeds.back() < *it) {
				realNeeds.push_back(*it);
			}

			auto jt = it; ++jt;
			if (jt != inters.end()) {
				if (realNeeds.empty() || realNeeds.back() < *jt) {
					realNeeds.push_back(*jt);
				}
			}
		}

		while (!realNeeds.empty() && realNeeds.back() > H[i]) realNeeds.pop_back();

		assert(V == int(adj.size()));
		adj.resize(V + int(realNeeds.size()));

		for (int z = 0; z < int(realNeeds.size()); z++) {
			height_t h = realNeeds[z];
			int v = V + z;

			auto it = prvVert.find(h);
			if (it != prvVert.end()) {
				int u = it->second.second;
				ll d = X[i] - X[it->second.first];
				adj[v].emplace_back(u, d);
				adj[u].emplace_back(v, d);

				it->second = {i, v};
			} else {
				prvVert.emplace(h, pair<int, int>{i, v});
			}
		}

		for (int z = 0; z+1 < int(realNeeds.size()); z++) {
			assert(realNeeds[z] < realNeeds[z+1]);
			int v = V + z + 1;
			int u = V + z;
			ll d = realNeeds[z+1].first - realNeeds[z].first;
			adj[v].emplace_back(u, d);
			adj[u].emplace_back(v, d);
		}

		if (i == S) {
			assert(!realNeeds.empty());
			assert(realNeeds[0] == SBOTTOM);
			Svert = V + 0;
		}
		if (i == T) {
			assert(!realNeeds.empty());
			assert(realNeeds[0] == TBOTTOM);
			Tvert = V + 0;
		}

		V += int(realNeeds.size());

		for (height_t h : rights[i]) {
			inters.erase(h);
		}
	}

	const ll INF = 1e18;
	vector<ll> dist(V, INF);
	priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
	dist[Svert] = 0;
	pq.emplace(0, Svert);
	while (!pq.empty()) {
		int cur = pq.top().second;
		ll d = pq.top().first;
		pq.pop();
		if (dist[cur] != d) continue;

		if (cur == Tvert) return d;

		for (auto it : adj[cur]) {
			int nxt = it.first;
			ll nd = d + it.second;
			if (nd < dist[nxt]) {
				dist[nxt] = nd;
				pq.emplace(nd, nxt);
			}
		}
	}

	return -1;
}

} // namespace min_distance_dijk

} // anonymous namespace

long long min_distance(std::vector<int> X, std::vector<int> H, std::vector<int> L, std::vector<int> R, std::vector<int> Y, int S, int T) {
	if (true) {
		return min_distance_set::min_distance(X, H, L, R, Y, S, T);
	} else {
		return min_distance_dijk::min_distance(X, H, L, R, Y, S, T);
	}
}
