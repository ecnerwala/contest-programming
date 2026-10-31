#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int P, W; cin >> P >> W;
	std::vector<std::array<int, 2>> coords(P);
	for (auto& [x, y] : coords) {
		cin >> x >> y;
	}
	std::vector<std::array<int, 2>> wall_points(W);
	for (auto& [a, b] : wall_points) {
		cin >> a >> b; a--, b--;
	}
	int Bx, By; cin >> Bx >> By;

	std::vector<int> weight(2*W, 0);
	std::vector<std::array<int, 4>> walls(P, std::array<int, 4>{-1, -1, -1, -1});
	for (int w = 0; w < W; w++) {
		auto [a, b] = wall_points[w];
		assert(a != b);
		assert(coords[a] != coords[b]);
		assert(coords[a][0] == coords[b][0] || coords[a][1] == coords[b][1]);
		int z = (coords[a][0] != coords[b][0]) ? 0 : 1;
		if (coords[a][z] > coords[b][z]) std::swap(a, b);
		assert(coords[a][!z] == coords[b][!z]);
		assert(coords[a][z] < coords[b][z]);

		if (z == 1 && coords[a][1] <= By && By < coords[b][1] && Bx <= coords[a][0]) {
			weight[2*w + 0] = +1;
			weight[2*w + 1] = -1;
		}
		// z = 0 means right
		assert(walls[a][0+z] == -1);
		walls[a][0+z] = w;
		assert(walls[b][2+z] == -1);
		walls[b][2+z] = w;
	}

	std::vector<int> nxt_e(2*W, 0);

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
				nxt_e[buf[i]] = buf[(i+1) % int(buf.size())];
			}
			buf.clear();
		}
	}

	std::vector<int> face_id(2*W, -1);
	int F = 0;
	std::vector<int> face_weight(2*W);
	std::vector<int> centers; centers.reserve(2*W);
	std::vector<int> outsides; outsides.reserve(2*W);
	for (int e = 0; e < 2*W; e++) {
		if (face_id[e] != -1) continue;
		int f = F++;
		int w = 0;
		int cur = e;
		do {
			w += weight[cur];
			face_id[cur] = f;
			cur = nxt_e[cur];
		} while (cur != e);
		assert(w == -1 || w == 0 || w == +1);
		if (w == +1) {
			centers.push_back(f);
		} else if (w == -1) {
			outsides.push_back(f);
		} else if (w == 0) {
			// nothing
		} else assert(false);
	}

	std::vector<std::vector<int>> adj(F);
	for (int w = 0; w < W; w++) {
		int a = face_id[2 * w + 0];
		int b = face_id[2 * w + 1];
		adj[a].push_back(b);
		adj[b].push_back(a);
		//cerr << "wall " << w << ": " << a << ' ' << b << '\n';
	}

	std::vector<int> q; q.reserve(F);
	std::vector<int> dist(F, -1);
	for (int f : centers) {
		dist[f] = 0;
		q.push_back(f);
	}
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
	std::vector<int8_t> in_queue(F);
	std::vector<int8_t> good_face(F);
	for (int f : outsides) {
		good_face[f] = true;
		in_queue[f] = true;
	}
	std::vector<int> q2; q2.reserve(F);
	for (int d = F, qit = int(q.size()) - 1; d >= 0; d--) {
		q2.clear();
		for (; qit >= 0 && dist[q[qit]] == d; qit--) {
			if (in_queue[q[qit]]) q2.push_back(q[qit]);
		}
		for (int z = 0; z < int(q2.size()); z++) {
			int cur = q2[z];
			assert(d <= dist[cur]);
			for (int nxt : adj[cur]) {
				if (in_queue[nxt]) continue;
				in_queue[nxt] = true;
				good_face[nxt] = d >= dist[nxt];
				if (d <= dist[nxt]) {
					q2.push_back(nxt);
				}
			}
		}
	}

	for (int w = 0; w < W; w++) {
		int a = face_id[2 * w + 0];
		int b = face_id[2 * w + 1];
		if (dist[a] == dist[b]) continue;
		if (good_face[b] && good_face[a]) {
			cout << w+1 << '\n';
		}
	}

	return 0;
}
