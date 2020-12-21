#include <stdio.h>
#include "mayaSceneRender.h"

#include <maya/MString.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MRenderTargetManager.h>

#include <maya/MIOStream.h>
#include <maya/MStreamUtils.h>
#include <maya/MGlobal.h>


mayaSceneRender::mayaSceneRender(const MString name, MSceneFilterOption filter) : MSceneRender(name), colorTarget(NULL), depthTarget(NULL), mSceneFilter(filter) {

}

mayaSceneRender::mayaSceneRender(const MString name) : MSceneRender(name), colorTarget(NULL), depthTarget(NULL), mSceneFilter(kRenderAllItems), mCullingOption(kNoCullingOverride) {

}

mayaSceneRender::~mayaSceneRender() {

}

MRenderTarget* const* mayaSceneRender::targetOverrideList(unsigned int &listSize)  {
    listSize = mTargets.size();
    if(listSize > 0) {
        return mTargets.data();
    } else {
        return NULL;
    }
}


MClearOperation& mayaSceneRender::clearOperation()
{
    mClearOperation.setMask( (unsigned int) ( MClearOperation::kClearDepth | MClearOperation::kClearStencil ) );
    return mClearOperation;
}

void mayaSceneRender::releaseTextures() {
    MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
    MHWRender::MTextureManager* textureManager = renderer->getTextureManager();
    textureManager->releaseTexture(colorTarget);
    textureManager->releaseTexture(depthTarget);


}

MSceneRender::MSceneFilterOption mayaSceneRender::renderFilterOverride() {
    return mSceneFilter;
}

MSceneRender::MCullingOption mayaSceneRender::cullingOverride() {
    return mCullingOption;
}
