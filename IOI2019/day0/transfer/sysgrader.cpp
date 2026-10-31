#include "transfer.h"
#include <cstdio>
#include <cassert>
#include <algorithm>
#include <iostream>
using namespace std;

//Author: Kian Mirjalali

typedef vector<int> VI;

template<class C>
inline int sz(const C& c) {
	return c.size();
}

#define input_secret "3fde153a-2d9e-4cab-aaed-47175facae96"
#define pipe_secret "aad8b291-f7c9-4935-ac07-3f34523a67ab"
#define output_secret "bbda7722-086a-420f-811f-e3648ef86a25"

template<class T>
inline static void shuffle(vector<T> &v){
	srand(3532);
	random_shuffle(v.begin(), v.end());
}

class GraderException {
	string msg;
	string reason;
public:
	inline GraderException(string msg_, string reason_=""):
			msg(msg_), reason(reason_) {}

	[[noreturn]] inline void eject(FILE* f, string secret) const {
		fprintf(f, "%s\n", secret.c_str());
		fprintf(f, "%s\n", msg.c_str());
		fprintf(f, "%s\n", reason.c_str());
		fclose(f);
		exit(0);
	}
};


static const int maxn = 1000;
static const int max_attachment_size = 2*maxn;

inline static string vi2str(const VI& v, string prefix="") {
	string s(prefix);
	for (int x : v)
		s += '0'+x;
	return s;
}

inline static VI str2vi(const char* s) {
	VI v;
	for (int i=0; s[i]>0; i++)
		v.push_back(s[i]-'0');
	return v;
}

inline static VI str2vi(const char* s, string prefix) noexcept(false) {
	for (int i=0; i<sz(prefix); i++)
		if (s[i]!=prefix[i])
			throw GraderException("FAIL", "expected prefix not found: "+prefix);
	return str2vi(s+sz(prefix));
}

inline void toggle(VI& v, int index) {
	v[index] = 1 - v[index];
}


static const string attachment_prefix = "A";
struct SourceAndAttachment {

	VI source;
	VI attachment;

	inline SourceAndAttachment(VI source_={}, VI attachment_={}):
			source(source_), attachment(attachment_) {}

	inline void write(FILE* f) const {
		fprintf(f, "%s %s\n", vi2str(source).c_str(), vi2str(attachment, attachment_prefix).c_str());
	}

	inline void read(FILE* f) {
		char source_str[maxn+2], attachment_str[max_attachment_size+2];
		assert(2 == fscanf(f, "%s %s", source_str, attachment_str));
		source = str2vi(source_str);
		attachment = str2vi(attachment_str, attachment_prefix);
	}
};


inline static void pass1(char* pipe_path) {
	FILE* pipe1 = fopen(pipe_path, "w");
	try {
		fclose(stdout);

		int N;
		vector<SourceAndAttachment> sourceAndAttachments;
		{//reading input
			char secret[1000];
			assert(1 == scanf("%s", secret));
			if (string(secret) != input_secret)
				throw GraderException("SV");

			char main_source_str[maxn+2];
			assert(1 == scanf("%s", main_source_str));
			VI main_source = str2vi(main_source_str);
			N = main_source.size();
			fclose(stdin);

			sourceAndAttachments.emplace_back(main_source);
			for (int i=0; i<N; i++) {
				VI source = main_source;//full copy
				toggle(source, i);
				sourceAndAttachments.emplace_back(source);
			}
		}

		const int attachment_size_limit = 2*N;
	
		for (auto& saa : sourceAndAttachments) {
			VI given_source = saa.source;//full copy
			VI attachment = get_attachment(given_source);
			if (sz(attachment) > attachment_size_limit)
				throw GraderException("WA", "attachment too large");
			for (int x : attachment)
				if (x != 0 && x != 1)
					throw GraderException("WA", "invalid attachment integer value");
			saa.attachment = attachment;//full copy
		}
	
		{//writing intermediate data
			fprintf(pipe1, "%s\n", pipe_secret);
			fprintf(pipe1, "OK\n");
			fprintf(pipe1, "%d\n", sz(sourceAndAttachments));
			for (auto& saa : sourceAndAttachments)
				saa.write(pipe1);
			fclose(pipe1);
		}
	} catch (GraderException& ge) {
		ge.eject(pipe1, pipe_secret);
	}
}

struct Scenario {
	int sourceAndAttachmentIndex;
	int dataCorruptionIndex; // (-1 for no corruption)
	inline Scenario(int saai, int dci):
			sourceAndAttachmentIndex(saai), dataCorruptionIndex(dci) {}
};

inline static void pass2(char* pipe_path) {
	try {
		fclose(stdin);
		vector<SourceAndAttachment> sourceAndAttachments;
		{//reading intermediate data
			FILE* pipe2 = fopen(pipe_path, "r");
			if (pipe2 == NULL)
				throw GraderException("FAIL", "pipe not found: "+string(pipe_path));
			char secret[1000];
			assert(1 == fscanf(pipe2, "%s", secret));
			if (string(secret) != pipe_secret)
				throw GraderException("SV");

			char status[1000];
			assert(1 == fscanf(pipe2, "%s\n", status));
			if (string(status) != "OK") {
				string reason = "";
				while (true) {
					int c = fgetc(pipe2);
					if (c == EOF || c=='\n')
						break;
					reason += char(c);
				}
				throw GraderException(status, reason);
			}

			int n_saa;
			assert(1 == fscanf(pipe2, "%d", &n_saa));
			sourceAndAttachments.resize(n_saa);
			for (int i=0; i<n_saa; i++)
				sourceAndAttachments[i].read(pipe2);

			fclose(pipe2);
		}

		vector<Scenario> scenarios;
		for (int i=0; i<sz(sourceAndAttachments); i++) {
			int N = sz(sourceAndAttachments[i].source);
			int K = sz(sourceAndAttachments[i].attachment);
			for (int j=-1; j<N+K; j++)
				scenarios.emplace_back(i, j);
		}

		shuffle(scenarios);

		for (auto& scenario : scenarios) {
			const VI& initial_source = sourceAndAttachments[scenario.sourceAndAttachmentIndex].source;
			const VI& attachment = sourceAndAttachments[scenario.sourceAndAttachmentIndex].attachment;
			VI data(initial_source);
			data.insert(data.end(), attachment.begin(), attachment.end());
			if (scenario.dataCorruptionIndex >= 0)
				toggle(data, scenario.dataCorruptionIndex);
			VI result_source = retrieve(data);
			if (initial_source != result_source) {
				cerr << "initial source:" << endl
					 << vi2str(initial_source) << endl
					 << "attachment:" << endl
					 << vi2str(attachment) << endl
					 << "data corruption index: " << scenario.dataCorruptionIndex << endl
					 << "result source:" << endl
					 << vi2str(result_source) << endl;
				throw GraderException("WA", "wrong source retrieval");
			}
		}
	
		{//writing output
			printf("%s\n", output_secret);
			printf("OK\n");
			for (auto& sa: sourceAndAttachments)
				printf("%d\n", sz(sa.attachment));
			fclose(stdout);
		}
	} catch (GraderException& ge) {
		ge.eject(stdout, output_secret);
	}
}

int main(int argc, char **argv){
	if (argc < 3) {
		cout << "invalid arguments" << endl;
		return 1;
	}
	int type = atoi(argv[1]);
	if (type == 0) {
		pass1(argv[2]);
	} else {
		pass2(argv[2]);
	}
	return 0;
}
