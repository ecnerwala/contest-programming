#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {


const int MAXN = 60;
int N;
vector<int> adj[MAXN];
bool vis[MAXN];
class BannedBook {

public:
	// deep to shallow
	vector<int> dfs(int cur) {
		assert(!vis[cur]);
		vis[cur] = true;
		vector<int> res;
		res.push_back(cur);
		for (int nxt : adj[cur]) {
			if (vis[nxt]) continue;
			auto ch = dfs(nxt);
			res.insert(res.end(), ch.begin(), ch.end());
		}
		reverse(res.begin(), res.end());
		return res;
	}

	vector <int> passAround(vector <string> fri) {
		N = int(fri.size());
		for (int i = 0; i < N; i++) {
			adj[i] = {};
			vis[i] = false;
			for (int j = 0; j < N; j++) {
				if (fri[i][j] == 'Y') adj[i].push_back(j);
			}
		}

		vector<int> res;
		for (int i = 0; i < N; i++) {
			if (!vis[i]) {
				auto v = dfs(i);
				res.insert(res.end(), v.begin(), v.end());
			}
		}
		return res;
	}
};

void my_tests() {
	// FILL IN TESTS
	{
		vector<string> v(50, string('Y', 50));
		auto res = BannedBook().passAround(v);
	}
	{
		vector<string> v(50, string('N', 50));
		auto res = BannedBook().passAround(v);
	}
}

}  // namespace ecnerwala

using ecnerwala::BannedBook;

// vim:ft=cpp
