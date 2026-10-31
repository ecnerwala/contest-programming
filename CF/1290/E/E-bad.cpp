#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 3.1e5;
int N;
int A[MAXN];
int invA[MAXN];
vector<pair<int, int>> evts[MAXN];

const int S = 1 << 18;
struct {
	int weight;
	int lazy;

	ll val;
} seg[2*S];
void update(int i) {
	if (i >= S) {
		seg[i].val = 1ll * seg[i].weight * seg[i].lazy;
	} else {
		seg[i].weight = seg[2*i].weight + seg[2*i+1].weight;
		seg[i].val = seg[2*i].val + seg[2*i+1].val + (1ll * seg[i].weight * seg[i].lazy);
	}
}
void updateAll(int a) {
	for (; a; a /= 2) update(a);
}

void modifyWt(int a, int v) {
	seg[S+a].weight += v;
	updateAll(S+a);
}
void modifyLazy(int r, int v) {
	for (int a = S, b = S+r; a < b; a /= 2, b /= 2) {
		if (b & 1) {
			--b;
			seg[b].lazy += v;
			update(b);
		}
	}
	updateAll(S+r);
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 1; i <= N; i++) {
		cin >> A[i];
		invA[A[i]] = i;
	}

	A[0] = A[N+1] = 0;

	{
		stack<int> st;
		for (int i = 1; i <= N; i++) {
			int curT = N+1;
			while (!st.empty() && st.top() > A[i]) {
				evts[st.top()].emplace_back(invA[st.top()], -1);
				evts[curT].emplace_back(invA[st.top()], +1);
				curT = st.top();
				st.pop();
			}
			st.push(A[i]);

			evts[A[i]].emplace_back(0, -1);
			evts[curT].emplace_back(0, +1);
		}
	}

	{
		stack<int> st;
		for (int i = N; i >= 1; i--) {
			int curT = N+1;
			while (!st.empty() && st.top() > A[i]) {
				evts[st.top()].emplace_back(invA[st.top()], +1);
				evts[curT].emplace_back(invA[st.top()], -1);
				curT = st.top();
				st.pop();
			}
			st.push(A[i]);

			evts[A[i]].emplace_back(N+1, +1);
			evts[curT].emplace_back(N+1, -1);
		}
	}

	modifyWt(0, +1);
	modifyWt(N+1, +1);
	for (int t = 1; t <= N; t++) {
		cerr << "time = " << t << '\n';
		modifyWt(invA[t], +1);
		cerr << " modifyWt " << invA[t] << '\n';
		for (auto evt : evts[t]) {
			cerr << " modifyLazy " << evt.first << ' ' << evt.second << '\n';
			modifyLazy(evt.first, evt.second);
		}
		ll ans = seg[1].val;
		ans -= t;
		cout << ans << '\n';
	}

	return 0;
}
