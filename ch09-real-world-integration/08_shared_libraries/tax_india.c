#include "tax.h"
const char *tax_region(void) { return "India GST 18%"; }
double tax_for(double amount) { return amount * 0.18; }
