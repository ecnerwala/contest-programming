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
	int N; cin >> N;

	vector<vector<int>> rpairs(N); // sorted from top to bottom
	vector<vector<int>> lpairs(N); // sorted from top to bottom

	vector<pair<int, int>> cur_stack; cur_stack.reserve(N);
	for (int i = 0; i < N; i++) {
		int f; cin >> f;
		while (f > 0) {
			int v = min(f, cur_stack.back().second);
			int j = cur_stack.back().first;
			rpairs[j].push_back(i);
			lpairs[i].push_back(j);
			f -= v;
			if (v == cur_stack.back().second) cur_stack.pop_back();
			else cur_stack.back().second -= v;
		}
		int h; cin >> h;
		if (h > 0) cur_stack.push_back({i, h});
	}

	vector<int> colors(N); // color of 0 means unset
	std::y_combinator([&](auto self, int l, int r) -> void {
		// we're going to walk from l..r and look for any bottom-level pairs.
		vector<pair<int, int>> cur_pairs;
		for (int i = l; i < r; ) {
			if (rpairs[i].empty()) {
				// nothing here
				i++;
				continue;
			}
			int j = rpairs[i].back();
			assert(!lpairs[j].empty() && i == lpairs[j].back());
			rpairs[i].pop_back();
			lpairs[j].pop_back();
			cur_pairs.push_back({i, j});
			i = j;
		}
		// Color all cur_pairs
		for (int a = 0; a < int(cur_pairs.size()); ) {
			int b = a;
			while (b+1 < int(cur_pairs.size()) && cur_pairs[b+1].first == cur_pairs[b].second) b++;
			// Now, color all endpoints from a.first ... b.second
			int l = cur_pairs[a].first, r = cur_pairs[b].second;
			assert(!(colors[l] && colors[r] && colors[l] == colors[r]));
			if (colors[l] == 0 && colors[r] == 0) colors[l] = 1;
			if (colors[l] != 0) {
				for (int i = a; i <= b; i++) {
					auto [x, y] = cur_pairs[i];
					if (colors[y] == 0) {
						colors[y]++;
						while (colors[y] == colors[x]) colors[y]++;
					} else {
						assert(y == r);
						if (colors[y] == colors[x]) {
							colors[x]++;
							assert(i > a);
							while (colors[x] == colors[cur_pairs[i-1].first]) {
								colors[x]++;
							}
						}
					}
				}
			} else if (colors[r] != 0) {
				for (int i = b; i >= a; i--) {
					auto [x, y] = cur_pairs[i];
					if (colors[x] == 0) {
						colors[x]++;
						while (colors[x] == colors[y]) colors[x]++;
					} else {
						assert(x == l);
						if (colors[x] == colors[y]) {
							colors[y]++;
							assert(i < b);
							while (colors[y] == colors[cur_pairs[i+1].second]) {
								colors[y]++;
							}
						}
					}
				}
			} else assert(false);
			a = b+1;
		}
		// Now that we've colored them, we can recurse
		for (auto [x, y] : cur_pairs) self(x, y);
	})(0, N-1);

	for (int i = 0; i < N; i++) {
		if (colors[i] == 0) colors[i] = 1;
	}

	cout << *max_element(colors.begin(), colors.end()) << '\n';
	for (int i = 0; i < N; i++) {
		cout << colors[i] << " \n"[i+1==N];
	}

	return 0;
}

// () (X     )
// ( )(X     )
// A BBC     B
// (   X     )
// (   X ()  )
// (   X     )
//     C
// (         )
// A         B
