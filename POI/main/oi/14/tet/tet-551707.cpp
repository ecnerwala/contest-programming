#include<iostream>
#include<vector>
#include<stack>

using namespace std;

const int MAXN = 1e5;
int N;
vector<int> res;
stack<int> st;
stack<int> top;

bool found[MAXN];

int main() {
	cin >> N;

	for(int i = 0; i < 2 * N; i++) {
		int v; cin >> v;
		if(found[v]) {
			while(st.top() != v) {
				res.push_back(st.size());
				top.push(st.top());
				st.pop();
			}
			//assert(st.top() == v);
			st.pop();
			while(!top.empty()) {
				st.push(top.top());
				top.pop();
			}
		} else {
			found[v] = true;
			st.push(v);
		}
	}

	cout << res.size() << '\n';
	for(int i = 0; i < res.size(); i++) cout << res[i] << '\n';
	return 0;
}
