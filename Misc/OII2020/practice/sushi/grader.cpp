#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int sushi(int N, int B, vector<int> A);

int main() {
	// ifstream cin("input.txt");
	// ofstream cout("output.txt");

	int N, B;
	cin >> N >> B;

	vector<int> A(N);
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}

	cout << sushi(N, B, A) << endl;

	return 0;
}
