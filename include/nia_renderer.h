#ifndef _NIA_RENDERER_H_
#define _NIA_RENDERER_H_

#include <windows.h>

#include "nia_window.h"
#include "nia_general.h"

NIA_CLASS niaRenderer{
public:
    NIA_CALL niaRenderer();
    NIA_CALL ~niaRenderer();

    NIA_CALL void swapBuffers(const niaWindow& window);
};

#endif //_NIA_RENDERER_H_