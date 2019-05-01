/** @file nia_loader.h
  * @author Streanga Sarmis-Stefan
  * @brief
  * 
  */
#ifndef _NIA_LOADER_H_
#define _NIA_LOADER_H_

#include "nia_general.h"

/**
 *  @class niaWindow
 *  @author Streanga Sarmis-Stefan
 *  @brief 
 * 
 * Loader class used to load everything needed for Nia to work properly
 */
NIA_CLASS niaLoader {
public:
    /**
     * @brief Function for loading everything needed for Nia to work properly
     */
    NIA_CALL static void loadEverything();
};

#endif // _NIA_LOADER_H_