#include "split.h"
#include <cstdio>
#include <cassert>
// BEGIN SECRET
#include <string>
// END SECRET

using namespace std;

int main() {
	// BEGIN SECRET
	const string input_secret = "7d6697fb-2a55-4d69-94ee-21b41f37da6a";
	const string output_secret = "9e46656f-28f5-4a1b-acb6-f2309f627891";
	char secret[1000];
	assert(1 == scanf("%s", secret));
	if (string(secret) != input_secret) {
		printf("%s\n", output_secret.c_str());
		printf("SV\n");
		fclose(stdout);
		return 0;
	}
	// END SECRET
	int n, m, a, b, c;
	assert(5 == scanf("%d%d%d%d%d", &n, &m, &a, &b, &c));
	vector<int> p(m), q(m);
	for (int i=0; i<m; i++)
		assert(2 == scanf("%d%d", &p[i], &q[i]));
	fclose(stdin);

	vector<int> result = find_split(n, a, b, c, p, q);

	// BEGIN SECRET
	printf("%s\n", output_secret.c_str());
	if ((int)result.size() != n) {
		printf("WA\n");
		printf("Invalid length of the result.\n");
		fclose(stdout);
		return 0;
	}
	printf("OK\n");
	// END SECRET
	for (int i=0; i<(int)result.size(); i++)
		printf("%s%d", ((i>0)?" ":""), result[i]);
	printf("\n");
	fclose(stdout);
	return 0;
}
