#include "tax.h"
const char *tax_region(void) { return "UK VAT 20%"; }
double tax_for(double amount) { return amount * 0.20; }
