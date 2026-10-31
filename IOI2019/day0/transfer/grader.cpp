#include "transfer.h"
#include <iostream>
using namespace std;


static inline string run_scenario() {
	int c;
	cin >> c;
	if (c < -1)
		return "invalid corruption index";
	string source_str;
	cin >> source_str;
	const int N = source_str.size();
	const int max_attachment_size = 2*N;
	vector<int> source;
	for (int i=0; i<N; i++)
		source.push_back(source_str[i]-'0');

	vector<int> attachment = get_attachment(source);
	if (int(attachment.size()) > max_attachment_size)
		return "attachment too large";
	for (int x : attachment)
		if (x != 0 && x != 1)
			return "invalid attachment integer value";

	vector<int> data(source);
	data.insert(data.end(), attachment.begin(), attachment.end());

	if (c >= int(data.size()))
		return "invalid corruption index";

	if (c >= 0)
		data[c] = 1-data[c];

	vector<int> result_source = retrieve(data);

	if (source != result_source)
		return "wrong source retrieval";

	return string("OK K=")+to_string(attachment.size());
}

int main() {
	int T;
	cin >> T;
	for (int scenario = 0; scenario < T; scenario++) {
		string result = run_scenario();
		cout << "scenario #" << scenario << ": " << result << endl;
	}
	return 0;
}
