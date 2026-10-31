#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	if (N % 2) {
		cout << -1 << '\n';
		exit(0);
	}

	map<string, int> name_map;
	auto get_id = [&name_map](string s) -> int {
		auto it = name_map.find(s);
		if (it != name_map.end()) return it->second;
		int v = int(name_map.size());
		name_map.emplace(s, v);
		return v;
	};

	vector<int> ptr(N, -1);
	for (int i = 0; i < N; i++) {
		string s_, t_; cin >> s_ >> t_;
		int s = get_id(s_), t = get_id(t_);
		assert(ptr[s] == -1);
		ptr[s] = t;
	}

	for (int i = 0; i < N; i++) {
		// if you point to a 2-cycle, they must be together, so you might as well point at yourself
		if (ptr[i] != i && ptr[ptr[i]] != i && ptr[ptr[i]] != ptr[i] && ptr[ptr[ptr[i]]] == ptr[i]) {
			ptr[i] = i;
		}
	}

	int num_2cyc = 0;
	for (int i = 0; i < N; i++) {
		// if you're in a 2-cycle, the answer is 1 cheaper
		if (ptr[i] != i && ptr[ptr[i]] == i) {
			num_2cyc++;
		}
	}
	assert(num_2cyc % 2 == 0);
	num_2cyc /= 2;

	// now it's just max matching, which we can do by greedy
	int num_matching = 0;
	vector<bool> alive(N, true);

	// indeg is # of alive children, maintained only if you're also alive
	vector<int> indeg(N);
	for (int i = 0; i < N; i++) {
		indeg[ptr[i]]++;
	}

	vector<int> q; q.reserve(N);
	for (int i = 0; i < N; i++) {
		if (!indeg[i]) q.push_back(i);
	}
	for (int z = 0; z < int(q.size()); z++) {
		int i = q[z];
		assert(!indeg[i]);
		if (!alive[i]) continue;
		alive[i] = false;
		if (alive[ptr[i]]) {
			alive[ptr[i]] = false;
			num_matching++;
			if (alive[ptr[ptr[i]]] && --indeg[ptr[ptr[i]]] == 0) {
				q.push_back(ptr[ptr[i]]);
			}
		} else {
			// alive[ptr[i]] = is already false
		}
	}

	for (int i = 0; i < N; i++) {
		if (!alive[i]) continue;
		int cyc_len = 0;
		for (int cur = i; alive[cur]; cur = ptr[cur]) alive[cur] = false, cyc_len++;
		num_matching += cyc_len / 2;
	}

	int ans = N - num_2cyc - num_matching;
	cout << ans << '\n';

	return 0;
}
