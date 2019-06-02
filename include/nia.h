/*
 * File: nia.h
 * Project: Nia
 * File created on October-08-2018
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef _NIA_H_
#define _NIA_H_

// THESE ARE NOTES FOR THE FUTURE ME, IF ANYTHING WILL BE DONE ABOUT THEM IS ANOTHER QUESTION

// TODO get rid of these directives they will be defined at
//      compilating externally not hard coded here, but since 
//      we only have opengl as a renderer for the moment, this will do it

// TODO this API was built with only opengl in mind, certain parts of it may not
//      fit a different rendering API like Vulkan or DirectX, although I will be able
//      to integrate these, at the moment the library is actually very tight to the
//      opengl API, things to change in order to make this more usable for other APIs will be
//      the majority of the things that are bound to opengl are the following:
//          - rectangles(they have a vertex array object if without even knowing if we need one or not)
//          - windows(we require a opengl context, without checking if we actually need opengl or something else)
//          - meshes(all the meshes are stored using opengl without any checks if we use opengl or not)
//          - live texture loading(we buffer the new images using another opengl context linked to our window, we also buffer the new texture using opengl calls)

#ifndef NIA_RENDERER_GL
#define NIA_RENDERER_GL
#endif

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#include "nia_constants.h"

#include "nia_mesh.h"
#include "nia_event.h"
#include "nia_scene.h"
#include "nia_vector.h"
#include "nia_matrix.h"
#include "shaders/nia_shader.h"
#include "shaders/nia_shader_cubemap.h"
#include "nia_loader.h"
#include "nia_window.h"
#include "nia_general.h"
#include "nia_renderer.h"
#include "nia_transform.h"
#include "nia_post_processing_pipeline.h"
#include "nia_texture.h"
#include "nia_sprite.h"

#endif //_NIA_H_