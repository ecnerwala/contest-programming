#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
#include <bits/extc++.h>
using namespace std;


namespace ecnerwala {

using namespace __gnu_pbds;

using ordered_set = tree<
	int,
	null_type,
	less<int>,
	rb_tree_tag,
	tree_order_statistics_node_update>;

using ll = long long;

class RookAttacks {

public:

	vector<ll> classify(int D, vector <int> R0, vector <int> C0, vector <int> T0, int seed, int steps) {
		vector<int> R;
		vector<int> C;
		vector<int> T;
		set<pair<int, int>> filled;

		map<int, vector<pair<int, int>>> rows;
		map<int, vector<pair<int, int>>> cols;

		auto add_rook = [&] (int r, int c, int t) {
			pair<int, int> pos(r,c);
			if (filled.count(pos)) return;
			filled.insert(pos);
			R.push_back(r);
			C.push_back(c);
			T.push_back(t);
			rows[r].emplace_back(c, t);
			cols[c].emplace_back(r, t);
		};

		for (int i = 0; i <= int(R0.size())-1; i++) {
			add_rook(R0[i], C0[i], T0[i]);
		}

		ll state = seed;
		for (int i = 0; i <= steps-1; i++) {
			state = (state * 1103515245 + 12345) % (1ll << 31);
			int r = int(state % D);
			state = (state * 1103515245 + 12345) % (1ll << 31);
			int c = int(state % D);
			state = (state * 1103515245 + 12345) % (1ll << 31);
			int t = int(state / (1ll << 30));
			add_rook(r, c, t);
		}

		ll E[5][5]; // white, black
		memset(E, 0, sizeof(E));

		auto processLine = [&](vector<pair<int, int>>& v) {
			v.emplace_back(-1, 2);
			v.emplace_back(D, 2);
			sort(v.begin(), v.end());

			for (int i = 0; i+1 < int(v.size()); i++) {
				int w = (v[i].second == 0) + (v[i+1].second == 0);
				int b = (v[i].second == 1) + (v[i+1].second == 1);
				E[w][b] += v[i+1].first - v[i].first - 1;
			}
		};

		for (auto& it : rows) {
			processLine(it.second);
		}
		for (auto& it : cols) {
			processLine(it.second);
		}

		ordered_set cnts[3][3];
		for (auto& it : cols) {
			reverse(it.second.begin(), it.second.end()); // we'll use is as a queue

			auto oldBack = it.second.back();
			it.second.pop_back();
			assert(oldBack.second == 2);

			int c = it.first;
			int t = it.second.back().second;
			cnts[t==0][t==1].insert(c);
			it.second.push_back(oldBack);
		}

		for (auto& it : rows) {
			int r = it.first;
			// queries first
			auto doQuery = [&] (int wr, int br, int l, int r) {
				l++; // half-open
				for (int w = 0; w <= 2; w++) {
					for (int b = 0; w + b <= 2; b++) {
						int c = int(cnts[w][b].order_of_key(r) - cnts[w][b].order_of_key(l));
						E[w][b] -= c;
						E[wr][br] -= c;
						E[w + wr][b + br] += c;
					}
				}
			};

			const auto& v = it.second;
			for (int i = 0; i + 1 < int(v.size()); i++) {
				doQuery((v[i].second == 0) + (v[i+1].second == 0), (v[i].second == 1) + (v[i+1].second == 1), v[i].first, v[i+1].first);
			}

			for (int i = 1; i + 1 < int(v.size()); i++) {
				// update the column
				int c = v[i].first;
				auto& col = cols[c];
				int t0 = col.back().second;
				col.pop_back();
				int t1 = col.back().second;
				assert(t1 == v[i].second);
				col.pop_back();
				int t2 = col.back().second;
				col.emplace_back(r, t1); // put it back

				cnts[(t0==0) + (t1==0)][(t0==1) + (t1==1)].erase(c);
				cnts[(t2==0) + (t1==0)][(t2==1) + (t1==1)].insert(c);
			}
		}

		vector<ll> res(25);
		for (int w = 0; w < 5; w++) {
			for (int b = 0; b < 5; b++) {
				res[5*w+b] = E[w][b];
			}
		}
		res[0] = 1ll * D * D - int(T.size());
		for (int i = 1; i < 25; i++) res[0] -= res[i];
		return res;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::RookAttacks;

// vim:ft=cpp
