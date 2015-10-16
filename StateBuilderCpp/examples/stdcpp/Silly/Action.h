#ifndef ACTIONPRIVATE_H_
#define ACTIONPRIVATE_H_

typedef enum {
	BLUE,
	RED
} COLOR;

class Action 
{
public:
    void printEntryHigh();
    void printExitHigh();
	void printEntryLow();
    void printExitLow();
	void printAlreadyHigh();
	void printAlreadyLow();
	void changeColor(int color);
};

#endif
