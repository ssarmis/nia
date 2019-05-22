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
    niaPostProcessingPipeline(r32 width, r32 height, const niaFrameBuffer& frameBuffer);
    niaPostProcessingPipeline(r32 width, r32 height, const niaScene& scene);
    ~niaPostProcessingPipeline();

    void addFilter(niaFilter* filter);
    void removeFilter(u32 index);

    void processPipeline(niaRenderer* renderer);

    GLuint getFrameBufferTextureId();

    void flush();
};

#endif //_NIA_POST_PROCESSING_PIPELINE_H_