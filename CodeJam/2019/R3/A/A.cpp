#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const ll L = ll(1e12);
const ll V = ll(1e10);

vector<ll> coords;
vector<int> nimbers;
vector<map<int, set<pair<int, int>>>> moves;
void precomp() {
	map<int, set<pair<int, int>>> curMoves;
	priority_queue<pair<ll, pair<int, int>>, vector<pair<ll, pair<int, int>>>, greater<pair<ll, pair<int, int>>>> evts;
	while (coords.empty() || coords.back() <= L) {
		ll t = evts.empty() ? 0 : evts.top().first;
		if (!evts.empty()) {
			int i = evts.top().second.first, j = evts.top().second.second;
			evts.pop();
			if (i > 0 && j > 0 && t == coords[i] + coords[j] + V - 1) {
				int v = nimbers[i-1] ^ nimbers[j-1];
				curMoves[v].erase(pair<int, int>(i-1, j-1));
				if (curMoves[v].empty()) curMoves.erase(v);
			} else {
				curMoves[nimbers[i] ^ nimbers[j]].insert(pair<int, int>(i, j));
			}
		}
		int v = 0;
		while(!curMoves[v].empty()) v++;

		if (coords.empty() || t > coords.back()) {
			// print the last thing
			if (!coords.empty()) {
				cerr << coords.back() << ' ' << nimbers.back() << '\n';
			}
			coords.push_back(t);
			nimbers.push_back(v);
			moves.push_back(curMoves);
			int i = int(coords.size()) - 1;
			for (int j = 0; j < int(coords.size()); j++) {
				if (j > 0 && i > 0) evts.emplace(coords[j] + t + V - 1, pair<int, int>(j, i));
				evts.emplace(coords[j] + t + V, pair<int, int>(j, i));
			}
		} else {
			// pop the things
			nimbers.back() = v;
			moves.back() = curMoves;
		}
	}
	for (int i = 0; i < int(coords.size()); i++) {
		cerr << coords[i] << ' ' << nimbers[i] << '\n';
		for (auto it : moves[i]) {
			cerr << it.first << ' ' << it.second.size() << '\n';
		}
		cerr << '\n';
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	precomp();
	int T, W; cin >> T >> W;

	for (int case_num = 1; case_num <= T; case_num ++) {
		set<pair<ll, ll>> intervals;
		intervals.emplace(1, L);

		auto make_move = [&](ll P) {
			assert(1 <= P && P <= L);
			auto it = intervals.lower_bound(pair<ll, ll>(P+1, P+1));
			assert(it != intervals.begin());
			--it;
			ll a = it->first, b = it->second;
			intervals.erase(it);
			assert(a <= P && P + V - 1 <= b);
			if (P - a >= V) {
				intervals.emplace(a, P-1);
			}
			if (b - (P + V - 1) >= V) {
				intervals.emplace(P + V, b);
			}
		};
		auto getNimber = [&]() {
			int v = 0;
			for (auto it : intervals) {
				ll l = it.second - it.first + 1;
				int ind = int(upper_bound(coords.begin(), coords.end(), l) - coords.begin()) - 1;
				v ^= nimbers[ind];
			}
			return v;
		};
		while (true) {
			ll P; cin >> P;
			if (P == -1) {
				exit(0); //WA
			} else if (P == -2) {
				break; // win
			} else if (P == -3) {
				break; // loss
			}

			make_move(P);

			int v = getNimber();
			if (v == 0) {
				// just do whatever
				for (auto it : intervals) {
					ll l = it.second - it.first + 1;
					if (l >= V) {
						cout << it.first << '\n' << flush;
						make_move(it.first);
						break;
					}
				}
			} else {
				for (auto it : intervals) {
					ll l = it.second - it.first + 1;
					int ind = int(upper_bound(coords.begin(), coords.end(), l) - coords.begin()) - 1;
					if ((v ^ nimbers[ind]) < nimbers[ind]) {
						assert(!moves[ind][v ^ nimbers[ind]].empty());
						auto mv = *moves[ind][v ^ nimbers[ind]].begin();
						ll a = coords[mv.first], b = coords[mv.second];
						assert(a + b + V <= l);
						ll pad = l - (a + b + V);
						a += pad / 2;
						b += pad - (pad / 2);
						assert(a + b + V == l);
						cout << it.first + a << '\n' << flush;
						make_move(it.first + a);
						break;
					}
				}
				//assert(getNimber() == 0);
			}
		}
	}

	return 0;
}
