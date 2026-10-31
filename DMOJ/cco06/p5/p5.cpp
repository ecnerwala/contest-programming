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

		vector<int> resp; resp.reserve(2*V);

		vector<int> vert_cnt(V);
		for (int i = 0; i < V; i++) {
			if (parity[i]) {
				int p = get_par(i);
				vert_cnt[p] ++;
			}
		}
		for (int i = 0; i < V; i++) {
			int p = get_par(i);
			if (parity[i]) {
				resp.push_back(i);
			} else if (!vert_cnt[p]) {
				resp.push_back(i);
				resp.push_back(i);
				vert_cnt[p] = 2;
			}
		}

		cout << (int(resp.size()) / 2) - 1 << '\n';
		for (int i = 0; i < int(resp.size()) - 2; i += 2) {
			int a = resp[i];
			int b = resp[i+1];
			int pa = get_par(a), pb = get_par(b);
			if (pa == pb && vert_cnt[pa] == 2) {
				std::swap(resp[i+1], resp[i+2]);
				b = resp[i+1];
				pb = get_par(b);
				assert(pa != pb);
			}
			cout << vertex_labels[a] << ' ' << vertex_labels[b] << '\n';
			vert_cnt[pa]--;
			vert_cnt[pb]--;
			if (pa != pb) {
				if (par[pa] > par[pb]) std::swap(pa, pb);
				par[pa] += par[pb];
				vert_cnt[pa] += vert_cnt[pb];
				par[pb] = pa;
			}
		}
	}

	return 0;
}
