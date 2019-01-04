#include<iostream>
#include<stack>

using namespace std;

stack<int> st;

int main() {
	int N; cin >> N;
	st.push(0);
	int res = 0;
	for(int i = 0; i < N; i++) {
		int d, w; cin >> d >> w;
		while(st.top() > w) {
			st.pop();
		}
		if(st.top() < w) {
			st.push(w);
			res++;
		}
	}
	cout << res << '\n';
	return 0;
}
