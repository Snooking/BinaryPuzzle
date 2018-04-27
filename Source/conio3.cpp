//do obslugi plikow
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "conio3.h"

Conio3::Conio3() {
	zn = 0;
	x = XDRAW;
	y = YDRAW;
	attr = LIGHTGRAY;
	back = BLACK;
	whichRowThirdRule = 0;
	whichColumnThirdRule = 0;
	isGameStarted = false;
	simpleAdviceBool = false;
	howManyOfRuleTwo = false;
	simpleGameEndCheck = false;
	amICheckingForGameEnd = false;
	canIFinishGame = true;
	actualMenuY = MENUY;
	highlight = false;
	putHighlighted = false;
	noFile = false;
}

Conio3::~Conio3() {
	clearBoards();
}

void Conio3::drawMenu() {
	actualMenuY = MENUY;
	_setcursortype(_NOCURSOR);
	textbackground(BLACK);
	gotoxy(MENUX, actualMenuY++);
	cputs("Imie, nazwisko, nr indexu");
	gotoxy(MENUX, actualMenuY++);
	cputs("DONE: a,b,c,d,e,f,g,h,i,j,k,l,m,n");
	gotoxy(MENUX, actualMenuY++);
	cputs("Arrows = Walking");
	gotoxy(MENUX, actualMenuY++);
	cputs("ESC = Exit");
	gotoxy(MENUX, actualMenuY++);
	cputs("N = New Game");
	gotoxy(MENUX, actualMenuY++);
	cputs("01 = Type number");
	gotoxy(MENUX, actualMenuY++);
	cputs("O = Randomize board");
	gotoxy(MENUX, actualMenuY++);
	cputs("P - Simple advice");
	gotoxy(MENUX, actualMenuY++);
	cputs("R - Resize board");
	gotoxy(MENUX, actualMenuY++);
	cputs("K = Simple check for game ending");
	gotoxy(MENUX, actualMenuY++);
	cputs("D = Check second rule");
	gotoxy(MENUX, actualMenuY++);
	cputs("A = Automatic detection of game ending");
	gotoxy(MENUX, actualMenuY++);
	cputs("J = Highlight clear fields");
	gotoxy(MENUX, actualMenuY++);
	cputs("W = Fill highlighted fields");
	gotoxy(MENUX, actualMenuY++);
	cputs("S = Save game");
	gotoxy(MENUX, actualMenuY++);
	cputs("L = Load game");
	gotoxy(MENUX, actualMenuY++);
	cputs("x = ");
	cputs(itoa(XCOORD + 1));
	cputs(" y = ");
	cputs(itoa(YCOORD + 1));
	gotoxy(MENUX, actualMenuY++);
	if (amICheckingForGameEnd)cputs("Automatic game ending detection is on");
	else cputs("Automatic game ending detection is off");
	if (noFile) {
		gotoxy(MENUX, actualMenuY++);
		cputs("Couldnt open file");
		noFile = false;
	}
	whichColumnThirdRule = 0;
	whichRowThirdRule = 0;
	simpleAdvice();
	ruleTwoHowMany();
};

int Conio3::getSizeOfInt(int value) {
	int size = 0;
	if (value == 0) size = 1;
	while (value > 0) {
		size++;
		value /= 10;
	}
	return size;
}

char* Conio3::itoa(int value) {
	int i = getSizeOfInt(value);
	char* charValue = new char[i + 1];
	charValue[i] = '\0';
	if (value == 0)
		charValue[0] = '0';
	while (value > 0) {
		i--;
		charValue[i] = value % 10 + 48;
		value /= 10;
	}
	return charValue;
}

void Conio3::doThis() {
	size = SIZESTART;
	newGame(size);
	isGameStarted = true;
	do {
		textbackground(back);
		clrscr();
		textcolor(attr);
		drawBorder(size);
		drawMenu();
		fillBoard();
		gotoxy(x, y);
		textcolor(attr);
		if (starterBoard[XCOORD][YCOORD] == ' ')
			textbackground(GREEN);
		else textbackground(RED);
		if (board[XCOORD][YCOORD] != ' ' || !highlight)
			putch(board[XCOORD][YCOORD]);
		else putch(highlightClearFields());
		textbackground(back);
		automaticGameEndCheck();
		reactToSign();
		simpleCheckForGameEnding();
	} while (zn != ESC);

}

