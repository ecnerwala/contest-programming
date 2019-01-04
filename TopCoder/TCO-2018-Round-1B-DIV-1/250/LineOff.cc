#include <bits/stdc++.h>
using namespace std;

class LineOff {

public:

	int movesToDo(string points) {
		int res = 0;
		stack<char> cur;
		for (char c : points) {
			if (!cur.empty() && cur.top() == c) {
				cur.pop();
				res ++;
			} else {
				cur.push(c);
			}
		}
		return res;
	}
};

// vim:ft=cpp
