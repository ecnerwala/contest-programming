#include <bits/stdc++.h>

std::mt19937 mt(48);

struct treap_node {
	std::array<treap_node*, 2> c;
	int val;
	std::mt19937::result_type pri = mt();
};

std::array<treap_node*, 2> split(const treap_node* a, int v) {
	if (!a) return {nullptr, nullptr};
	treap_node *l, *r;
	if (v < a->val) {
		r = new treap_node(*a);
		auto [x, y] = split(a->c[0], v);
		l = x;
		r->c[0] = y;
	} else {
		l = new treap_node(*a);
		auto [x, y] = split(a->c[1], v);
		l->c[1] = x;
		r = y;
	}
	return {l, r};
}

treap_node* insert(treap_node* a, int v, std::mt19937::result_type p) {
	if (!a || p < a->pri) {
		treap_node* r = new treap_node();
		r->val = v;
		r->pri = p;
		r->c = split(a, v);
		return r;
	} else if (v < a->val) {
		treap_node* r = new treap_node(*a);
		r->c[0] = insert(a->c[0], v, p);
		return r;
	} else {
		treap_node* r = new treap_node(*a);
		r->c[1] = insert(a->c[1], v, p);
		return r;
	}
}

treap_node* merge(treap_node* a, treap_node* b) {
	if (!a) return b;
	if (!b) return a;
	treap_node* r;
	if (a->pri < b->pri) {
		r = new treap_node(*a);
		r->c[1] = merge(a->c[1], b);
	} else {
		r = new treap_node(*b);
		r->c[0] = merge(a, b->c[0]);
	}
	return r;
}

treap_node* del(treap_node* a, int v) {
	assert(a);
	if (a->val == v) {
		return merge(a->c[0], a->c[1]);
	}
	if (v < a->val) {
		treap_node* r = new treap_node(*a);
		r->c[0] = del(a->c[0], v);
		return r;
	} else {
		treap_node* r = new treap_node(*a);
		r->c[1] = del(a->c[1], v);
		return r;
	}
}