void Conio3::newGame(int size) {
	clearBoards();
	this->size = size;
	drawBorder(size);
	board = new char*[size];
	for (int i = 0; i < size; i++) {
		board[i] = new char[size];
		for (int j = 0; j < size; j++) {
			board[i][j] = ' ';
		}
	}
	starterBoard = new char*[size];
	for (int i = 0; i < size; i++) {
		starterBoard[i] = new char[size];
		for (int j = 0; j < size; j++) {
			starterBoard[i][j] = ' ';
		}
	}
	highlightedBoard = new char*[size];
	for (int i = 0; i < size; i++) {
		highlightedBoard[i] = new char[size];
		for (int j = 0; j < size; j++) {
			highlightedBoard[i][j] = ' ';
		}
	}
}

void Conio3::boardToBoard(char** from, char** to) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (from[i][j]!=' ')
			to[i][j] = from[i][j];
		}
	}
}


void Conio3::drawBorder(int size) {
	//Plus 2, cause we're drawing board for the game
	for (int i = 0; i < size + 2; i++) {
		for (int j = 0; j < size + 2; j++) {
			if (i == 0 || j == 0 || i == size + 1 || j == size + 1) {
				gotoxy(XDRAW - 1 + i, YDRAW - 1 + j);
				cputs("*");
			}
		}
	}
}

void Conio3::reactToSign() {
	zn = getch();
	howManyOfRuleTwo = false;
	simpleAdviceBool = false;
	simpleGameEndCheck = false;
	putHighlighted = false;
	if (zn != 'w' && zn != 'W')
		highlight = false;
	if (zn == 0) {
		zn = getch();		
		if (zn == 0x48 && y > YDRAW) y--;
		else if (zn == 0x50 && y < YDRAW + size - 1) y++;
		else if (zn == 0x4b && x > XDRAW) x--;
		else if (zn == 0x4d && x < XDRAW + size - 1) x++;
	}
	else if (zn == '0' && checkAllRules('0', board) && starterBoard[XCOORD][YCOORD] == ' ')
		board[XCOORD][YCOORD] = '0';
	else if (zn == '1' && checkAllRules('1', board) && starterBoard[XCOORD][YCOORD] == ' ')
		board[XCOORD][YCOORD] = '1';
	else if (zn == 'n' || zn == 'N') openFile("Base12.txt");
	else if (zn == 'o' || zn == 'O') randomizeBoard();
	else if (zn == 'p' || zn == 'P') simpleAdviceBool = true;
	else if (zn == 'k' || zn == 'K') simpleGameEndCheck = true;
	else if (zn == 'd' || zn == 'D') howManyOfRuleTwo = true;
	else if (zn == 'a' || zn == 'A') amICheckingForGameEnd == true ?
		amICheckingForGameEnd = false : amICheckingForGameEnd = true;
	else if (zn == 'j' || zn == 'J') highlight = true;
	else if ((zn == 'w' || zn == 'W') && highlight) {
		putHighlighted = true;
		fillBoard();
		putHighlighted = false;
		highlight = false;
		boardToBoard(highlightedBoard, board);
		fillBoard();
	}
	else if (zn == 'r' || zn == 'R') changeSize();
	else if (zn == 's' || zn == 'S') saveFile(getName());
	else if (zn == 'l' || zn == 'L') openFile(getName());
}

void Conio3::fillBoard() {
	int actualX = x;
	int actualY = y;
	for (int i = 0; i < size; i++) {
		x = XDRAW + i;
		for (int j = 0; j < size; j++) {
			y = YDRAW + j;
			gotoxy(x, y);
			if (starterBoard[i][j] != ' ')
				textattr(RED);
			else if (highlightClearFields() != ' ' && highlight) {
				textattr(LIGHTGREEN);
				putch(highlightClearFields());
				if (!putHighlighted)
					continue;
				else
					textattr(LIGHTGRAY);
			}
			else
				textattr(LIGHTGRAY);
			putch(board[i][j]);
		}
	}
	x = actualX;
	y = actualY;
}

bool Conio3::checkIfInBorder(int x, int y) {
	if (x < 0 || y < 0 || x >= size || y >= size) return false;
	return true;
}

