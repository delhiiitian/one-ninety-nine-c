#ifndef PASSWORD_H
#define PASSWORD_H

typedef enum { WEAK, MEDIUM, STRONG } Strength;

Strength    password_strength(const char *pw);
const char *strength_name(Strength s);

#endif
