#include <bits/stdc++.h>
using namespace std;

class TreesAndBrackets {

	void remove_rest(string::iterator& t1) {
		while (*t1 == '(') {
			t1 ++;
			remove_rest(t1);
			assert(*t1 == ')');
			t1 ++;
		}
		assert(*t1 == ')');
	}

	bool recurse(string::iterator& t1, string::iterator& t2) {
		assert(*t1 == '(');
		assert(*t2 == '(');
		t1 ++;
		t2 ++;
		while (*t2 != ')') {
			if (*t1 == ')') {
				t1 ++;
				return false;
			}
			string::iterator t2p = t2;
			if (recurse(t1, t2p)) {
				t2 = t2p;
			}
		}

		assert(*t2 == ')');
		t2++;

		remove_rest(t1);
		assert(*t1 == ')');
		t1 ++;

		return true;
	}

	bool go(string t1, string t2) {
		string::iterator a = t1.begin(), b = t2.begin();
		return recurse(a, b);
	}

public:

	string check(string t1, string t2) {
		return go(t1, t2) ? "Possible" : "Impossible";
	}
};

// vim:ft=cpp
