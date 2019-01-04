#include "zeus.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>

#ifdef WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#endif

#define MAX_MESSAGE_SIZE (8*1024*1024)
#define MAGIC 1736434764
#define SEND 3
#define RECV 4

static int initialized;
static FILE* cmdin;
static FILE* cmdout;
static int nof_nodes;
static int node_id;

static unsigned char ReadByte() {
	unsigned char c;
	assert(fread(&c, 1, 1, cmdin) == 1);
	return c;
}

static int ReadInt() {
	int v = 0;
	int i;
	for(i=0;i<4;i++)
		v |= (int)(ReadByte()) << (8 * i);
	return v;
}

static void WriteByte(unsigned char c) {
	assert(fwrite(&c, 1, 1, cmdout) == 1);
}

static void WriteInt(int v) {
	int i;
	for(i=0;i<4;i++)
		WriteByte((v >> (8 * i)) & 0xff);
}

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

static void Init() {
	if (initialized)
		return;
	cmdin = fdopen(GetFd(0), "r");
	assert(cmdin != NULL);
	cmdout = fdopen(GetFd(1), "w");
	assert(cmdout != NULL);
	if (ReadInt() != MAGIC)
		assert(0);
	nof_nodes = ReadInt();
	assert(1 <= nof_nodes);
	node_id = ReadInt();
	assert(0 <= node_id && node_id < nof_nodes);
	initialized = 1;
}

int ZEUS(NumberOfNodes)() {
	Init();
	return nof_nodes;
}

ZEUS(NodeId) ZEUS(MyNodeId)() {
	Init();
	return node_id;
}

#ifdef WIN32
static int CurrentTime() {
	HANDLE me = GetCurrentProcess();
	FILETIME lpCreationTime, lpExitTime, lpKernelTime, lpUserTime;
	GetProcessTimes(me, &lpCreationTime, &lpExitTime, &lpKernelTime, &lpUserTime);
	ULONGLONG cTime =
		lpUserTime.dwLowDateTime +
		lpKernelTime.dwLowDateTime +
		(((ULONGLONG) lpUserTime.dwHighDateTime) << 32) +
		(((ULONGLONG) lpKernelTime.dwHighDateTime) << 32);
	return (int)(cTime / 10000);
}
#else
static int CurrentTime() {
	static int warned;
	int time = clock();
	if (time == -1) {
		if (!warned) {
			warned = 1;
			fprintf(stderr, "Warning: clock() returned -1; time measurements will be bogus.\n");
		}
		return 0;
	}
	return time * 1000 / CLOCKS_PER_SEC;
}
#endif

void ZEUS(Send)(ZEUS(NodeId) target, const char* message, int bytes) {
	Init();
	assert(target >= 0 && target < nof_nodes);
	assert(bytes <= MAX_MESSAGE_SIZE);
	int i;
	WriteByte(SEND);
	WriteInt(target);
	WriteInt(CurrentTime());
	WriteInt(bytes);
	for(i=0;i<bytes;i++)
		WriteByte(message[i]);
	fflush(cmdout);
}

ZEUS(MessageInfo) ZEUS(Receive)(ZEUS(NodeId) source, char* buffer, int buffer_size) {
	Init();
	assert(source >= -1 && source < nof_nodes);
	ZEUS(MessageInfo) mi;
	int i;
	WriteByte(RECV);
	WriteInt(source);
	WriteInt(CurrentTime());
	fflush(cmdout);
	if (ReadInt() != MAGIC + 1)
		assert(0);
	mi.sender_id = ReadInt();
	mi.length = ReadInt();
	assert(mi.length <= buffer_size);
	for(i=0;i<mi.length;i++)
		buffer[i] = ReadByte();
	return mi;
}
