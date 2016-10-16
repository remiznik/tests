#ifndef _DELETEMACROS_H
#define _DELETEMACROS_H

#define SafeRelease(x) { if(x) { x->Release(); x = nullptr; } }
#define SafeDelete(x) { if(x) { delete x; x = nullptr; } }

#endif


