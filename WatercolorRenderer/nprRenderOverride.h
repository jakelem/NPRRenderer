#pragma once

#include <stdio.h>

#include <maya/MString.h>
#include <maya/MColor.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MRenderTargetManager.h>
#include <maya/MShaderManager.h>
#include <maya/MTextureManager.h>
#include <maya/MAnimControl.h>

#include "quadRender.h"
#include "sceneRender.h"
#include "mayaSceneRender.h"

#include "userRenderOperation.h"
#include "renderersettings.h"


class nprRenderOverride : public MHWRender::MRenderOverride {
public:
    nprRenderOverride(const MString& name);
    ~nprRenderOverride() override;
    MHWRender::DrawAPI supportedDrawAPIs() const override;

    MStatus setup(const MString& destination) override;
    MStatus cleanup() override;


    int numOps = 13;
    int currOp = 0;


    bool nextRenderOperation() override;
    MRenderOperation* renderOperation() override;
    MStatus setupShaders();

    sceneRender* vertexRender;
    mayaSceneRender* mayaRender;
    mayaSceneRender* mayaUIRender;
    MHUDRender* hudRender;

    quadRender* blurBleedQuad;
    quadRender* clearQuad;
    quadRender* bleedQuad;
    quadRender* stylizeQuad;
    quadRender* crossHatchQuad;
    quadRender* substrateQuad;
    quadRender* outlineQuad;
    quadRender* dilateQuad;

    MPresentTarget* present;
    bool finishedSetup = false;

    MRenderTarget* target1;
    MRenderTarget* target2;
    MRenderTarget* target3;
    MRenderTarget* target4;
    MRenderTarget* target5;
    MRenderTarget* target6;
    MRenderTarget* target7;

    const MRenderer* renderer;
    const MShaderManager* shaderManager;
    const MRenderTargetManager* targetManager;
    MTextureManager* textureManager;


    MHWRender::MTexture* paperTexture;
    MHWRender::MTexture* noiseTexture;
    MHWRender::MTexture* hatchTexture;

    MShaderInstance* objectShader;

    MShaderInstance* postProcess;
    MStatus passRendersToPostProcess();

    MShaderInstance* clearShader;
    MShaderInstance* bleedShader;
    MShaderInstance* stylizeShader;
    MShaderInstance* crossHatchShader;
    MShaderInstance* substrateShader;
    MShaderInstance* edgeShader;
    MShaderInstance* edgeDilateShader;

    MRenderTargetDescription* targetDesc1; //used for original color image
    MRenderTargetDescription* targetDesc2; //used for control image ping ponging
    MRenderTargetDescription* targetDesc3; //used for depth buffer, blurred image pass
    MRenderTargetDescription* targetDesc4; //used for control image ping ponging
    MRenderTargetDescription* targetDesc5; //used for first bleed pass
    MRenderTargetDescription* targetDesc6; //used for second bleed pass
    MRenderTargetDescription* targetDesc7; //used for second bleed pass

    //set up rendering operation pipeline
    rendererSettings settings;
    MStatus setTargetDescriptions();
    MStatus setTargetDimensions(MRenderTargetDescription* desc, float width, float height);
    MStatus setTargetDimensionsToScreen(MRenderTargetDescription* desc);
    MStatus setFixedColor(float r, float g, float b);
    MStatus updateShaderAttributes();



};
static nprRenderOverride* npr;


