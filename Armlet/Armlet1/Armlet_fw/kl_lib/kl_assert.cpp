#include "kl_assert.h"

#include "kl_lib.h"
#include "lcd1200.h"
#include "vibro.h"


void assertion_error(const char *filename, int line, const char *cond_string) {

	// only report first violation to simplify diagnostics
	// and avoid recursion
	if (assertion_raised)
		return;

	assertion_raised = true;
	Uart.Printf("assert(%s) violated in file '%s', line %d\r",
				cond_string, filename, line);
	Lcd.Cls();
	Lcd.Printf(0, 0, "assert(%s) violated in file '%s', line %d",
			   cond_string, filename, line);
	Vibro.Flinch(4);
}

bool assertion_raised = false;
