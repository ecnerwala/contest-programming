#include<bits/stdc++.h>

struct state {
	int64_t tot_val;
	std::vector<int> inds;

	int nxt_ind; // store it for quick access

	friend std::ostream& operator << (std::ostream& o, const state& s) {
		o << s.tot_val << ": [";
		for (int i : s.inds) o << i << ",";
		return o << "]" << " (" << s.nxt_ind << ")";
	}

	friend bool operator < (const state& a, const state& b) {
		if (a.tot_val != b.tot_val) {
			return a.tot_val < b.tot_val;
		} else {
			return a.inds < b.inds;
		}
	}
	friend bool operator > (const state& a, const state& b) {
		return b < a;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, K; cin >> N >> K;
	vector<int> A(N); for (auto& a : A) cin >> a;
	vector<pair<int, int>> vals(N);
	for (int i = 0; i < N; i++) {
		vals[i] = {A[i], i};
	}
	sort(vals.begin(), vals.end());

	priority_queue<state, std::vector<state>, std::greater<state>> pq;
	pq.push(state{0, vector<int>({}), 0});

	for (int k = 0; k < K-1; k++) {
		state n1 = std::move(pq.top()); pq.pop();
		//cerr << n1 << '\n';
		int i = n1.nxt_ind;
		if (i == N) continue;

		n1.nxt_ind++;

		n1.inds.insert(lower_bound(n1.inds.begin(), n1.inds.end(), vals[i].second), vals[i].second);
		n1.tot_val += vals[i].first;

		if (i > 0) {
			state n2 = n1;
			n2.inds.erase(lower_bound(n2.inds.begin(), n2.inds.end(), vals[i-1].second));
			n2.tot_val -= vals[i-1].first;
			//cerr << "pushing " << n2 << '\n';
			pq.emplace(std::move(n2));
		}

		//cerr << "pushing " << n1 << '\n';
		pq.emplace(std::move(n1));
	}

	state ans = std::move(pq.top());
	cout << ans.tot_val << '\n';
	for (int z = 0; z < int(ans.inds.size()); z++) {
		cout << ans.inds[z]+1 << " \n"[z+1 == int(ans.inds.size())];
	}

	return 0;
}

// renumber the beads by (value, id)
// (a_1, a_2, a_3, ... a_k)
//   -> (a_1, a_2, ... a_k + 1)
//   -> (a_1, a_2, ... a_k, a_k + 1)
