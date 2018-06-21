#include "utils.h"
#include "menu.h"
#include "gomoku.h"
#include "interface.h"
#include "defaultRules.h"

int main(int argc, char **argv) {
	(void)argc;
	(void)argv;
	srand(time(NULL));
	Interface		interface;
	Gomoku			gomoku(interface);
	gomoku.start();

	success("end of the game");
	return (0);
}