bool Conio3::checkFirstRule(char sign, char**board) {

	if (checkIfInBorder(XCOORD - 1, YCOORD)) {
		if (board[XCOORD - 1][YCOORD] == sign) {
			if (checkIfInBorder(XCOORD - 2, YCOORD)) {
				if (board[XCOORD - 2][YCOORD] == sign) {
					return false;
				}
			}
			if (checkIfInBorder(XCOORD + 1, YCOORD)) {
				if (board[XCOORD + 1][YCOORD] == sign) {
					return false;
				}
			}
		}
	}

	if (checkIfInBorder(XCOORD + 1, YCOORD)) {
		if (board[XCOORD + 1][YCOORD] == sign) {
			if (checkIfInBorder(XCOORD + 2, YCOORD)) {
				if (board[XCOORD + 2][YCOORD] == sign) {
					return false;
				}
			}
		}
	}

	if (checkIfInBorder(XCOORD, YCOORD - 1)) {
		if (board[XCOORD][YCOORD - 1] == sign) {
			if (checkIfInBorder(XCOORD, YCOORD - 2)) {
				if (board[XCOORD][YCOORD - 2] == sign) {
					return false;
				}
			}
			if (checkIfInBorder(XCOORD, YCOORD + 1)) {
				if (board[XCOORD][YCOORD + 1] == sign) {
					return false;
				}
			}
		}
	}

	if (checkIfInBorder(XCOORD, YCOORD + 1)) {
		if (board[XCOORD][YCOORD + 1] == sign) {
			if (checkIfInBorder(XCOORD, YCOORD + 2)) {
				if (board[XCOORD][YCOORD + 2] == sign) {
					return false;
				}
			}
		}
	}
	return true;
}

bool Conio3::checkSecondRuleHorizontally(char sign, char** board) {
	int count = 0;
	for (int i = 0; i < size; i++) {
		count = board[i][y - YDRAW] == sign ? count + 1 : count;
	}
	if (count > size / 2 - 1)
		return false;
	return true;
}

bool Conio3::checkSecondRuleVertically(char sign, char** board) {
	int count = 0;
	for (int i = 0; i < size; i++) {
		count = (board[x - XDRAW][i] == sign) ? count + 1 : count;
	}
	if (count > size / 2 - 1)
		return false;
	return true;
}

bool Conio3::checkSecondRule(char sign, char** board)
{
	return (checkSecondRuleHorizontally(sign, board) && checkSecondRuleVertically(sign, board));
}

bool Conio3::checkThirdRuleVertically(char sign, char** board) {
	int thisNumber = 0;
	int theOtherNumber = 0;
	for (int i = 0; i < size; i++) {
		if (board[XCOORD][i] == ' ' && YCOORD != i) return true;
		if (i == YCOORD) thisNumber += sign;
		else thisNumber += board[XCOORD][i];
		thisNumber *= 10;
	}
	int j;
	for (int i = 0; i < size; i++) {
		if (i == XCOORD) continue;
		for (j = 0; j < size; j++) {
			if (board[i][j] == ' ') break;
			theOtherNumber += board[i][j];
			theOtherNumber *= 10;
		}
		if (theOtherNumber == thisNumber) {
			whichColumnThirdRule = i + 1;
			return false;
		}
		theOtherNumber = 0;
	}
	return true;
}

bool Conio3::checkThirdRuleHorizontally(char sign, char** board) {
	int thisNumber = 0;
	int theOtherNumber = 0;
	for (int i = 0; i < size; i++) {
		if (board[i][YCOORD] == ' ' && XCOORD != i) return true;
		if (i == XCOORD) thisNumber += sign;
		else thisNumber += board[i][YCOORD];
		thisNumber *= 10;
	}
	int j;
	for (int i = 0; i < size; i++) {
		if (i == YCOORD) continue;
		for (j = 0; j < size; j++) {
			if (board[j][i] == ' ') break;
			theOtherNumber += board[j][i];
			theOtherNumber *= 10;
		}
		if (theOtherNumber == thisNumber) {
			whichRowThirdRule = i + 1;
			return false;
		}
		theOtherNumber = 0;
	}
	return true;
}

bool Conio3::checkThirdRule(char sign, char** board) {
	return (checkThirdRuleHorizontally(sign, board) && checkThirdRuleVertically(sign, board));
}

bool Conio3::checkAllRules(char sign, char** board) {
	if (checkFirstRule(sign, board) && checkSecondRule(sign, board) &&
		checkThirdRule(sign, board))
		return true;
	return false;
}

bool Conio3::openFile(char* name) {
	clrscr();
	FILE* file = fopen(name, "r");
	if (file == NULL) {
		noFile = true;
		return false;
	}
	int newSize;
	fscanf(file, "%d\n", &newSize);
	newGame(newSize);
	amICheckingForGameEnd = fgetc(file);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			int sign = fgetc(file);
			if (sign == '1' || sign == '0')
				starterBoard[j][i] = sign;
			else starterBoard[j][i] = ' ';
		}
		fgetc(file);
	}
	fgetc(file);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			int sign = fgetc(file);
			if (sign == '1' || sign == '0')
				board[j][i] = sign;
			else board[j][i] = ' ';
		}
		fgetc(file);
	}
	fclose(file);
	return true;
}

