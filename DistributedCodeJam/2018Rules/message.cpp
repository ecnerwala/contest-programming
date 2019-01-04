// A program you submit to Distributed Code Jam will be compiled by Google, and
// will run on multiple computers (nodes). This library describes the interface
// needed for the nodes to identify themselves and to communicate.
//
// This is the version of the interface for programs written in C++. Your
// program should contain the line
// #include <message.h>
// before the code starts.
#ifndef MESSAGE_H_
#define MESSAGE_H_

// The number of nodes on which the solution is running.
int NumberOfNodes();

// The index (in the range [0 .. NumberOfNodes()-1]) of the node on which this
// process is running.
int MyNodeId();

// In all the functions below, if "target" or "source" is not in the valid
// range, the behaviour is undefined.

// The library internally has a message buffer for each of the nodes in
// [0 .. NumberOfNodes()-1]. It accumulates the message in such a buffer through
// the "Put" methods.

// Append "value" to the message that is being prepared for the node with id
// "target". The "Int" in PutInt is interpreted as 32 bits, regardless of
// whether the actual int type will be 32 or 64 bits.
void PutChar(int target, char value);
void PutInt(int target, int value);
void PutLL(int target, long long value);

// Send the message that was accumulated in the appropriate buffer to the
// "target" instance, and clear the buffer for this instance.
//
// This method is non-blocking - that is, it does not wait for the receiver to
// call "Receive", it returns immediately after sending the message.
void Send(int target);

// The library also has a receiving buffer for each instance. When you call
// "Receive" and retrieve a message from an instance, the buffer tied to this
// instance is overwritten. You can then retrieve individual parts of the
// message through the Get* methods. You must retrieve the contents of the
// message in the order in which they were appended.
//
// This method is blocking - if there is no message to receive, it will wait for
// the message to arrive.
//
// You can call Receive(-1) to retrieve a message from any source, or with
// source in [0 .. NumberOfNodes()-1] to retrieve a message from a particular
// source.
//
// It returns the number of the instance which sent the message (which is equal
// to source, unless source is -1).
int Receive(int source);

// Each of these methods returns and consumes one item from the buffer of the
// appropriate instance. You must call these methods in the order in which the
// elements were appended to the message (so, for instance, if the message was
// created with PutChar, PutChar, PutLL, you must call GetChar, GetChar, GetLL
// in this order).
// If you call them in different order, or you call a Get* method after
// consuming all the contents of the buffer, behaviour is undefined.
// The "Int" in GetInt is interpreted as 32 bits, regardless of whether the
// actual int type will be 32 or 64 bits.
char GetChar(int source);
int GetInt(int source);
long long GetLL(int source);

#endif  // MESSAGE_H_
