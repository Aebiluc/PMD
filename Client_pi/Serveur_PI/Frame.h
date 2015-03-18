#ifndef Frame_H
#define Frame_H

typedef struct {
	
	char Type;
	__int16 PayloadSize;
	void* Payload;

} Frame_t;

typedef enum {Msg} Msg_t;

#include <iostream>

class Frame
{
public:
	Frame();
	void* Send();
	int SendSize();
	void Receve(char &type, char *buffer);
	~Frame();

private:

	Frame_t* Message;

};

#endif

