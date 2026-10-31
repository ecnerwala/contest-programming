#include<bits/stdc++.h>
using namespace std;

using ll = long long;
struct frac {
	ll n, d;
	explicit frac(ll n_ = 0, ll d_ = 1) : n(n_), d(d_) {
		assert(d);
		if (d < 0) {
			n = -n, d = -d;
		}
	}
	bool is_int() const {
		return n % d == 0;
	}
	friend ostream& operator << (ostream& o, const frac& f) {
		return o << f.n << "/" << f.d;
	}
	friend bool operator < (frac a, frac b) { return a.n * b.d < b.n * a.d; }
	friend bool operator <= (frac a, frac b) { return a.n * b.d <= b.n * a.d; }
	friend bool operator > (frac a, frac b) { return a.n * b.d > b.n * a.d; }
	friend bool operator >= (frac a, frac b) { return a.n * b.d >= b.n * a.d; }
	friend bool operator == (frac a, frac b) { return a.n * b.d == b.n * a.d; }
	friend bool operator != (frac a, frac b) { return a.n * b.d != b.n * a.d; }
};

const int MAXP = 600;
const int MAXH = 60;
int P, H;
ll V[MAXP][MAXH];
int line_ind[MAXP];
ll line_pref[MAXP];

int sorted_inds[MAXP];
int sorted_rank[MAXP];

int true_rank[MAXP];
frac last_rank_change[MAXP];
int best_rank[MAXP];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> P >> H;
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < H; j++) {
			cin >> V[i][j];
		}
		sort(V[i], V[i] + H);
		line_ind[i] = 0;
		line_pref[i] = 0;
	}

	for (int i = 0; i < P; i++) {
		sorted_inds[i] = i;
		sorted_rank[i] = i;
		true_rank[i] = P;
		best_rank[i] = true_rank[i];
		last_rank_change[i] = frac(0);
	}

	map<frac, set<int>> evts;

	for (int i = 0; i < P; i++) {
		evts[frac(0)].insert(i);
		for (int j = 0; j < H; j++) {
			evts[frac(V[i][j])].insert(i);
		}
	}

	for (const auto& it : evts) {
		frac x = it.first;
		for (int i : it.second) {
			while (line_ind[i] < H && frac(V[i][line_ind[i]]) <= x) {
				line_pref[i] += V[i][line_ind[i]];
				line_ind[i] ++;
			}
		}
		set<int, greater<int>> change_ranks;
		for (int i : it.second) {
			change_ranks.insert(sorted_rank[i]);
		}

		// recompute everything in change_inds in reverse order
		for (int r : change_ranks) {
			while (r + 1 < P) {
			}
		}
	}

	return 0;
}
