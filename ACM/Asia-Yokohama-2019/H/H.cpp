#include<bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	string S; cin >> S;
	ll ans = 0;
	stack<int> stk;
	stk.push(1);
	stack<int> opStack;
	for (char c : S) {
		if (c == '(') {
			stk.push(1);
			opStack.push(-1);
		} else if (c == ')') {
			if (stk.empty()) {
				stk.push(0);
			}
			// TODO: record it
			opStack.push(stk.top());
			stk.pop();
			if (stk.empty()) {
				stk.push(0);
			}
			ans += stk.top();
			stk.top()++;
		} else if (c == '-') {
			int lastOp = opStack.top(); opStack.pop();
			if (lastOp == -1) {
				stk.pop();
			} else {
				stk.top()--;
				ans -= stk.top();
				stk.push(lastOp);
			}
		} else assert(false);
		cout << ans << '\n';
	}

	return 0;
}
