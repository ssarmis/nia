#ifndef _NIA_POST_PROCESSING_PIPELINE_H_
#define _NIA_POST_PROCESSING_PIPELINE_H_

#include "nia_gl.h"
#include "nia_scene.h"
#include "nia_array.h"
#include "nia_general.h"
#include "nia_renderer.h"
#include "nia_frame_buffer.h"
#include "shaders/nia_shader_filter.h"

NIA_CLASS niaPostProcessingPipeline {
private:
    niaFrameBuffer inFrameBuffer;
    niaFrameBuffer outFrameBuffer;
    niaFrameBuffer finalFrameBuffer;

    niaArray<niaFilter*> filterBuffer;

public:
    NIA_CALL niaPostProcessingPipeline(r32 width, r32 height, const niaFrameBuffer& frameBuffer);
    NIA_CALL niaPostProcessingPipeline(r32 width, r32 height, const niaScene& scene);
    NIA_CALL ~niaPostProcessingPipeline();

    void NIA_CALL addFilter(niaFilter* filter);
    void NIA_CALL removeFilter(u32 index);

    void NIA_CALL processPipeline(niaRenderer* renderer);

    GLuint NIA_CALL getFrameBufferTextureId();

    void NIA_CALL flush();
};

#endif //_NIA_POST_PROCESSING_PIPELINE_H_