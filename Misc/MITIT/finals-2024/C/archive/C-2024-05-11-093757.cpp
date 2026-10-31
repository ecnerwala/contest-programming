#include <bits/stdc++.h>

template <typename F> struct reverse_comparator_t {
	F f;
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) & {
		return f(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) const& {
		return f(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) && {
		return std::move(f)(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) const&& {
		return std::move(f)(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
};

template <typename F> constexpr reverse_comparator_t<std::decay_t<F>> reverse_comparator(F&& f) {
	return { std::forward<F>(f) };
}

class CartesianTree {
public:
	struct Node {
		int l, m, r; // inclusive ranges
		std::array<int, 2> c;
		int p = -1;
	};
	std::vector<Node> nodes;
	int root = -1;

	CartesianTree() {}

	Node& operator [] (int idx) { return nodes[idx]; }
	const Node& operator [] (int idx) const { return nodes[idx]; }

	int size() const { return int(nodes.size()); }

private:
	CartesianTree(std::vector<Node>&& nodes_, int root_) : nodes(std::move(nodes_)), root(root_) {}

public:

	// min-cartesian-tree, with earlier cells tiebroken earlier
	template <typename T, typename Comp = std::less<T>>
	static CartesianTree build_min_tree(const std::vector<T>& v, Comp comp = Comp()) {
		std::vector<Node> nodes(v.size()*2+1);
		std::vector<int> stk; stk.reserve(v.size());
		int root = -1;
		for (int i = 0; i <= int(v.size()); i++) {
			int cur = 2*i;
			nodes[cur].l = i;
			nodes[cur].r = i-1;
			nodes[cur].m = i-1;
			nodes[cur].c = {-1, -1};
			while (!stk.empty() && (i == int(v.size()) || comp(v[i], v[nodes[stk.back()].m]))) {
				int nxt = stk.back(); stk.pop_back();
				nodes[nxt].c[1] = cur;
				nodes[nxt].r = nodes[cur].r;
				cur = nxt;
			}
			if (i == int(v.size())) {
				root = cur;
				break;
			}
			nodes[2*i+1].l = nodes[cur].l;
			nodes[2*i+1].m = i;
			nodes[2*i+1].c[0] = cur;
			stk.push_back(2*i+1);
		}
		return {std::move(nodes), root};
	}

	// max-cartesian-tree, with earlier cells tiebroken earlier
	template <typename T, typename Comp = std::less<T>>
	static CartesianTree build_max_tree(const std::vector<T>& v, Comp comp = Comp()) {
		return build_min_tree(v, reverse_comparator(comp));
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	constexpr bool TESTING = true;
	std::mt19937 mt(48);

	int T;
	if (TESTING) {
		T = 100;
	} else {
		cin >> T;
	}
	while (T--) {
		int N;
		if (TESTING) {
			N = std::uniform_int_distribution<int>(3, 5)(mt);
		} else {
			cin >> N;
		}

		std::vector<int> actual_skills;
		if (TESTING) {
			actual_skills.resize(N);
			std::iota(actual_skills.begin(), actual_skills.end(), 0);
			std::shuffle(actual_skills.begin(), actual_skills.end(), mt);
			cerr << "skills ";
			for (int i = 0; i < N; i++) cerr << actual_skills[i] << " \n"[i+1==N];
		}

		std::vector<int> p(N);
		std::iota(p.begin(), p.end(), 0);
		std::vector<int> nxt_p(N);
		while (true) {
			std::vector<int> rounds(N);
			if (TESTING) {
				cerr << '?';
				for (int i = 0; i < N; i++) {
					cerr << ' ' << p[i]+1;
				}
				cerr << '\n' << flush;
				assert(int(p.size()) == N);
				std::vector<std::pair<int, int>> cur_p(N);
				for (int i = 0; i < N; i++) {
					cur_p[i] = {actual_skills[p[i]], i};
					rounds[i] = 0;
				}
				for (int r = 1; int(cur_p.size()) > 1; r++) {
					std::vector<std::pair<int, int>> nxt_cur_p; nxt_cur_p.reserve(cur_p.size());
					for (int i = 0; i < int(cur_p.size()); i++) {
						if (cur_p[i] < cur_p[(i+1)%int(cur_p.size())] && cur_p[i] < (i == 0 ? cur_p.back() : cur_p[i-1])) {
							rounds[cur_p[i].second] = r;
							// don't push
						} else {
							nxt_cur_p.push_back(cur_p[i]);
						}
					}
					cur_p = std::move(nxt_cur_p);
				}
				for (int i = 0; i < N; i++) {
					cerr << rounds[i] << " \n"[i+1==N];
				}
			} else {
				cout << '?';
				for (int i = 0; i < N; i++) {
					cout << ' ' << p[i]+1;
				}
				cout << '\n' << flush;
				for (int i = 0; i < N; i++) {
					cin >> rounds[i];
				}
			}
			int winner = int(std::find(rounds.begin(), rounds.end(), 0) - rounds.begin());
			std::rotate(rounds.begin(), rounds.begin() + winner, rounds.end());
			assert(rounds.front() == 0);
			rounds.front() = N;
			auto tree = CartesianTree::build_max_tree(rounds);
			for (int i = 0; i < N; i++) {
				tree[tree[2*i+1].c[0]].p = 2*i+1;
				tree[tree[2*i+1].c[1]].p = 2*i+1;
			}
			std::vector<bool> vis(2*N+1);
			std::vector<int> cur_run; cur_run.reserve(N);
			int num_runs = 0;
			nxt_p.clear();
			for (int i = 0; i < N; i++) {
				if (tree[2*i+1].l == i && tree[2*i+1].r == i) {
					num_runs++;
					for (int cur = 2*i+1; cur != -1 && !vis[cur]; cur = tree[cur].p) {
						vis[cur] = true;
						assert(cur & 1);
						assert(cur / 2 == tree[cur].m);
						cur_run.push_back(p[(winner+(cur/2))%N]);
					}
					if (num_runs & 1) {
						nxt_p.insert(nxt_p.end(), cur_run.begin(), cur_run.end());
					} else {
						nxt_p.insert(nxt_p.end(), cur_run.rbegin(), cur_run.rend());
					}
					cur_run.clear();
				}
			}
			std::swap(p, nxt_p);

			assert(num_runs >= 1);
			if (num_runs == 1) {
				break;
			}
		}
		// higher skill wins
		std::reverse(p.begin(), p.end());
		std::vector<int> skill(N, -1);
		for (int i = 0; i < N; i++) {
			skill[p[i]] = i;
		}
		if (TESTING) {
			cerr << '!';
			for (int i = 0; i < N; i++) {
				cerr << skill[i]+1 << " \n"[i+1==N];
			}
			assert(skill == actual_skills);
		} else {
			cout << '!';
			for (int i = 0; i < N; i++) {
				cout << skill[i]+1 << " \n"[i+1==N];
			}
			cout << flush;
		}
	}

	return 0;
}
