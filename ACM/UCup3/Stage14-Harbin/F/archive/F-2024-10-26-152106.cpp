#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	struct pt_t {
		int64_t x, v, w;
	};
	std::vector<pt_t> pts(N);
	for (auto& [x, v, w] : pts) {
		cin >> x >> w;
		x *= 2;
	}

	std::vector<int> par(N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) {
				par[a] = par[par[a]];
			}
			a = par[a];
		}
		return a;
	};
	auto merge_into = [&](int a, int b) -> bool {
		a = get_par(a), b = get_par(b);
		assert(a != b);
		std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};

	std::vector<int> prv(N, -1);
	std::vector<int> nxt(N, -1);
	// vel is the point's velocity
	std::vector<int> vel(N);
	std::vector<int> offset(N, 0);
	int64_t cur_t = 0;

	struct evt_t {
		int64_t t;
		int x, y;
		bool operator < (const evt_t& o) const {
			return t < o.t;
		}
		bool operator > (const evt_t& o) const {
			return t > o.t;
		}
	};
	min_pq<evt_t> evts;
	auto get_evt_time = [&](int x, int y) -> std::optional<int> {
		assert(x == -1 || nxt[x] == y);
		assert(y == -1 || prv[y] == x);
		if (x == -1 || y == -1) return std::nullopt;
		int64_t px = pts[x].x + cur_t * vel[x];
		int64_t py = pts[y].x + cur_t * vel[y];
		int64_t delta_p = py - px;
		assert(delta_p >= 0);
		int v = (vel[x] - vel[y]);
		if (delta_p & 1) {
			assert(v != 2);
			delta_p --;
		}
		if (delta_p == 0 && (pts[x].x & 1) && (pts[y].x & 1)) return std::nullopt;
		if (delta_p == 0) return cur_t;
		if (v <= 0) return std::nullopt;
		assert(v == 1 || v == 2);
		return cur_t + delta_p / v;
	};
	auto check_evt = [&](int x, int y) -> void {
		auto t = get_evt_time(x, y);
		if (t) evts.push({*t, x, y});
	};

	auto dump_lst = [&]() -> void {
		cerr << "t = " << cur_t << '\n';
		int a = get_par(0);
		while (prv[a] != -1) a = prv[a];
		for (; a != -1; a = nxt[a]) {
			cerr << " pt " << a << ' ';
			cerr << "x=" << pts[a].x + cur_t * vel[a] << ' ';
			cerr << "vel=" << vel[a] << ' ';
			cerr << "x_0=" << pts[a].x << ' ';
			cerr << "offset=" << offset[a] << ' ';
			cerr << "v=" << pts[a].v << ' ';
			cerr << "w=" << pts[a].w << ' ';
			cerr << '\n';
		}
		cerr << '\n';
	};

	{
		int lst = -1;
		int64_t cur_pref = 0;
		for (auto [x, v, w] : pts) cur_pref -= w;

		std::vector<std::pair<int, int>> pt_locs(N);
		for (int i = 0; i < N; i++) {
			pt_locs[i] = {pts[i].x, i};
		}
		std::sort(pt_locs.begin(), pt_locs.end());
		for (int i = 0, j = 0; i < N; i = j) {
			int a = pt_locs[i].second;
			j++;
			while (j < N && pt_locs[j].first == pt_locs[i].first) {
				// merge i and j
				int b = pt_locs[j].second;
				assert(pts[a].x == pts[b].x);
				pts[a].w += pts[b].w;
				merge_into(b, a);
				// self-loop means dead
				prv[b] = nxt[b] = b;
				j++;
			}
			cur_pref += pts[a].w;
			pts[a].v = cur_pref;
			cur_pref += pts[a].w;

			vel[a] = (pts[a].v < 0) - (pts[a].v > 0);

			prv[a] = lst;
			if (lst != -1) nxt[lst] = a;
			lst = a;
			check_evt(prv[a], a);
		}
		nxt[lst] = -1;
	}

	// DOES NOT CHECK EVENTS
	auto swap_pts = [&](int x, int y) -> void {
		assert(nxt[x] == y);
		assert(prv[y] == x);

		prv[y] = prv[x];
		if (prv[y] != -1) nxt[prv[y]] = y;

		nxt[x] = nxt[y];
		if (nxt[x] != -1) prv[nxt[x]] = x;

		prv[x] = y;
		nxt[y] = x;
		
		assert(pts[y].v - pts[x].v == pts[x].w + pts[y].w);
		pts[x].v += 2 * pts[y].w;
		pts[y].v -= 2 * pts[x].w;
	};

	auto split_pair = [&](int x, int y) -> void {
		assert(!(cur_t & 1));

		assert(x != -1);
		assert(y != -1);
		assert(nxt[x] == y);
		assert(prv[y] == x);
		assert(vel[x] == 0);
		assert(vel[y] == 0);
		assert(pts[x].x == pts[y].x);
		int64_t p = pts[x].x;
		assert(p & 1);
		assert(offset[x] == -offset[y]);
		if (offset[x] * ((cur_t & 2) ? -1 : +1) != -1) {
			// we need to swap things
			swap_pts(x, y);
			std::swap(x, y);
		}

		assert(nxt[x] == y);
		assert(prv[y] == x);
		assert(offset[x] * ((cur_t & 2) ? -1 : +1) == -1);
		assert(offset[y] * ((cur_t & 2) ? -1 : +1) == +1);
		offset[x] = offset[y] = 0;
		vel[x] = +1;
		vel[y] = -1;
		pts[x].x = (p-1) - cur_t * vel[x];
		pts[y].x = (p+1) - cur_t * vel[y];
		check_evt(x, y);
		check_evt(prv[x], x);
		check_evt(y, nxt[y]);
	};

	struct query_t {
		int64_t t; int i, q;
	};
	std::vector<query_t> queries(Q);
	std::vector<int64_t> ans(Q);
	for (int q = 0; q < Q; q++) {
		int64_t t; int i; cin >> t >> i; i--;
		t *= 2;
		queries[q] = {t, i, q};
	}
	std::sort(queries.begin(), queries.end(), [&](auto a, auto b) { return a.t < b.t; });
	for (auto query : queries) {
		assert(query.t >= cur_t);
		while (!evts.empty() && evts.top().t <= query.t) {
			auto [nxt_t, x, y] = evts.top(); evts.pop();
			assert(nxt_t >= cur_t);
			cur_t = nxt_t;

			// teleport forwards
			assert(x != -1 && y != -1);
			if (!(par[x] < 0)) continue;
			if (!(par[y] < 0)) continue;
			if (!(nxt[x] == y && prv[y] == x)) continue;
			if (get_evt_time(x, y) != cur_t) continue;

			int64_t px = pts[x].x + cur_t * vel[x];
			int64_t py = pts[y].x + cur_t * vel[y];
			assert(py - px == 1 || py - px == 0);
			// now we merge things
			if (py - px == 1) {
				assert(!(cur_t & 1));
				if (pts[x].x & 1) {
					split_pair(prv[x], x);
				} else if (pts[y].x & 1) {
					split_pair(y, nxt[y]);
				} else assert(false);
				// that's all we're doing, we just process the new event
			} else if (py - px == 0) {
				assert(!(pts[x].x & 1));
				assert(!(pts[y].x & 1));
				// merge x and y
				if (px & 1) {
					// fake merge
					assert(cur_t & 1);
					assert(vel[x] == +1);
					assert(vel[y] == -1);
					swap_pts(x, y);
					std::swap(x, y);
					vel[x] = (pts[x].v < 0) - (pts[x].v > 0);
					vel[y] = (pts[y].v < 0) - (pts[y].v > 0);
					if (vel[x] == +1 && vel[y] == -1) {
						vel[x] = vel[y] = 0;
						// x was at +1 at cur_t - 1
						offset[x] = ((cur_t - 1) & 2) ? -1 : +1;
						offset[y] = ((cur_t - 1) & 2) ? +1 : -1;
						pts[x].x = pts[y].x = px;
					} else {
						// at cur_t + 1, they're at px - 1 and py + 1
						pts[x].x = (px - 1) - (cur_t + 1) * vel[x];
						pts[y].x = (py + 1) - (cur_t + 1) * vel[y];
						assert(!(pts[x].x & 1));
						assert(!(pts[y].x & 1));
					}
					check_evt(prv[x], x);
					check_evt(x, y);
					check_evt(y, nxt[y]);
				} else {
					assert(!(cur_t & 1));
					// real merge
					merge_into(y, x);
					pts[x].w += pts[y].w;
					pts[x].v += pts[y].w;

					nxt[x] = nxt[y];
					if (nxt[x] != -1) prv[nxt[x]] = x;
					prv[y] = nxt[y] = y;

					vel[x] = (pts[x].v < 0) - (pts[x].v > 0);
					pts[x].x = px - cur_t * vel[x];

					check_evt(x, nxt[x]);
					check_evt(prv[x], x);
				}
			} else assert(false);
		}
		assert(query.t >= cur_t);
		cur_t = query.t;
		int a = get_par(query.i);
		int64_t p = pts[a].x + cur_t * vel[a];
		if (pts[a].x & 1) {
			p += ((cur_t & 2) ? -1 : +1) * offset[a];
		}
		assert(!(p & 1));
		ans[query.q] = p / 2;
	}
	for (auto v : ans) cout << v << '\n';

	return 0;
}
