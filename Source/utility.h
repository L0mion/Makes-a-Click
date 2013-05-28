#ifndef UTILITY_H
#define UTILITY_H

#define SAFE_RELEASE(com) if(com != nullptr){ com->Release(); com = nullptr;}
#define SAFE_DELETE(obj) if(obj != nullptr){delete obj; obj = nullptr;}

#ifndef NULL
#define NULL 0
#endif //NULL

static const unsigned int SCREEN_WIDTH	= 1280;
static const unsigned int SCREEN_HEIGHT = 720;

#endif //UTILITY_H