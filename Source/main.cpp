#include "conio3.h"

int main() {
	Conio3* betterConio = new Conio3();
	settitle("imie nazwisko nr_indeksu");
	betterConio->doThis();
	delete betterConio;
	return 0;
}
