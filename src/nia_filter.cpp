/*
 * File: nia_filter.cpp
 * Project: Nia
 * File created on March-30-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "nia_filter.h"

// TODO rewrite the whole filter system, make it more data oriended

niaFilter::niaFilter(){
    quad = quad.quad(1.0);
}

niaFilter::niaFilter(const char* sourceVertex, const char* sourceFragment):
shader(sourceVertex, sourceFragment){
    quad = quad.quad(1.0);
}

niaFilter::~niaFilter(){
}

void niaFilter::renderFrameBuffer(niaRenderer* renderer, niaFrameBuffer& in){
    renderer->bindTexture(1, in.getDepthTextureId());
    shader.setUniform1i("depthTexture", 1);
    renderer->renderMeshRaw(quad, in.getTextureId());
}

/// greyscale
const char* greyScaleVertex = ""
NIA_GLSL_VERSION" \n""precision "
NIA_GLSL_PRECISION" float;\n"
"layout(location = 0) in vec3 pos;\n"
"layout(location = 3) in vec2 uv;\n"

"uniform mat4 mP;\n"
"uniform mat4 mT;\n"
"uniform mat4 mV;\n"

"out vec2 o_uv;\n"

"void main(){\n"
"   vec4 position = vec4(pos, 1.0);\n"
"   gl_Position = vec4(pos, 1.0);\n"
"   o_uv = uv;\n"
"}\n"
"";

const char* greyScaleFragment = ""
NIA_GLSL_VERSION" \n""precision "
NIA_GLSL_PRECISION" float;\n"

"out vec4 finalColor;\n"

"in  vec2 o_uv;\n"

"uniform sampler2D tex;\n"

"void main(){\n"
"   float avg = (texture(tex, o_uv).x + texture(tex, o_uv).y + texture(tex, o_uv).z) / 3;"
"   finalColor = vec4(avg, avg, avg, 1);\n"
"}\n";

niaFilterGreyScale::niaFilterGreyScale():
niaFilter(greyScaleVertex, greyScaleFragment){
}

niaFilterGreyScale::~niaFilterGreyScale(){
    printf("Destroyed\n");
}

void niaFilterGreyScale::process(niaRenderer* renderer, niaFrameBuffer& original, niaFrameBuffer& in, niaFrameBuffer& out){
    out.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.useShader();
    renderFrameBuffer(renderer, in);
    shader.unuseShader();
    out.unbind();
}
/// 


/// invert
const char* invertVertex = ""
NIA_GLSL_VERSION" \n""precision "
NIA_GLSL_PRECISION" float;\n"
"layout(location = 0) in vec3 pos;\n"
"layout(location = 3) in vec2 uv;\n"

"uniform mat4 mP;\n"
"uniform mat4 mT;\n"
"uniform mat4 mV;\n"

"out vec2 o_uv;\n"

"void main(){\n"
"   vec4 position = vec4(pos, 1.0);\n"
"   gl_Position = vec4(pos, 1.0);\n"
"   o_uv = uv;\n"
"}\n"
"";

const char* invertFragment = ""
NIA_GLSL_VERSION" \n""precision "
NIA_GLSL_PRECISION" float;\n"

"out vec4 finalColor;\n"

"in  vec2 o_uv;\n"

"uniform sampler2D tex;\n"

"void main(){\n"
"   float r = 1.0 - texture(tex, o_uv).r;\n"
"   float g = 1.0 - texture(tex, o_uv).g;\n"
"   float b = 1.0 - texture(tex, o_uv).b;\n"

"   finalColor = vec4(r, g, b, 1);\n"
"}\n";

niaFilterInvert::niaFilterInvert():
niaFilter(invertVertex, invertFragment){
}

niaFilterInvert::~niaFilterInvert(){
}

void niaFilterInvert::process(niaRenderer* renderer, niaFrameBuffer& original, niaFrameBuffer& in, niaFrameBuffer& out){
    out.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.useShader();
    renderFrameBuffer(renderer, in);
    shader.unuseShader();
    out.unbind();
}
///


/// box blur
const char* boxBlurVertex = ""
NIA_GLSL_VERSION" \n""precision "
NIA_GLSL_PRECISION" float;\n"
"layout(location = 0) in vec3 pos;\n"
"layout(location = 3) in vec2 uv;\n"

"uniform mat4 mP;\n"
"uniform mat4 mT;\n"
"uniform mat4 mV;\n"

"out vec2 o_uv;\n"

"void main(){\n"
"   vec4 position = vec4(pos, 1.0);\n"
"   gl_Position = vec4(pos, 1.0);\n"
"   o_uv = uv;\n"
"}\n"
"";

const char* boxBlurFragment = ""
NIA_GLSL_VERSION" \n""precision "
NIA_GLSL_PRECISION" float;\n"

"out vec4 finalColor;\n"

"in  vec2 o_uv;\n"

"uniform sampler2D tex;\n"
"uniform sampler2D depthTexture;\n"

"uniform float depthLimit;\n"

"void main(){\n"
"   vec4 dcolor = texture(depthTexture, o_uv);\n"
"   dcolor.r = pow(dcolor.r, 128.0);\n"
"   dcolor.g = pow(dcolor.g, 128.0);\n"
"   dcolor.b = pow(dcolor.b, 128.0);\n"
"   float avg = (dcolor.r + dcolor.g + dcolor.b) / 3.0;\n"
"   if(avg >= depthLimit){\n"
"      float pixelSize = 1.0 / 800.0;\n"
"      float radius = 5.0 * pixelSize;\n"
"      vec3 color;\n"
"      for(float y = -radius; y <= radius; y += pixelSize){\n"
"         for(float x = -radius; x <= radius; x += pixelSize){\n"
"             vec2 fxy = o_uv + vec2(x, y);\n"
"             color += texture(tex, fxy).xyz;\n"
"         }\n"
"       }\n"
"       color /= 100.0;\n"
"       finalColor = vec4(color, 1.0);\n"
"   } else {\n"
"       finalColor = texture(tex, o_uv);\n"
"   }\n"
"}\n";

niaFilterBoxBlur::niaFilterBoxBlur(){
}

niaFilterBoxBlur::niaFilterBoxBlur(r32 depthLimit):
niaFilter(boxBlurVertex, boxBlurFragment){
    this->depthLimit = depthLimit;
}

niaFilterBoxBlur::~niaFilterBoxBlur(){
}

void niaFilterBoxBlur::process(niaRenderer* renderer, niaFrameBuffer& original, niaFrameBuffer& in, niaFrameBuffer& out){
    out.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.useShader();
    shader.setUniform1f("depthLimit", depthLimit);
    renderFrameBuffer(renderer, in);
    shader.unuseShader();
    out.unbind();
}
///

/// brightness

const char* brightnessVertex = ""
NIA_GLSL_VERSION" \n""precision "
NIA_GLSL_PRECISION" float;\n"
"layout(location = 0) in vec3 pos;\n"
"layout(location = 3) in vec2 uv;\n"

"uniform mat4 mP;\n"
"uniform mat4 mT;\n"
"uniform mat4 mV;\n"

"out vec2 o_uv;\n"

"void main(){\n"
"   vec4 position = vec4(pos, 1.0);\n"
"   gl_Position = vec4(pos, 1.0);\n"
"   o_uv = uv;\n"
"}\n"
"";

const char* brightnessFragment = ""
NIA_GLSL_VERSION" \n""precision "
NIA_GLSL_PRECISION" float;\n"

"out vec4 finalColor;\n"

"in  vec2 o_uv;\n"

"uniform sampler2D tex;\n"

"void main(){\n"
"   vec3 color = texture(tex, o_uv).rgb;\n"
"   vec3 normalizedColor = color;\n"
"   float brightness = normalizedColor.r * 0.2126 + normalizedColor.y * 0.7152 + normalizedColor.z * 0.0722;\n"
"   if(brightness > 0.8){\n"
"       finalColor = texture(tex, o_uv) * 1.1;\n"
"   } else {\n"
"       finalColor = vec4(0.0, 0.0, 0.0, 0.0);\n"
"   }\n"
"}\n";


niaFilterBrightness::niaFilterBrightness():
niaFilter(brightnessVertex, brightnessFragment){
}
niaFilterBrightness::~niaFilterBrightness(){
}

void niaFilterBrightness::process(niaRenderer* renderer, niaFrameBuffer& original, niaFrameBuffer& in, niaFrameBuffer& out){
    out.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.useShader();
    renderFrameBuffer(renderer, in);
    shader.unuseShader();
    out.unbind();
}
///


niaFilterBloom::niaFilterBloom():
niaFilter(){
    blurFilter = niaFilterBoxBlur(0.0);
}

niaFilterBloom::~niaFilterBloom(){
}

void niaFilterBloom::process(niaRenderer* renderer, niaFrameBuffer& original, niaFrameBuffer& in, niaFrameBuffer& out){
    brightPassFilter.process(renderer, original, in, out);
    blurFilter.process(renderer, original, out, in);
   
    out.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    blendAddShader.useShader();
    renderer->bindTexture(0, in.getTextureId(), GL_TEXTURE_2D);
    renderer->bindTexture(1, original.getTextureId(), GL_TEXTURE_2D);

    blendAddShader.setUniform1i("tex", 0);
    blendAddShader.setUniform1i("tex2", 1);

    renderer->renderMeshRaw(quad);
    blendAddShader.unuseShader();

    out.unbind();
}

///



/// sepia

const char* sepiaVertex = ""
NIA_GLSL_VERSION" \n""precision "
NIA_GLSL_PRECISION" float;\n"
"layout(location = 0) in vec3 pos;\n"
"layout(location = 3) in vec2 uv;\n"

"uniform mat4 mP;\n"
"uniform mat4 mT;\n"
"uniform mat4 mV;\n"

"out vec2 o_uv;\n"

"void main(){\n"
"   vec4 position = vec4(pos, 1.0);\n"
"   gl_Position = vec4(pos, 1.0);\n"
"   o_uv = uv;\n"
"}\n"
"";

const char* sepiaFragment = ""
NIA_GLSL_VERSION" \n""precision "
NIA_GLSL_PRECISION" float;\n"

"out vec4 finalColor;\n"

"in vec2 o_uv;\n"

"uniform sampler2D tex;\n"

"void main(){\n"
"   vec4 color = texture(tex, o_uv);\n"
"   color.x = color.x * 0.393 + color.y * 0.769 + color.z * 0.189;\n"
"   color.y = color.x * 0.349 + color.y * 0.686 + color.z * 0.168;\n"
"   color.z = color.x * 0.272 + color.y * 0.534 + color.z * 0.131;\n"
"   finalColor = color;\n"
"}\n";

niaFilterSepia::niaFilterSepia():
niaFilter(sepiaVertex, sepiaFragment){
}

niaFilterSepia::~niaFilterSepia(){
}

void niaFilterSepia::process(niaRenderer* renderer, niaFrameBuffer& original, niaFrameBuffer& in, niaFrameBuffer& out){
    out.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.useShader();
    renderFrameBuffer(renderer, in);
    shader.unuseShader();
    out.unbind();
}
///
