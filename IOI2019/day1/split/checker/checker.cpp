#include "testlib.h"
#include <functional>
using namespace std;

int main(int argc, char * argv[])
{
	static string input_secret = "7d6697fb-2a55-4d69-94ee-21b41f37da6a";
	static string output_secret = "9e46656f-28f5-4a1b-acb6-f2309f627891";

	registerChecker("split", argc, argv);

	inf.readLine();//input_secret

	int n = inf.readInt(), m = inf.readInt();
	inf.readEoln();
	int a = inf.readInt(), b = inf.readInt(), c = inf.readInt();
	inf.readEoln();

	vector <int> p(m), q(m);
	for(int i=0; i<m; i++) {
		p[i] = inf.readInt();
		q[i] = inf.readInt();
		inf.readEoln();
	}
	inf.readEof();

	readBothSecrets(output_secret);
	readBothGraderResults();

	vector <int> expected(n);
	for(int i=0; i<n; i++) {
		expected[i] = ans.readInt();
	}
	ans.readEoln();
	ans.readEof();

	vector <int> answered(n);
	for(int i=0; i<n; i++) {
		answered[i] = ouf.readInt();
	}
	ouf.readEoln();
	ouf.readEof();

	for(int i=0; i<n; i++) {
		if(not(0 <= answered[i] and answered[i] <= 3)) {
			quitf(_pe, "answer for %d-th vertex does not belong to \{0, 1, 2, 3}", i);
		}
	}

	bool contains_zero = false, contains_plus = false;
	for(int i=0; i<n; i++) {
		if(answered[i] == 0) contains_zero = true;
		if(answered[i] > 0) contains_plus = true;
	}

	if(contains_zero and contains_plus) {
		quitf(_wa, "answer contains both zero and positive values");
	}

	if(contains_zero) {
		if(expected[0] == 0) {
			quitf(_ok, "ok, no valid answer");
		} else {
			quitf(_wa, "jury found a solution, contestant did not");
		}
	}

	vector <vector <int> > G(n);
  for(int i=0; i<m; i++) {
    G[p[i]].push_back(q[i]);
    G[q[i]].push_back(p[i]);
  }
  auto connected = [&](int c) {
    vector <bool> visited(n, false);
    function<void(int)> dfs = [&](int v) {
      visited[v] = true;
      for(auto w : G[v]) if(!visited[w] and answered[w] == c) dfs(w);
    };
    for(int i=0; i<n; i++) if(answered[i] == c) {
      dfs(i);
      break;
    }
    for(int i=0; i<n; i++) if(answered[i] == c and !visited[i]) return false;
    return true;
  };
  int cnt[4] = {0, 0, 0, 0};
  for(int i=0; i<n; i++) cnt[answered[i]]++;
  int hm = connected(1) + connected(2) + connected(3);

	if(cnt[1] == a and cnt[2] == b and cnt[3] == c and hm >= 2) {
		if(expected[0] == 0) {
			quitf(_fail, "contestant found solution, jury did not");
		} else {
			quitf(_ok, "ok, correct split");
		}
	} else {
		if(cnt[1] != a or cnt[2] != b or cnt[3] != c) {
			quitf(_wa, "invalid split: #1=%d, #2=%d, #3=%d", cnt[1], cnt[2], cnt[3]);
		} else {
			quitf(_wa, "%d components are not connected", 3-hm);
		}
	}
}
