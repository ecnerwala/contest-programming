#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int P, W; cin >> P >> W;
	std::vector<std::array<int, 2>> coords(P);
	for (auto& [x, y] : coords) {
		cin >> x >> y;
	}
	const int X = P+1;
	std::vector<std::array<int, 2>> wall_points(W);
	for (auto& [a, b] : wall_points) {
		cin >> a >> b; a--, b--;
	}
	int Bx, By; cin >> Bx >> By;

	std::vector<int> xs; xs.reserve(P+1);
	std::vector<int> ys; ys.reserve(P+1);
	for (auto [x, y] : coords) {
		xs.push_back(x);
		ys.push_back(y);
	}
	xs.push_back(Bx);
	ys.push_back(By);

	std::sort(xs.begin(), xs.end());
	std::sort(ys.begin(), ys.end());
	xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
	ys.erase(std::unique(ys.begin(), ys.end()), ys.end());
	for (auto& [x, y] : coords) {
		x = int(std::lower_bound(xs.begin(), xs.end(), x) - xs.begin());
		y = int(std::lower_bound(ys.begin(), ys.end(), y) - ys.begin());
	}

	Bx = int(std::lower_bound(xs.begin(), xs.end(), Bx) - xs.begin());
	By = int(std::lower_bound(ys.begin(), ys.end(), By) - ys.begin());

	std::vector<std::array<int, 4>> walls(P, std::array<int, 4>{-1, -1, -1, -1});
	std::array<std::vector<std::vector<std::pair<int, int>>>, 2> inserts;
	std::array<std::vector<std::vector<std::pair<int, int>>>, 2> deletes;
	for (int z = 0; z < 2; z++) {
		inserts[z].resize(X);
		deletes[z].resize(X);
	}
	for (int w = 0; w < W; w++) {
		auto [a, b] = wall_points[w];
		assert(a != b);
		assert(coords[a] != coords[b]);
		if (coords[a] > coords[b]) std::swap(a, b);
		assert(coords[a][0] == coords[b][0] || coords[a][1] == coords[b][1]);
		for (int z = 0; z < 2; z++) {
			if (coords[a][z] != coords[b][z]) {
				assert(coords[a][!z] == coords[b][!z]);
				assert(coords[a][z] < coords[b][z]);

				// z = 0 means right
				assert(walls[a][0+z] == -1);
				walls[a][0+z] = w;
				assert(walls[b][2+z] == -1);
				walls[b][2+z] = w;

				inserts[z][coords[a][z]].push_back({coords[a][!z], w});
				deletes[z][coords[b][z]].push_back({coords[b][!z], w});
			}
		}
	}

	const int V = 2*W+2;
	const int vert_infty = 2 * W;
	const int vert_bomb = 2 * W + 1;
	std::vector<int> par(V, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		//cerr << "merge " << a << ' ' << b << '\n';
		a = get_par(a), b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};

	{
		std::vector<int> buf; buf.reserve(8);
		for (auto lst : walls) {
			buf.clear();
			if (lst[0] != -1) {
				buf.push_back(2 * lst[0]);
				buf.push_back(2 * lst[0] + 1);
			}
			if (lst[1] != -1) {
				buf.push_back(2 * lst[1] + 1);
				buf.push_back(2 * lst[1]);
			}
			if (lst[2] != -1) {
				buf.push_back(2 * lst[2] + 1);
				buf.push_back(2 * lst[2]);
			}
			if (lst[3] != -1) {
				buf.push_back(2 * lst[3]);
				buf.push_back(2 * lst[3] + 1);
			}
			//cerr << "vert" << ' '; for (int w : buf) { cerr << w << ' '; } cerr << '\n';
			for (int i = 1; i < int(buf.size()); i += 2) {
				merge(buf[i], buf[(i+1) % int(buf.size())]);
			}
			buf.clear();
		}
	}

	// go through the guys
	for (int z = 0; z < 2; z++) {
		std::map<int, int> alive;
		for (int x = 0; x < X; x++) {
			// process inserts and deletes
			for (auto [y, w] : deletes[z][x]) {
				int num_erased = int(alive.erase(y));
				assert(num_erased);
			}
			for (auto [y, w] : inserts[z][x]) {
				auto [_, did_insert] = alive.insert({y, w});
				assert(did_insert);
			}

			if (z == 0 && x == Bx) {
				//cerr << "x = " << x << " by = " << By << '\n';
				//for (auto [k, v] : alive) { cerr << "at " << k << ' ' << v << '\n';}
				auto it = alive.lower_bound(By);
				assert(it == alive.end() || it->first != By);
				merge(vert_bomb, (it == alive.end() ? vert_infty : it->second * 2 + 0));
			}

			if (!alive.empty()) {
				for (auto [y, w] : deletes[z][x]) {
					auto it = alive.lower_bound(y);
					// don't need to check it
					if (it != alive.end() && it->first == y) continue;
					merge(
						(it == alive.begin() ? vert_infty : std::prev(it)->second * 2 + 1),
						(it == alive.end() ? vert_infty : it->second * 2 + 0)
					);
				}
				for (auto [y, w] : inserts[z][x]) {
					auto it = alive.find(y);
					assert(it != alive.end() && it->second == w);
					merge(
						(it == alive.begin() ? vert_infty : std::prev(it)->second * 2 + 1),
						it->second * 2 + 0
					);
					merge(
						it->second * 2 + 1,
						(std::next(it) == alive.end() ? vert_infty : std::next(it)->second * 2 + 0)
					);
				}
			}
		}
		assert(alive.empty());
	}

	int c_bomb = get_par(vert_bomb);
	int c_infty = get_par(vert_infty);
	//cerr << "c_bomb = " << c_bomb << '\n';
	//cerr << "c_infty = " << c_infty << '\n';
	std::vector<std::vector<int>> adj(V);
	std::vector<std::array<int, 2>> wall_conns(W);
	for (int w = 0; w < W; w++) {
		int a = get_par(2*w+0), b = get_par(2*w+1);
		wall_conns[w] = {a, b};
		adj[a].push_back(b);
		adj[b].push_back(a);
		//cerr << "wall " << w << ": " << a << ' ' << b << '\n';
	}

	std::vector<int> q; q.reserve(V);
	std::vector<int> dist(V, -1);
	dist[c_bomb] = 0;
	q.push_back(c_bomb);
	for (int z = 0; z < int(q.size()); z++) {
		int cur = q[z];
		int d = dist[cur] + 1;
		for (int nxt : adj[cur]) {
			if (dist[nxt] == -1) {
				dist[nxt] = d;
				q.push_back(nxt);
			}
		}
	}

	// condition: the bottleneck shortest path from a vertex to infty must still be at least itself
	std::priority_queue<std::pair<int, int>> pq;
	std::vector<int> infty_dist(V, -1);
	pq.push({infty_dist[c_infty] = dist[c_infty], c_infty});
	while (!pq.empty()) {
		auto [d, cur] = pq.top(); pq.pop();
		assert(d <= infty_dist[cur]);
		if (d < infty_dist[cur]) continue;
		assert(d <= dist[cur]);
		for (int nxt : adj[cur]) {
			int cnd = std::min(d, dist[nxt]);
			if (cnd > infty_dist[nxt]) {
				pq.push({infty_dist[nxt] = cnd, nxt});
			}
		}
	}

	for (int w = 0; w < W; w++) {
		auto [a, b] = wall_conns[w];
		if (dist[a] == dist[b]) continue;
		if (dist[a] > dist[b]) std::swap(a, b);
		assert(dist[b] - dist[a] == 1);
		if (dist[b] <= infty_dist[b]) {
			cout << w+1 << '\n';
		}
	}

	return 0;
}
