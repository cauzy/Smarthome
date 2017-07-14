#ifndef _Common_H_
#define _Common_H_

typedef struct __attribute__((__packed__))
{
	uint8_t	Address;
	uint8_t CycleCount;
	float Temperature;
} Message;

#endif /* _Common_H_ */