void Conio3::randomizeBoard() {
	int howManyNumbers, howManyTries;
	srand(time(0));
	howManyNumbers = rand() % RANDCOUNT;
	howManyTries = RANDTRIES;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			board[i][j] = ' ';
			starterBoard[i][j] = ' ';
			highlightedBoard[i][j] = ' ';
		}
	}

	int randX, randY;
	char zeroOne;

	for (int i = 0; i < howManyNumbers; i++) {
		if (howManyTries <= 0) break;
		randX = rand() % size;
		randY = rand() % size;
		zeroOne = rand() % 2 + 48;
		x = randX + XDRAW;
		y = randY + YDRAW;
		if (checkAllRules(zeroOne, starterBoard))
			starterBoard[randX][randY] = zeroOne;
		else i--;
		howManyTries--;
	}
	boardToBoard(starterBoard, board);
	x = XDRAW;
	y = YDRAW;
}

void Conio3::simpleAdvice(char sign) {
	if (!simpleAdviceBool) return;
	if (starterBoard[XCOORD][YCOORD] != ' ') {
		gotoxy(MENUX, actualMenuY++);
		cputs("Unmodifiable field");
		return;
	}
	else if (checkAllRules(sign, board)) {
		gotoxy(MENUX, actualMenuY++);
		putch(sign);
		cputs(" can be used here");
	}
	else {
		if (!checkFirstRule(sign, board)) {
			gotoxy(MENUX, actualMenuY++);
			putch(sign);
			cputs(" - I rule");
		}
		if (!checkSecondRuleHorizontally(sign, board)) {
			gotoxy(MENUX, actualMenuY++);
			putch(sign);
			cputs(" - II rule, horizontally");
		}
		if (!checkSecondRuleVertically(sign, board)) {
			gotoxy(MENUX, actualMenuY++);
			putch(sign);
			cputs(" - II rule, vertically");
		}
		if (!checkThirdRule(sign, board)) {
			gotoxy(MENUX, actualMenuY++);
			putch(sign);
			cputs(" - III rule");
			if (whichColumnThirdRule != 0) {
				cputs(" - column ");
				cputs(itoa(whichColumnThirdRule));
			}
			if (whichRowThirdRule != 0) {
				cputs(" - row ");
				cputs(itoa(whichRowThirdRule));
			}
		}
	}
}

void Conio3::simpleAdvice() {
	simpleAdvice('0');
	simpleAdvice('1');
}

void Conio3::simpleCheckForGameEnding() {
	bool gameChanged = false;
	if (simpleGameEndCheck) {
		int actualX = x;
		int actualY = y;
		for (int i = 0; i < size; i++) {
			y = i + YDRAW;
			for (int j = 0; j < size; j++) {
				x = j + XDRAW;
				if (!checkAllRules('0', board) && !checkAllRules('1', board) && board[j][i] == ' ') {
					board[j][i] = '*';
					canIFinishGame = false;
				}
			}
		}
		x = actualX;
		y = actualY;
	}
	else {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (board[j][i] == '*') {
					board[j][i] = ' ';
					gameChanged = true;
				}
				else continue;
			}
		}
	}
	if (gameChanged) canIFinishGame = true;
}

void Conio3::ruleTwoHowMany() {
	if (!howManyOfRuleTwo)
		return;
	int zeros;
	int ones;
	for (int i = 0; i < size; i++) {
		zeros = 0;
		ones = 0;
		for (int j = 0; j < size; j++) {
			if (board[j][i] == '0')
				zeros++;
			if (board[j][i] == '1')
				ones++;
		}
		gotoxy(XDRAW + size + 2, YDRAW + i);
		if (zeros < size / 2 + 1) textattr(CYAN);
		else textattr(RED);
		cputs(itoa(zeros));
		cputs(" ");
		if (ones < size / 2 + 1) textattr(BLUE);
		else textattr(RED);
		cputs(itoa(ones));
		textattr(BLACK);
	}

	for (int i = 0; i < size; i++) {
		zeros = 0;
		ones = 0;
		for (int j = 0; j < size; j++) {
			if (board[i][j] == '0')
				zeros++;
			if (board[i][j] == '1')
				ones++;
		}
		gotoxy(XDRAW + i, YDRAW + size + 1);
		if (zeros < size / 2 + 1) textattr(CYAN);
		else textattr(RED);
		cputs(itoa(zeros));
		gotoxy(XDRAW + i, YDRAW + size + 2);
		if (ones < size / 2 + 1) textattr(BLUE);
		else textattr(RED);
		cputs(itoa(ones));
		textattr(BLACK);
	}
}
void Conio3::automaticGameEndCheck() {
	if (!amICheckingForGameEnd) {
		return;
	}
	simpleGameEndCheck = true;
	simpleCheckForGameEnding();
	if (!canIFinishGame) {
		gotoxy(MENUX, actualMenuY);
		actualMenuY++;
		textattr(RED);
		cputs("This game cant be finished");
		textattr(WHITE);
	}
	simpleGameEndCheck = false;
	simpleCheckForGameEnding();
}

