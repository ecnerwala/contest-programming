#include "find.h"
using namespace std;

int find_pos(int n, int x)
{
	int num = query(1); // 询问有序数组中的最小值
	if (num <= x) return 1; else return 0;
}
