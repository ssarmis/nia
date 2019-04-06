#include "nia_post_processing_pipeline.h"

NIA_CALL niaPostProcessingPipeline::niaPostProcessingPipeline(r32 width, r32 height, const niaScene& scene){
    niaPostProcessingPipeline(width, height, scene.getFrameBuffer());
}

NIA_CALL niaPostProcessingPipeline::niaPostProcessingPipeline(r32 width, r32 height, const niaFrameBuffer& frameBuffer):
inFrameBuffer(frameBuffer), outFrameBuffer(width, height, GL_COLOR_ATTACHMENT0){
    filterBuffer = niaArray<niaFilter*>();
}

NIA_CALL niaPostProcessingPipeline::~niaPostProcessingPipeline(){
    filterBuffer.clean();
}

NIA_CALL void niaPostProcessingPipeline::addFilter(niaFilter* filter){
    filterBuffer.add(filter);
}

NIA_CALL void niaPostProcessingPipeline::removeFilter(u32 index){
    filterBuffer.remove(index);
}

NIA_CALL void niaPostProcessingPipeline::processPipeline(niaRenderer* renderer){
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

NIA_CALL GLuint niaPostProcessingPipeline::getFrameBufferTextureId(){
    return finalFrameBuffer.getTextureId();
}

NIA_CALL void niaPostProcessingPipeline::flush(){
    // TODO here I will directly display the end line of the pipeline prob
}