char Conio3::highlightClearFields() {
	if (checkAllRules('0', board) && !checkAllRules('1', board) && board[XCOORD][YCOORD] == ' ') {
		if (putHighlighted)
			highlightedBoard[XCOORD][YCOORD] = '0';
		return '0';
	}
	else if (checkAllRules('1', board) && !checkAllRules('0', board) && board[XCOORD][YCOORD] == ' ') {
		if (putHighlighted)
			highlightedBoard[XCOORD][YCOORD] = '1';
		return '1';
	}
	else return ' ';
}


void Conio3::changeSize() {
	gotoxy(MENUX, actualMenuY);
	actualMenuY++;
	cputs("Wprowadz rozmiar w przedziale <2, 26>");
	gotoxy(MENUX, actualMenuY);
	actualMenuY++;
	int size = 0, zn = 0;
	bool isSize = false;
	do {
		zn = getche();
		if (zn >= '0' && zn <= '9') {
			size *= 10;
			size += zn - 48;
			isSize = true;
		}
		else if (!isSize) {
			randomizeBoard();
			return;
		}
	} while (zn != 13);
	int sizeOfSize = getSizeOfInt(size);
	char* name = new char[sizeOfSize + 9];
	int i = 0;
	int j = 0;
	name[i++] = 'b';
	name[i++] = 'a';
	name[i++] = 's';
	name[i++] = 'e';
	char* sizeChar = itoa(size);
	while (j < sizeof(sizeChar) - 2)
		name[i++] = sizeChar[j++];
	name[i++] = '.';
	name[i++] = 't';
	name[i++] = 'x';
	name[i++] = 't';
	name[i++] = '\0';
	if (!openFile(name)) {
		if (size > MAXSIZE)
			newGame(MAXSIZE);
		else if (size < MINSIZE)
			newGame(MINSIZE);
		else newGame(size);
	}
}

void Conio3::clearBoards() {
	if (!isGameStarted) return;
	for (int i = 0; i < size; i++) {
		delete[] board[i];
	}
	delete[] board;
	for (int i = 0; i < size; i++) {
		delete[] starterBoard[i];
	}
	delete[] starterBoard;
	for (int i = 0; i < size; i++) {
		delete[] highlightedBoard[i];
	}
	delete[] highlightedBoard;
	x = XDRAW;
	y = YDRAW;
}

char* Conio3::getName() {
	gotoxy(MENUX, actualMenuY++);
	cputs("Podaj nazwe pliku:");
	gotoxy(MENUX, actualMenuY++);
	char*name = new char[NAMESIZE];
	int i = 0;
	while (zn != 13 && i < NAMESIZE - 5) {
		zn = getche();
		if (zn != 13)
			name[i++] = zn;
	}
	name[i++] = '.';
	name[i++] = 't';
	name[i++] = 'x';
	name[i++] = 't';
	name[i] = '\0';
	return name;
}

void Conio3::saveFile(char* name) {
	FILE* file = fopen(name, "w");
	if (file == NULL) {
		noFile = true;
		return;
	}
	fprintf(file, "%d\n", size);
	
	fputc(amICheckingForGameEnd, file);

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++) {
			if (starterBoard[j][i] == '1' || starterBoard[j][i] == '0')
				fputc(starterBoard[j][i], file);
			else fputc('.', file);
		}
		fputc('\n', file);
	}
	fputc('\n', file);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++) {
			if (board[j][i] == '1' || board[j][i] == '0')
				fputc(board[j][i], file);
			else fputc('.', file);
		}
		fputc('\n', file);
	}
	fclose(file);
	settitle(name);
}



