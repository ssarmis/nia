#include "nia_filter.h"

NIA_CALL niaFilter::niaFilter(){
    quad = niaMesh::quad(1.0);
}

NIA_CALL niaFilter::niaFilter(const char* sourceVertex, const char* sourceFragment):
shader(sourceVertex, sourceFragment){
}

NIA_CALL niaFilter::~niaFilter(){
}

NIA_CALL void niaFilter::renderFrameBuffer(niaRenderer* renderer, niaFrameBuffer& in){
    renderer->renderMeshRaw(niaMesh::quad(1.0), in.getTextureId());
}

/// greyscale
const char* greyScaleVertex = ""
NIA_GLSL_VERSION" \n""precision "NIA_GLSL_PRECISION" float;\n"
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
NIA_GLSL_VERSION" \n""precision "NIA_GLSL_PRECISION" float;\n"

"out vec4 finalColor;\n"

"in  vec2 o_uv;\n"

"uniform sampler2D tex;\n"

"void main(){\n"
"   float avg = (texture(tex, o_uv).x + texture(tex, o_uv).y + texture(tex, o_uv).z) / 3;"
"   finalColor = vec4(avg, avg, avg, 1);\n"
"}\n";

NIA_CALL niaFilterGreyScale::niaFilterGreyScale():
niaFilter(greyScaleVertex, greyScaleFragment){
}

NIA_CALL niaFilterGreyScale::~niaFilterGreyScale(){
    printf("Destroyed\n");
}

NIA_CALL void niaFilterGreyScale::process(niaRenderer* renderer, niaFrameBuffer& in, niaFrameBuffer& out){
    out.bind();
    shader.useShader();
    renderFrameBuffer(renderer, in);
    shader.unuseShader();
    out.unbind();
}
/// 




/// invert
const char* invertVertex = ""
NIA_GLSL_VERSION" \n""precision "NIA_GLSL_PRECISION" float;\n"
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
NIA_GLSL_VERSION" \n""precision "NIA_GLSL_PRECISION" float;\n"

"out vec4 finalColor;\n"

"in  vec2 o_uv;\n"

"uniform sampler2D tex;\n"

"void main(){\n"
"   float r = 1.0 - texture(tex, o_uv).r;\n"
"   float g = 1.0 - texture(tex, o_uv).g;\n"
"   float b = 1.0 - texture(tex, o_uv).b;\n"

"   finalColor = vec4(r, g, b, 1);\n"
"}\n";

NIA_CALL niaFilterInvert::niaFilterInvert():
niaFilter(invertVertex, invertFragment){
}

NIA_CALL niaFilterInvert::~niaFilterInvert(){
}

NIA_CALL void niaFilterInvert::process(niaRenderer* renderer, niaFrameBuffer& in, niaFrameBuffer& out){
    out.bind();
    shader.useShader();
    renderFrameBuffer(renderer, in);
    shader.unuseShader();
    out.unbind();
}
///


/// box blur
const char* boxBlurVertex = ""
NIA_GLSL_VERSION" \n""precision "NIA_GLSL_PRECISION" float;\n"
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
NIA_GLSL_VERSION" \n""precision "NIA_GLSL_PRECISION" float;\n"

"out vec4 finalColor;\n"

"in  vec2 o_uv;\n"

"uniform sampler2D tex;\n"

"void main(){\n"
"   float pixelSize = 1.0 / 800.0;\n"
"   float radius = 5.0 * pixelSize;\n"
"   vec3 color;\n"
"   for(float y = -radius; y <= radius; y += pixelSize){\n"
"       for(float x = -radius; x <= radius; x += pixelSize){\n"
"           vec2 fxy = o_uv + vec2(x, y);\n"
"           color += texture(tex, fxy).xyz;\n"
"       }\n"
"   }\n"
"   color /= 100.0;\n"
"   finalColor = vec4(color, 1.0);\n"
"}\n";

NIA_CALL niaFilterBoxBlur::niaFilterBoxBlur():
niaFilter(boxBlurVertex, boxBlurFragment){
}

NIA_CALL niaFilterBoxBlur::~niaFilterBoxBlur(){
}

NIA_CALL void niaFilterBoxBlur::process(niaRenderer* renderer, niaFrameBuffer& in, niaFrameBuffer& out){
    out.bind();
    shader.useShader();
    renderFrameBuffer(renderer, in);
    shader.unuseShader();
    out.unbind();
}
///