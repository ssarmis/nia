/*
 * File: nia_loader.h
 * Project: Nia
 * File created on October-17-2018
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

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
    static void NIA_CALL loadEverything();
};

#endif // _NIA_LOADER_H_