// strict successor of lo
std::optional<int> succ(treap_node* rt, int lo) {
	std::optional<int> cnd = std::nullopt;
	while (rt) {
		if (lo < rt->val) {
			cnd = rt->val;
			rt = rt->c[0];
		} else {
			rt = rt->c[1];
		}
	}
	return cnd;
}
std::optional<int> pred(treap_node* rt, int lo) {
	std::optional<int> cnd = std::nullopt;
	while (rt) {
		if (lo > rt->val) {
			cnd = rt->val;
			rt = rt->c[1];
		} else {
			rt = rt->c[0];
		}
	}
	return cnd;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<array<int, 2>> P(N+1);
	for (int i = 0; i < N; i++) {
		cin >> P[i][0] >> P[i][1];
	}
	P[N] = P[0];

	map<int, map<int, int>> vert_mp;
	map<int, map<int, int>> horz_mp;

	for (int i = 0; i < N; i++) {
		auto a = P[i], b = P[i+1];
		if (a[0] == b[0]) {
			auto [l, r] = std::minmax(a[1], b[1]);
			auto& mp = vert_mp[a[0]];
			mp[l]++;
			mp[r]--;
		} else if (a[1] == b[1]) {
			auto [l, r] = std::minmax(a[0], b[0]);
			auto& mp = horz_mp[a[1]];
			mp[l]++;
			mp[r]--;
		} else assert(false);
	}

	using seg_t = pair<array<int, 2>, bool>;
	auto coalesce_segs = [&](const map<int, map<int, int>>& prefs) -> vector<seg_t> {
		vector<seg_t> resp; resp.reserve(N);
		for (const auto& [x, mp] : prefs) {
			int tot = 0;
			for (auto [y, cur] : mp) {
				if (tot == 0) {
					resp.push_back({{x, y}, false});
				}
				assert(tot > 0 || cur != 0);
				tot += cur;
				assert(tot >= 0);
				if (tot == 0) {
					resp.push_back({{x, y}, true});
				}
			}
		}
		return resp;
	};

	const auto vert_seg = coalesce_segs(vert_mp);
	const auto horz_seg = coalesce_segs(horz_mp);

	auto build_treaps = [&](vector<seg_t> segs) -> vector<pair<int, treap_node*>> {
		for (auto& a : segs) {
			swap(a.first[0], a.first[1]);
		}
		sort(segs.begin(), segs.end());
		vector<pair<int, treap_node*>> resp; resp.reserve(segs.size()+1);
		treap_node* rt = nullptr;
		resp.push_back({-1, rt});
		for (auto [p, op] : segs) {
			auto [x, y] = p;
			if (op == false) {
				// insert
				rt = insert(rt, y, mt());
			} else if (op == true) {
				// delete
				rt = del(rt, y);
			}
			resp.push_back({x, rt});
		}
		return resp;
	};

	auto vert_treaps = build_treaps(vert_seg);
	auto horz_treaps = build_treaps(horz_seg);
	auto treap_cmp = [&](pair<int, treap_node*> a, pair<int, treap_node*> b) {
		return a.first < b.first;
	};

	const int UP = 0;
	const int RIGHT = 1;
	const int DOWN = 2;
	const int LEFT = 3;

	const array<int, 2> st_cur = vert_seg.front().first;
	const int st_dir = UP;

	array<int, 2> cur = st_cur;
	int dir = st_dir;

	int64_t ans = 0;
	do {
		if (dir == UP) {
			auto max_y = upper_bound(vert_seg.begin(), vert_seg.end(), seg_t{cur, false})->first[1];
			assert(cur[1] < max_y);
			auto rt = prev(lower_bound(horz_treaps.begin(), horz_treaps.end(), make_pair(cur[0], nullptr), treap_cmp))->second;
			auto v = succ(rt, cur[1]);
			if (v && *v <= max_y) {
				dir--;
				cur = {*v, cur[0]};
			} else {
				dir++;
				cur = {max_y, cur[0]};
			}
		} else if (dir == RIGHT) {
			auto max_y = upper_bound(horz_seg.begin(), horz_seg.end(), seg_t{cur, false})->first[1];
			assert(cur[1] < max_y);
			auto rt = prev(upper_bound(vert_treaps.begin(), vert_treaps.end(), make_pair(cur[0], nullptr), treap_cmp))->second;
			auto v = succ(rt, cur[1]);
			if (v && *v <= max_y) {
				ans += int64_t(*v - cur[1]) * cur[0];
				dir--;
				cur = {*v, cur[0]};
			} else {
				ans += int64_t(max_y - cur[1]) * cur[0];
				dir++;
				cur = {max_y, cur[0]};
			}
		} else if (dir == DOWN) {
			auto min_y = prev(lower_bound(vert_seg.begin(), vert_seg.end(), seg_t{cur, true}))->first[1];
			assert(min_y < cur[1]);
			auto rt = prev(upper_bound(horz_treaps.begin(), horz_treaps.end(), make_pair(cur[0], nullptr), treap_cmp))->second;
			auto v = pred(rt, cur[1]);
			if (v && min_y <= *v) {
				dir--;
				cur = {*v, cur[0]};
			} else {
				dir++;
				cur = {min_y, cur[0]};
			}
		} else if (dir == LEFT) {
			auto min_y = prev(lower_bound(horz_seg.begin(), horz_seg.end(), seg_t{cur, true}))->first[1];
			assert(min_y < cur[1]);
			auto rt = prev(lower_bound(vert_treaps.begin(), vert_treaps.end(), make_pair(cur[0], nullptr), treap_cmp))->second;
			auto v = pred(rt, cur[1]);
			if (v && min_y <= *v) {
				ans -= int64_t(cur[1] - *v) * cur[0];
				dir--;
				cur = {*v, cur[0]};
			} else {
				ans -= int64_t(cur[1] - min_y) * cur[0];
				dir++;
				cur = {min_y, cur[0]};
			}
		} else assert(false);
		dir &= 3;
	} while (!(cur == st_cur && dir == st_dir));

	cout << ans << '\n';

	return 0;
}
