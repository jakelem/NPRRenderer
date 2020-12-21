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
class mayaSceneRender : public MSceneRender {

public:
    mayaSceneRender(const MString name, MSceneFilterOption filter);
    mayaSceneRender(const MString name);

    ~mayaSceneRender() override;
    void setShader();
    bool useShaderOverride;
   // MShaderInstance* shaderOverride();
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
    MCullingOption cullingOverride() override;
    MCullingOption mCullingOption;
};
