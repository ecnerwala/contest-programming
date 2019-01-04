// Copyright (c) 2014, Onufry Wojtaszczuk <onufryw@gmail.com>
#ifndef RECRUITING_DISTRIBUTED_API_ZEUS_H_
#define RECRUITING_DISTRIBUTED_API_ZEUS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define ZEUS(s) zeus_##s

// The basic contestant-available API for the Zeus distributed contest.

// The number of nodes on which the solution is running.
int ZEUS(NumberOfNodes)();

// The number (in the range [0 .. NumberOfNodes()-1]) of the node on which this
// process is running.
typedef int ZEUS(NodeId);
ZEUS(NodeId) ZEUS(MyNodeId());

// It is guaranteed that messages sent between two given nodes will arrive in
// order.
// No ordering guarantees are given between messages to different nodes (in
// particular, if A sends a message to B, then to C, and C sends a message to B
// after receiving the message from A, it is possible for B to receive C's
// message first).

// Send |bytes| bytes of |message| to node |target|. This is asynchronous (that
// is, it does not wait for the receiver to Receive the message).
// If |message| is shorter than |bytes|, behaviour is undefined.
// If |target| is not a valid node id (not in [0 .. NumberOfNodes()-1]), will
// crash.
void ZEUS(Send)(ZEUS(NodeId) target, const char *message, int bytes);

typedef struct {
  // Id of the sending node.
  ZEUS(NodeId) sender_id;
  // Length of the received message in bytes.
  int length;
} ZEUS(MessageInfo);

// Receive a message from the |source| node id, and copy the contents of the
// message to |buffer|, up to |buffer_size| bytes. No extra characters (in
// particular, a trailing '\0') will be appended to the message.
// A special case is |source| equal to -1, in which case a message from any
// other node can be received.
// This call is blocking - that is, the function will not return until a message
// is received.
// A std::pair will be returned.
// First element of the pair will be the id of the sending node.
// Second element of the pair will be the length of the received message
// in bytes.
//
// If the received message is larger than |buffer_size|, will crash.
// If the received message is smaller than |buffer_size|, the rest of the buffer
// contents are not modified.
// If |buffer| is smaller than |buffer_size|, behaviour is undefined.
// If |source| is neither -1 nor a valid node ID, will crash.
ZEUS(MessageInfo) ZEUS(Receive)(ZEUS(NodeId) source, char *buffer, int buffer_size);

// Returns the list of nodes from which we have unreceived messages (thus,
// calling Receive() with one of the returned node ids as the argument will
// not block). The order in which the node IDs are given is not specified. Each
// ID will be given once.
//std::vector<NodeId> Poll();  // NOTE: NOT IMPLEMENTED


#ifdef __cplusplus
}
#endif

#endif  // RECRUITING_DISTRIBUTED_API_ZEUS_H_
