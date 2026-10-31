#include "cycle.h"
#include <cstdio>
#include <cassert>
#include <string>

using namespace std;

static const int cnt_lim = 35;
static int n, p, cnt;

[[noreturn]] inline static void wrong_answer(string reason) {
	printf("%s\n", reason.c_str());
	fclose(stdout);
	exit(0);
}

bool jump(int x) {
	if (x < 0 || x >= n)
		wrong_answer("Invalid argument.");
	cnt++;
	if (cnt > cnt_lim)
		wrong_answer("Too many queries.");

	p = (p + x) % n;
	return (n - p) % n <= n / 2;
}

int main() {
	assert(1 == scanf("%d", &n));
	assert(1 == scanf("%d", &p));
	fclose(stdin);

	cnt = 0;
	escape(n);
	if (p != 0)
		wrong_answer("The exit was not found.");

	printf("OK\n");
	fprintf(stderr, "%d\n", cnt);
	fclose(stdout);
	return 0;
}
