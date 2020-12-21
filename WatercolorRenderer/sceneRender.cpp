#include <stdio.h>
#include "sceneRender.h"

#include <maya/MString.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MRenderTargetManager.h>

#include <maya/MIOStream.h>
#include <maya/MStreamUtils.h>
#include <maya/MGlobal.h>


sceneRender::sceneRender(const MString name) : MSceneRender(name), colorTarget(NULL), depthTarget(NULL), mSceneFilter(kRenderShadedItems) {

}


sceneRender::~sceneRender() {

}

MShaderInstance* sceneRender::shaderOverride() {
   return mShader;

}

MRenderTarget* const* sceneRender::targetOverrideList(unsigned int &listSize)  {
    listSize = mTargets.size();
    if(listSize > 0) {
        return mTargets.data();
    } else {
        return NULL;
    }
}


MClearOperation& sceneRender::clearOperation()
{
    mClearOperation.setMask( (unsigned int) ( MClearOperation::kClearDepth | MClearOperation::kClearStencil ) );
    return mClearOperation;
}

void sceneRender::releaseTextures() {
    MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
    MHWRender::MTextureManager* textureManager = renderer->getTextureManager();
    textureManager->releaseTexture(colorTarget);
    textureManager->releaseTexture(depthTarget);
}

MSceneRender::MSceneFilterOption sceneRender::renderFilterOverride() {
    return mSceneFilter;
}
