#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	struct tree_t {
		int verts;
		std::vector<int> ch;
		std::string paren_seq;
		int nimber;
		int ch_nimber;
		// new tree, extra nimber
		std::vector<std::pair<int, int>> interior_moves;
		// extra nimber
		std::vector<int> root_moves;
	};
	auto canonize = [&](std::vector<int>& ch) -> void {
		std::sort(ch.begin(), ch.end());
		std::reverse(ch.begin(), ch.end());
		while (!ch.empty() && ch.back() == -1) ch.pop_back();
	};
	auto uniquify = [&](auto& v) {
		std::sort(v.begin(), v.end());
		v.resize(std::unique(v.begin(), v.end()) - v.begin());
	};
	std::map<std::vector<int>, int> tree_id;
	std::vector<tree_t> trees; trees.reserve(1e7);

	auto compute_tree = std::y_combinator([&](auto self, std::vector<int> dfs_ch) -> int {
		if (tree_id.count(dfs_ch)) return tree_id[dfs_ch];

		int idx = int(trees.size());
		tree_id.insert({dfs_ch, idx});
		trees.emplace_back();
		auto& t = trees.back();
		t.ch = dfs_ch;

		t.verts = 1;
		for (int c : t.ch) {
			t.verts += trees[c].verts;
		}

		t.paren_seq.reserve(2*t.verts);
		t.paren_seq += '(';
		for (int c : t.ch) {
			t.paren_seq += trees[c].paren_seq;
		}
		t.paren_seq += ')';

		t.ch_nimber = 0;
		for (int c : t.ch) {
			t.ch_nimber ^= trees[c].nimber;
		}

		t.root_moves.push_back(0);
		for (int c : t.ch) {
			std::vector<int> n_root_moves; n_root_moves.reserve(int(t.root_moves.size()) * 2);
			for (int v : t.root_moves) {
				n_root_moves.push_back(v ^ trees[c].nimber);
				n_root_moves.push_back(v ^ trees[c].ch_nimber);
			}
			uniquify(n_root_moves);
			int sz = int(n_root_moves.size());
			assert((sz & (sz - 1)) == 0);
			t.root_moves = std::move(n_root_moves);
		}

		for (auto v : t.root_moves) {
			t.interior_moves.push_back({-1, v});
		}

		for (int z = 0; z < int(t.ch.size()); z++) {
			int c = t.ch[z];

			// fully interior move
			for (auto [nc, v] : trees[c].interior_moves) {
				auto nch = t.ch;
				nch[z] = nc;
				canonize(nch);
				int nt = self(nch);
				t.interior_moves.push_back({nt, v});
			}

			// root move
			for (auto v : trees[c].root_moves) {
				t.interior_moves.push_back({-1, t.ch_nimber ^ trees[c].nimber ^ v});
			}
		}

		uniquify(t.interior_moves);
		std::vector<int> moves;
		for (auto [nt, v] : t.interior_moves) {
			moves.push_back((nt == -1 ? 0 : trees[nt].nimber) ^ v);
		}
		uniquify(moves);
		t.nimber = 0;
		while (t.nimber < int(moves.size()) && moves[t.nimber] == t.nimber) {
			t.nimber++;
		}

		return idx;
	});

	std::mt19937 mt(48);
	for (int V = 1; V <= 10; V++) {
		cerr << "V = " << V << endl;
		std::vector<int> dfs_ch;
		std::y_combinator([&](auto self, int cur, int rem) -> void {
			assert(rem >= 0);
			if (rem == 0) {
				int idx = compute_tree(dfs_ch);
				const auto& t = trees[idx];
				if (t.nimber == 0) {
					cerr << "tree " << idx << ' ' << t.paren_seq << '\n';
					cerr << " nimber = " << t.nimber << ' ' << t.ch_nimber << '\n';
					/*
					cerr << " ch ="; for (int c : t.ch) { cerr << " " << c; } cerr << '\n';
					cerr << " root moves";
					for (int v : t.root_moves) { cerr << " " << v; } cerr << '\n';
					cerr << " interior moves" << '\n';
					for (auto [nt, v] : t.interior_moves) {
						cerr << "  " << nt << ' ' << v << ' ' << ((nt == -1 ? 0 : trees[nt].nimber) ^ v) << '\n';
					}
					*/
				}
				return;
			}
			for (; cur >= 0; cur--) {
				if (rem >= trees[cur].verts) {
					dfs_ch.push_back(cur);
					self(cur, rem - trees[cur].verts);
					dfs_ch.pop_back();
				}
			}
		})(int(trees.size())-1, V-1);
		cerr << "num trees: " << V << ' ' << int(trees.size()) << '\n';
	}

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;

		cout << "Case #" << case_num << ": ";
	}

	return 0;
}
