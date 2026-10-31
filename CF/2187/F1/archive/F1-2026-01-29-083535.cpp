#include <bits/stdc++.h>

class PermTree {
public:
	enum class NodeType {
		LEAF,
		INCR,
		DECR,
		FULL,
		PARTIAL,
	};

	struct Node {
		std::array<int, 2> c;
		NodeType type;
		int l, r, lo, hi;
	};

	std::vector<Node> nodes;
	int root = -1;

	PermTree() {}
	Node& operator [] (int idx) { return nodes[idx]; }
	const Node& operator [] (int idx) const { return nodes[idx]; }

	int size() const { return int(nodes.size()); }

	PermTree(const std::vector<int>& A) : nodes(int(A.size())*2-1) {
		int N = int(A.size());
		std::vector<int> nxt_earlier(N);
		std::vector<int> prv_earlier(N);
		for (int i = 0; i < N; i++) {
			nxt_earlier[i] = i+1;
			prv_earlier[i] = i-1;
		}
		for (int i = N-1; i >= 0; i--) {
			int a = A[i];
			int p = prv_earlier[a];
			int n = nxt_earlier[a];
			if (p != -1) nxt_earlier[p] = n;
			if (n != N) prv_earlier[n] = p;
		}

		struct cnd_t {
			int left;
			int lo;
			int lo_gap;
			int hi;
			int hi_gap;
			int node;
		};

		std::vector<cnd_t> stk; stk.reserve(N);

		for (int i = 0; i < N; i++) {
			int a = A[i];
			while (true) {
				if (!stk.empty() && (a < stk.back().lo_gap || a > stk.back().hi_gap)) {
					assert(stk.size() >= 2);
					stk.end()[-2].lo = std::min(stk.end()[-2].lo, stk.back().lo);
					stk.end()[-2].hi = std::max(stk.end()[-2].hi, stk.back().hi);

					int n = 2 * stk.back().left - 1;
					nodes[n].c = {stk.end()[-2].node, stk.end()[-1].node};
					nodes[n].type = NodeType::PARTIAL;
					nodes[n].l = stk.end()[-2].left;
					nodes[n].r = i-1;
					nodes[n].lo = stk.end()[-2].lo;
					nodes[n].hi = stk.end()[-2].hi;

					stk.pop_back();

					stk.back().node = n;
				} else {
					break;
				}
			}

			stk.push_back({i, a, prv_earlier[a]+1, a, nxt_earlier[a]-1, 2*i});
			nodes[2*i].type = NodeType::LEAF;
			nodes[2*i].c = {-1, -1};
			nodes[2*i].l = nodes[2*i].r = i;
			nodes[2*i].lo = nodes[2*i].hi = a;

			while (stk.size() >= 2 && std::max(stk.back().hi, stk.end()[-2].hi) - std::min(stk.back().lo, stk.end()[-2].lo) == i - stk.end()[-2].left) {
				// merge these two nodes into one
				stk.end()[-2].lo = std::min(stk.end()[-2].lo, stk.back().lo);
				stk.end()[-2].hi = std::max(stk.end()[-2].hi, stk.back().hi);

				int n = 2 * stk.back().left - 1;
				nodes[n].c = {stk.end()[-2].node, stk.end()[-1].node};
				if (stk.end()[-2].lo == stk.end()[-1].lo) {
					nodes[n].type = NodeType::DECR;
				} else if (stk.end()[-2].hi == stk.end()[-1].hi) {
					nodes[n].type = NodeType::INCR;
				} else {
					nodes[n].type = NodeType::FULL;
				}
				nodes[n].l = stk.end()[-2].left;
				nodes[n].r = i;
				nodes[n].lo = stk.end()[-2].lo;
				nodes[n].hi = stk.end()[-2].hi;

				stk.pop_back();
				stk.back().node = n;
			}
		}

		assert(stk.size() == 1);
		root = stk.back().node;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
		std::vector<int> B(N);
		for (auto& b : B) { cin >> b; b--; }

		std::vector<int> P(N);
		for (int i = 0; i < N; i++) P[A[i]] = B[i];
		// we have 0..N-1 and P[0]..P[N-1]
		PermTree tree(P);
		cerr << "case " << '\n';
		for (int p : P) { cerr << p << ' '; } cerr << '\n';
		int ans = [&](this auto&& self, int cur) -> int {
			int a = tree.nodes[cur].l;
			int b = tree.nodes[cur].r;
			cerr << a << '-' << b << ' ';
			for (int i = a; i <= b; i++) cerr << P[i] << ' ';
			cerr << '\n';
			if (tree.nodes[cur].type == PermTree::NodeType::LEAF) {
				return 1;
			}
			if (tree.nodes[cur].type == PermTree::NodeType::INCR) {
				// walk all the way left and check it
				int best = 0;
				while (tree.nodes[cur].type == PermTree::NodeType::INCR) {
					assert(tree.nodes[tree.nodes[cur].c[1]].type != PermTree::NodeType::INCR);
					best = std::max(best, self(tree.nodes[cur].c[1]));
					cur = tree.nodes[cur].c[0];
				}
				if (tree.nodes[cur].type == PermTree::NodeType::LEAF) best++;
				else best = std::max(best, self(cur));
				return best;
			} else {
				return std::max(self(tree.nodes[cur].c[0]), self(tree.nodes[cur].c[1]));
			}
		}(tree.root);
		cout << ans << '\n';
	}

	return 0;
}
