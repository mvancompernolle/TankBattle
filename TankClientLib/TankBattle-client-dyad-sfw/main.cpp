#include <iostream>

#include "sfwdraw.h"

const char FWRD_KEY = 'w';
const char REVS_KEY = 's';
const char LEFT_KEY = 'a';
const char RIGT_KEY = 'd';

const char FIRE_KEY = 'e';

int main()
{
    sfw::initContext(400, 400, "Tank Client");
    sfw::setBackgroundColor(BLACK);

    while (sfw::stepContext())
    {
        
    }

    sfw::termContext();
}