#ifndef CONIO3
#define CONIO3

#include "conio2.h"

#define XCOORD x-XDRAW
#define YCOORD y-YDRAW

//it is better this way than using const (so if we have bigger board we use more numbers)
#define RANDCOUNT size*size/2+10
#define RANDTRIES RANDCOUNT + size

#define NAMESIZE 32

#define ESC 0x1b

class Conio3 {
private:
	const int MENUX = 1;
	const int MENUY = 1;
	const int XDRAW = 50;
	const int YDRAW = 2;
	const int SIZESTART = 8;
	const int MAXSIZE = 26;
	const int MINSIZE = 2;
	int size;
	int actualMenuY;
	int zn;
	int x;
	int y;
	int attr;
	int back;
	int whichRowThirdRule;
	int whichColumnThirdRule;

	bool isGameStarted;
	bool simpleAdviceBool;
	bool howManyOfRuleTwo;
	bool simpleGameEndCheck;
	bool amICheckingForGameEnd;
	bool canIFinishGame;
	bool highlight;
	bool putHighlighted;
	bool noFile;

	char** board;
	char** starterBoard;
	char** highlightedBoard;


	void drawMenu();
	int getSizeOfInt(int);
	char* itoa(int);
	void newGame(int);
	void boardToBoard(char**, char**);

	void drawBorder(int);

	void reactToSign();
	void fillBoard();
	bool checkIfInBorder(int, int);

	bool checkFirstRule(char, char**);
	bool checkSecondRuleVertically(char, char**);
	bool checkSecondRuleHorizontally(char, char**);
	bool checkSecondRule(char, char**);
	bool checkThirdRule(char, char**);
	bool checkThirdRuleVertically(char, char**);
	bool checkThirdRuleHorizontally(char, char**);
	bool checkAllRules(char, char**);

	bool openFile(char*);

	void randomizeBoard();
	void simpleAdvice(char);
	void simpleAdvice();
	
	void simpleCheckForGameEnding();
	void ruleTwoHowMany();
	void automaticGameEndCheck();
	char highlightClearFields();
	void changeSize();
	void clearBoards();
	char* getName();
	void saveFile(char*);
public:
	Conio3();
	~Conio3();
	void doThis();
};

#endif 
