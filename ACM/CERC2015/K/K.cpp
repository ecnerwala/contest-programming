#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N; N *= 2;

	vector<int> outEdge(N);
	for (int i = 0; i < N; i++) {
		cin >> outEdge[i]; outEdge[i] --;
	}

	vector<int> inDeg(N);
	for (int i = 0; i < N; i++) {
		inDeg[outEdge[i]]++;
	}

	vector<bool> take(N);
	vector<bool> used(N);

	vector<int> q; q.reserve(N);
	for (int i = 0; i < N; i++) {
		if (inDeg[i] == 0) {
			used[i] = true;
			take[i] = true;
			q.push_back(i);
		}
	}
	for (int z = 0; z < int(q.size()); z++) {
		int cur = q[z];
		assert(used[cur]);
		int nxt = outEdge[cur];
		if (take[cur]) {
			if (used[nxt]) {
				assert(take[nxt] == false);
			} else {
				used[nxt] = true;
				take[nxt] = false;
				q.push_back(nxt);
			}
		} else {
			if (used[nxt]) {
				// don't bother doing anything
			} else {
				inDeg[nxt] --;
				if (inDeg[nxt] == 0) {
					used[nxt] = true;
					take[nxt] = true;
					q.push_back(nxt);
				}
			}
		}
	}

	for (int i = 0; i < N; i++) {
		if (!used[i]) {
			take[i] = i < N/2;
		}
	}

	vector<int> ans;
	for (int i = 0; i < N; i++) if (take[i]) ans.push_back(i);
	for (int i = 0; i < int(ans.size()); i++) {
		cout << ans[i]+1 << " \n"[i+1==int(ans.size())];
	}

	return 0;
}
