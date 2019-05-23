#include "nia_post_processing_pipeline.h"

niaPostProcessingPipeline::niaPostProcessingPipeline(r32 width, r32 height, const niaScene& scene){
    niaPostProcessingPipeline(width, height, scene.getFrameBuffer());
}

niaPostProcessingPipeline::niaPostProcessingPipeline(r32 width, r32 height, const niaFrameBuffer& frameBuffer):
inFrameBuffer(frameBuffer), outFrameBuffer(width, height, GL_COLOR_ATTACHMENT0){
    filterBuffer = niaArray<niaFilter*>();
}

niaPostProcessingPipeline::~niaPostProcessingPipeline(){
    filterBuffer.clean();
}

void niaPostProcessingPipeline::addFilter(niaFilter* filter){
    filterBuffer.add(filter);
}

void niaPostProcessingPipeline::removeFilter(u32 index){
    filterBuffer.remove(index);
}

void niaPostProcessingPipeline::processPipeline(niaRenderer* renderer){
    niaFrameBuffer* fin = &inFrameBuffer;
    niaFrameBuffer* fout = &outFrameBuffer;
    niaFrameBuffer* aux;
    for(u32 filterIndex = 0; filterIndex < filterBuffer.getSize(); ++filterIndex){
        if(!filterBuffer[filterIndex]){
            continue;
        }
        filterBuffer[filterIndex]->process(renderer, *fin, *fout);
        aux = fin;
        fin = fout;
        fout = aux;
    }
    
    finalFrameBuffer = *fin;
}

GLuint niaPostProcessingPipeline::getFrameBufferTextureId(){
    return finalFrameBuffer.getTextureId();
}

void niaPostProcessingPipeline::flush(){
    // TODO here I will directly display the end line of the pipeline prob
}
