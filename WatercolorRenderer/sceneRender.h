#pragma once

#include <stdio.h>
#include <vector>
#include <maya/MString.h>
#include <maya/MColor.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MRenderTargetManager.h>
#include <maya/MShaderManager.h>
#include <maya/MDrawContext.h>
#include <quadRender.h>

class sceneRender : public MSceneRender {

public:
    sceneRender(const MString name);
    ~sceneRender() override;
    void setShader();
    bool useShaderOverride;
    MShaderInstance* shaderOverride();
    MRenderTarget* const* targetOverrideList(unsigned int &listSize);
    MShaderInstance* mShader;
    MTextureAssignment* mTexture;
    std::vector<MRenderTarget*> mTargets;
    MClearOperation& clearOperation();
    std::vector<quadRender*> quads;
    MHWRender::MTexture *colorTarget;
    MHWRender::MTexture *depthTarget;
    void releaseTextures();
    MSceneFilterOption renderFilterOverride() override;

    MSceneFilterOption mSceneFilter;
};
