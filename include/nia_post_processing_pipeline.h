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

    NIA_CALL void addFilter(niaFilter* filter);
    NIA_CALL void removeFilter(u32 index);

    NIA_CALL void processPipeline(niaRenderer* renderer);

    NIA_CALL GLuint getFrameBufferTextureId();

    NIA_CALL void flush();
};

#endif //_NIA_POST_PROCESSING_PIPELINE_H_