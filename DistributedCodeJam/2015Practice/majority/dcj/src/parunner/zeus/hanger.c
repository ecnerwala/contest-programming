// A binary that terminates in the middle of receiving a message
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

const char command[] = {
	0x04, // receive
	0xff, // source = int32(-1) little endian
	0xff,
	0xff,
	0xff,
	0x01, // time = int32(1) little endian
	0x00,
	0x00,
	0x00,
};

#ifdef WIN32
static int GetFd(int dir) {
	const char* names[2] = { "ZSHANDLE_IN", "ZSHANDLE_OUT" };
	char* handle_s = getenv(names[dir]);
	if (handle_s == NULL)
		return -1;
	int handle = atoi(handle_s);
	return _open_osfhandle(handle, dir == 0 ? _O_RDONLY : _O_APPEND);
}
#else
static int GetFd(int dir) {
	return 3 + dir;
}
#endif


int main() {
	char buf[20];
	FILE* cmdin = fdopen(GetFd(0), "r");
	assert(cmdin);
	FILE* cmdout = fdopen(GetFd(1), "w");
	assert(cmdout);
	assert(fwrite(command, sizeof(command), 1, cmdout) == 1);
	assert(fflush(cmdout) == 0);
	assert(fread(buf, 1, sizeof(buf), cmdin) >= 0);
	return 0;
}
	

