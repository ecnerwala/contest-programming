#include "message.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "zeus.h"

#define ZEUS_WRAP(s) zeus_##s

#define DEBUG 1
#define MAX_MESSAGE_SIZE (8 * (1 << 20))
#define MAX_MACHINES 100

static void Die(const char* s) {
  fputs(s, stderr);
  exit(20);
}

int NumberOfNodes() { return ZEUS_WRAP(NumberOfNodes)(); }

int MyNodeId() { return ZEUS_WRAP(MyNodeId)(); }

static void CheckNodeId(int node) {
  if (!DEBUG) return;
  if (node < 0 || node >= NumberOfNodes()) Die("Incorrect machine number");
}

typedef struct Buffer {
  char* buffer;
  int size;
  int pos;  // for input buffers next byte to be read. for output buffers next
            // byte to be written.
} Buffer;

static int Empty(Buffer* buffer) { return buffer->pos >= buffer->size; }

static unsigned char GetRawByte(Buffer* buf) {
  if (Empty(buf)) {
    Die("Read past the end of the message");
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
    buffer->size = 2 * buffer->size;
    if (buffer->size < 128) buffer->size = 128;
    buffer->buffer = (char*)realloc(buffer->buffer, buffer->size);
    assert(buffer->buffer);
  }
  buffer->buffer[buffer->pos++] = byte;
}

static Buffer incoming_buffers[MAX_MACHINES];
static Buffer outgoing_buffers[MAX_MACHINES];

char recv_buffer[MAX_MESSAGE_SIZE];

int Receive(int source) {
  if (source != -1) CheckNodeId(source);
  if (DEBUG && source == -1) {
    int i;
    for (i = 0; i < ZEUS_WRAP(NumberOfNodes)(); i++) {
      if (!Empty(&incoming_buffers[i]))
        Die("Cannot call Receive(-1) if any message is unread.");
    }
  }
  ZEUS_WRAP(MessageInfo) mi =
      ZEUS_WRAP(Receive)(source, recv_buffer, sizeof(recv_buffer));
  Buffer* buf = &incoming_buffers[mi.sender_id];
  if (DEBUG && !Empty(buf))
    Die("Receive()'ed a message when the previous one wasn't consumed.");
  if (buf->buffer != NULL) {
    free(buf->buffer);
    buf->buffer = NULL;
  }
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
  if (!DEBUG) return;
  int tag = GetRawByte(&incoming_buffers[source]);
  if (tag != expected) Die("Type mismatch between read and requested types");
}

int GetInt(int source) {
  CheckNodeId(source);
  GetTag(source, kInt);
  int result = 0, i;
  for (i = 0; i < 4; i++)
    result |= (int)(GetRawByte(&incoming_buffers[source])) << (8 * i);
  return result;
}

void PutInt(int target, int value) {
  CheckNodeId(target);
  if (DEBUG) PutRawByte(&outgoing_buffers[target], kInt);
  int i;
  for (i = 0; i < 4; i++)
    PutRawByte(&outgoing_buffers[target], (0xff & (value >> (8 * i))));
}

long long GetLL(int source) {
  CheckNodeId(source);
  GetTag(source, kLL);
  long long result = 0;
  int i;
  for (i = 0; i < 8; i++)
    result |= (long long)(GetRawByte(&incoming_buffers[source])) << (8 * i);
  return result;
}

void PutLL(int target, long long value) {
  CheckNodeId(target);
  if (DEBUG) PutRawByte(&outgoing_buffers[target], kLL);
  int i;
  for (i = 0; i < 8; i++)
    PutRawByte(&outgoing_buffers[target], (0xff & (value >> (8 * i))));
}

char GetChar(int source) {
  CheckNodeId(source);
  GetTag(source, kChar);
  return GetRawByte(&incoming_buffers[source]);
}

void PutChar(int target, char value) {
  CheckNodeId(target);
  if (DEBUG) PutRawByte(&outgoing_buffers[target], kChar);
  PutRawByte(&outgoing_buffers[target], value);
}

void Send(int target) {
  CheckNodeId(target);
  Buffer* buffer = &outgoing_buffers[target];
  if (buffer->pos > (int)sizeof(recv_buffer)) Die("Message too long");
  ZEUS_WRAP(Send)(target, buffer->buffer, buffer->pos);
  free(buffer->buffer);
  buffer->buffer = NULL;
  buffer->pos = buffer->size = 0;
}
