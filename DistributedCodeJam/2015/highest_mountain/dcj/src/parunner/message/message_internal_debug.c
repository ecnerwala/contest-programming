#include "message.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "zeus.h"

#define ZEUS(s) zeus_##s

#define DEBUG 1
#define MAX_MESSAGE_SIZE (8 * (1<<20))

static void Die(const char* s) {
	fputs(s, stderr);
	exit(20);
}

int NumberOfNodes() {
	return ZEUS(NumberOfNodes)();
}

int MyNodeId() {
	return ZEUS(MyNodeId)();
}

static void CheckNodeId(int node) {
	if (!DEBUG)
		return;
	if (node < 0 || node >= NumberOfNodes())
		Die("Niepoprawny numer maszyny");
}

typedef struct Buffer {
	char* buffer;
	int size;
	int pos; // for input buffers next byte to be read. for output buffers next byte to be written.
} Buffer;

static int Empty(Buffer* buffer) {
	return buffer->pos >= buffer->size;
}

static unsigned char GetRawByte(Buffer* buf) {
	if (Empty(buf)) {
		Die("Przeczytano za koncem wiadomosci");
	}
	char r = buf->buffer[buf->pos++];
	if (Empty(buf)) {
		free(buf->buffer);
		buf->buffer = NULL;
		buf->pos = 0;
		buf->size = 0;
	}
	return r;
}

static void PutRawByte(Buffer* buffer, unsigned char byte) {
	if (buffer->pos >= buffer->size) {
		buffer->size = 2*buffer->size;
		if (buffer->size < 128)
			buffer->size = 128;
		buffer->buffer = (char*)realloc(buffer->buffer, buffer->size);
		assert(buffer->buffer);
	}
	buffer->buffer[buffer->pos++] = byte;
}

#define MAX_MACHINES 100

static Buffer incoming_buffers[MAX_MACHINES];
static Buffer outgoing_buffers[MAX_MACHINES];

char recv_buffer[MAX_MESSAGE_SIZE];

int Receive(int source) {
	if (source != -1)
		CheckNodeId(source);
	if (DEBUG && source == -1) {
		int i;
		for(i=0;i<ZEUS(NumberOfNodes)();i++) {
			if (!Empty(&incoming_buffers[i]))
				Die("Receive(-1) z nieprzeczytana wiadomoscia");
		}
	}
	ZEUS(MessageInfo) mi = ZEUS(Receive)(source, recv_buffer, sizeof(recv_buffer));
	Buffer* buf = &incoming_buffers[mi.sender_id];
	if (!Empty(buf))
		Die("Receive() odebral wiadomosc od maszyny z nieprzeczytana wiadomoscia");
	assert(buf->buffer == NULL);
	buf->buffer = (char*)malloc(mi.length);
	assert(buf->buffer);
	memcpy(buf->buffer, recv_buffer, mi.length);
	buf->pos = 0;
	buf->size = mi.length;
	return mi.sender_id;
}

#define kChar 14
#define kInt 15
#define kLL 16

static void GetTag(int source, int expected) {
	if (!DEBUG)
		return;
	int tag = GetRawByte(&incoming_buffers[source]);
	if (tag != expected)
		Die("Przeczytano inny typ wartosci niz wyslano");
}

int GetInt(int source) {
	CheckNodeId(source);
	GetTag(source, kInt);
	int result = 0, i;
	for(i=0;i<sizeof(int);i++)
		result |= (int)(GetRawByte(&incoming_buffers[source])) << (8 * i);
	return result;
}

void PutInt(int target, int value) {
	CheckNodeId(target);
	if (DEBUG)
		PutRawByte(&outgoing_buffers[target], kInt);
	int i;
	for(i=0;i<sizeof(int);i++)
		PutRawByte(&outgoing_buffers[target], (0xff & (value >> (8 * i))));
}

long long GetLL(int source) {
	CheckNodeId(source);
	GetTag(source, kLL);
	long long result = 0;
	int i;
	for(i=0;i<sizeof(long long);i++)
		result |= (long long)(GetRawByte(&incoming_buffers[source])) << (8 * i);
	return result;
}

void PutLL(int target, long long value) {
	CheckNodeId(target);
	if (DEBUG)
		PutRawByte(&outgoing_buffers[target], kLL);
	int i;
	for(i=0;i<sizeof(long long);i++)
		PutRawByte(&outgoing_buffers[target], (0xff & (value >> (8 * i))));
}

char GetChar(int source) {
	CheckNodeId(source);
	GetTag(source, kChar);
	return GetRawByte(&incoming_buffers[source]);
}

void PutChar(int target, char value) {
	CheckNodeId(target);
	if (DEBUG)
		PutRawByte(&outgoing_buffers[target], kChar);
	PutRawByte(&outgoing_buffers[target], value);
}

void Send(int target) {
	CheckNodeId(target);
	Buffer* buffer = &outgoing_buffers[target];
	if (buffer->pos > sizeof(recv_buffer))
		Die("Za dluga wiadomosc");
	ZEUS(Send)(target, buffer->buffer, buffer->pos);
	free(buffer->buffer);
	buffer->buffer = NULL;
	buffer->pos = buffer->size = 0;
}
