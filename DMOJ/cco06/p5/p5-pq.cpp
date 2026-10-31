#include <bits/stdc++.h>

template <typename T> using max_pq = std::priority_queue<T, std::vector<T>, std::less<T>>;
template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int D; cin >> D;
		vector<int> vertex_labels; vertex_labels.reserve(2*D);

		vector<array<int, 2>> edges(D);
		for (auto& e : edges) for (auto& v : e) {
			cin >> v;
			vertex_labels.push_back(v);
		}

		sort(vertex_labels.begin(), vertex_labels.end());
		vertex_labels.resize(unique(vertex_labels.begin(), vertex_labels.end()) - vertex_labels.begin());
		int V = int(vertex_labels.size());

		for (auto& e : edges) for (auto& v : e) {
			v = int(lower_bound(vertex_labels.begin(), vertex_labels.end(), v) - vertex_labels.begin());
		}

		vector<int> par(V, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) -> bool {
			a = get_par(a), b = get_par(b);
			if (a == b) return false;
			if (par[a] > par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
			return true;
		};

		vector<int> parity(V);
		for (auto e : edges) {
			merge(e[0], e[1]);
			parity[e[0]] ^= 1;
			parity[e[1]] ^= 1;
		}

		int ans = 0;
		vector<min_pq<int>> verts(V);
		for (int i = 0; i < V; i++) {
			int p = get_par(i);
			if (parity[i]) {
				ans++;
				verts[p].push(i);
			}
		}
		for (int i = 0; i < V; i++) {
			int p = get_par(i);
			if (verts[p].empty()) {
				ans += 2;
				verts[p].push(i);
				verts[p].push(i);
			}
		}

		cout << (ans / 2) - 1 << '\n';

		min_pq<std::pair<int, int>> pq;
		for (int i = 0; i < V; i++) {
			if (par[i] < 0) {
				pq.push({verts[i].top(), i});
			}
		}

		while (pq.size() >= 2 || verts[pq.top().second].size() > 2) {
			auto [v, p] = pq.top(); pq.pop();
			assert(v == verts[p].top());
			verts[p].pop();
			assert(verts[p].size() & 1);
			if (!pq.empty() && (verts[p].size() == 1 || pq.top().first < verts[p].top())) {
				auto [u, q] = pq.top(); pq.pop();
				assert(u == verts[q].top());
				verts[q].pop();
				cout << vertex_labels[v] << ' ' << vertex_labels[u] << '\n';

				if (verts[q].size() > verts[p].size()) std::swap(p, q);
				// MELD
				while (!verts[q].empty()) {
					verts[p].push(verts[q].top());
					verts[q].pop();
				}
				pq.push({verts[p].top(), p});
			} else {
				int u = verts[p].top(); verts[p].pop();
				cout << vertex_labels[v] << ' ' << vertex_labels[u] << '\n';
				assert(!verts[p].empty());
				pq.push({verts[p].top(), p});
			}
		}
	}

	return 0;
}
