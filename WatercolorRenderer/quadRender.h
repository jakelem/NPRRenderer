#pragma once

#include <stdio.h>
#include <vector>
#include <maya/MString.h>
#include <maya/MColor.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MRenderTargetManager.h>
#include <maya/MShaderManager.h>
#include <maya/MGlobal.h>
class quadRender : public MHWRender::MQuadRender {
public:
    quadRender(const MString name);
    ~quadRender() override;
    const MShaderInstance* shader() override;
    MShaderInstance* mShader;
    MTextureAssignment* mTexture;
    MRenderTarget* const* targetOverrideList(unsigned int &listSize) override;
    MStatus setShaderColorDepth(MTexture* color, MTexture* depth);
    std::vector<MRenderTarget*> mTargets;
    MClearOperation& clearOperation();
    MStatus getOutputFromOp(MHWRender::MQuadRender* m);
    MStatus addInputTarget(MString* target);
};
