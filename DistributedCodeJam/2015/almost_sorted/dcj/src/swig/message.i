%module message

%{
extern int NumberOfNodes();
extern int MyNodeId();
extern void PutChar(int target, char value);
extern void PutInt(int target, int value);
extern void PutLL(int target, long long value);
extern void Send(int target);
extern int Receive(int source);
extern char GetChar(int source);
extern int GetInt(int source);
extern long long GetLL(int source);
%}

extern int NumberOfNodes();
extern int MyNodeId();
extern void PutChar(int target, char value);
extern void PutInt(int target, int value);
extern void PutLL(int target, long long value);
extern void Send(int target);
extern int Receive(int source);
extern char GetChar(int source);
extern int GetInt(int source);
extern long long GetLL(int source);
