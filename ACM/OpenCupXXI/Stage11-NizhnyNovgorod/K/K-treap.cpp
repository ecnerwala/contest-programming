#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }
template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

std::mt19937 mt(48);

struct treap_node {
	mutable treap_node* p = nullptr;
	treap_node* c[2] = {nullptr, nullptr};
	std::mt19937::result_type pri = mt();
	std::array<int, 2> x{0, 0};
	int y = 0;
	std::array<int, 2> max_x{0, 0};
	int max_y;

	void update() {
		max_y = y;
		max_x = x;
		if (c[0]) {
			c[0]->p = this;
			max_x[0] = c[0]->max_x[0];
			setmax(max_y, c[0]->max_y);
		}
		if (c[1]) {
			c[1]->p = this;
			max_x[1] = c[1]->max_x[1];
			setmax(max_y, c[1]->max_y);
		}
	}

	~treap_node() {
		delete c[0];
		delete c[1];
	}
};

treap_node* merge(treap_node* a, treap_node* b) {
	if (!a) return b;
	if (!b) return a;
	treap_node* r;
	if (a->pri < b->pri) {
		r = a;
		r->c[1] = merge(a->c[1], b);
	} else {
		r = b;
		r->c[0] = merge(a, b->c[0]);
	}
	r->update();
	return r;
}

std::tuple<treap_node*, treap_node*, treap_node*> split_3(treap_node* a, int x) {
	if (!a) return {nullptr, nullptr, nullptr};
	treap_node *l, *m, *r;
	if (x <= a->x[0]) {
		r = a;
		std::tie(l, m, r->c[0]) = split_3(a->c[0], x);
	} else if (x >= a->x[1]) {
		l = a;
		std::tie(l->c[1], m, r) = split_3(a->c[1], x);
	} else {
		m = a;
		l = std::exchange(a->c[0], nullptr);
		r = std::exchange(a->c[1], nullptr);
	}
	a->update();
	return {l, m, r};
}

std::pair<treap_node*, treap_node*> split(treap_node* a, int x) {
	treap_node *l, *m, *r;
	std::tie(l, m, r) = split_3(a, x);
	if (m != nullptr) {
		assert(m->c[0] == nullptr && m->c[1] == nullptr);
		treap_node* m2 = new treap_node(*m);
		m2->pri = mt(); // pick a new priority
		m->x[1] = m2->x[0] = x;
		l = merge(l, m);
		r = merge(m2, r);
	}
	return {l, r};
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t S = 0;
	auto read_encoded = [&S]() -> int {
		int64_t v; cin >> v;
		v ^= S;
		assert(0 <= v && v <= int(1e6));
		return int(v);
	};

	map<int, map<pair<int, int>, treap_node*>> treaps;
	{
		treap_node* BASE = new treap_node();
		BASE->x[0] = -10;
		BASE->x[1] = int(1e6);
		treaps[0].insert({{BASE->x[0], BASE->x[1]}, BASE});
	}
	auto run_insert = [&](int l, int r, int y) {
		cerr << "inserting " << l << ' ' << r << ' ' << y << '\n';
		assert(l < r);
		treaps[y]
	};

	map<int, set<pair<int, int>>> intervals;
	int Q; cin >> Q;
	for (int q = 0; q < Q; q++) {
		int op; cin >> op;
		if (op == 1) {
			int y = read_encoded();
			int l = read_encoded();
			int r = read_encoded();
			y++;
			r++;

			auto& y_intervals = intervals[y];
			{
				auto it = y_intervals.lower_bound({l, -1});
				if (it != y_intervals.begin()) {
					l = max(l, prev(it)->second);
				}
			}
			{
				auto it = y_intervals.lower_bound({r, -1});
				if (it != y_intervals.begin()) {
					--it;
					if (it->second >= r) r = min(r, it->first);
				}
			}
			if (l < r) {
				auto it = y_intervals.lower_bound({l, -1});
				int cur_l = l;
				while (cur_l < r) {
					int cur_r = r;
					int nxt_l = cur_r;
					if (it != y_intervals.end() && it->first < r) {
						assert(it->second <= r);
						cur_r = it->first;
						nxt_l = it->second;
						it = y_intervals.erase(it);
					}
					if (cur_l < cur_r) {
						run_insert(cur_l, cur_r, y);
					}
					cur_l = nxt_l;
				}
				y_intervals.insert({l, r});
			}
		} else if (op == 2) {
			int l = read_encoded();
			int r = read_encoded();
			r++;

			// Query the bar at height y from [l,r)
			treap_node* &x = treaps[0].begin()->second;
			treap_node *a, *b;
			tie(a, x) = split(x, l);
			tie(x, b) = split(x, r);

			int ans = x->max_y;
			cout << ans << '\n';
			S += ans;

			x = merge(x, b);
			x = merge(a, x);
		}
	}

	return 0;
}
