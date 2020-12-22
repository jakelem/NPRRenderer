#include <stdio.h>
#include "nprRenderOverride.h"

#include <maya/MString.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MRenderTargetManager.h>
#include <maya/MTextureManager.h>

#include <maya/MIOStream.h>
#include <maya/MStreamUtils.h>
#include <maya/MGlobal.h>

nprRenderOverride::nprRenderOverride(const MString& name) : MRenderOverride(name), targetDesc1(NULL), targetDesc2(NULL), targetDesc3(NULL),
    targetDesc6(NULL), clearShader(NULL), postProcess(NULL), bleedShader(NULL), stylizeShader(NULL), paperTexture(NULL), noiseTexture(NULL) {
    vertexRender = new sceneRender("vertexRender");
    mayaRender = new mayaSceneRender("mayaRender", MSceneRender::MSceneFilterOption::kRenderShadedItems);
    mayaUIRender = new mayaSceneRender("mayaUIRender", MSceneRender::MSceneFilterOption::kRenderUIItems);
    mayaUIRender->mCullingOption = MSceneRender::MCullingOption::kCullBackFaces;
    hudRender = new MHUDRender();
    blurBleedQuad = new quadRender("quad");
    bleedQuad = new quadRender("bleed");
    stylizeQuad = new quadRender("stylize");
    crossHatchQuad = new quadRender("crossHatch");
    substrateQuad = new quadRender("substrate");
    outlineQuad = new quadRender("outline");
    dilateQuad = new quadRender("dilate");

    clearQuad = new quadRender("clear");
    present = new MPresentTarget("Present Target");
    renderer = MRenderer::theRenderer();
    shaderManager = renderer->getShaderManager();

    //edit to be current path of this project
    MString projectPath("/Users/Jake/Documents/Penn/2020-2021/20Fall/Senior\ Design/Senior-Design/WatercolorRenderer/");
    shaderManager->addShaderPath(projectPath + "OGSFX");
    textureManager = renderer->getTextureManager();
    textureManager->addImagePath(projectPath + "Textures");

    paperTexture = textureManager->acquireTexture("normal_stockpaper.jpg");
    noiseTexture = textureManager->acquireTexture("bump_stockpaper.jpg");
    hatchTexture = textureManager->acquireTexture("stripes2.png");

    targetManager = renderer->getRenderTargetManager();
    objectShader = shaderManager->getEffectsFileShader( "vertexColor", "" );
    stylizeShader = shaderManager->getEffectsFileShader( "stylization", "" );
    crossHatchShader = shaderManager->getEffectsFileShader( "crosshatchStylization", "" );
    substrateShader = shaderManager->getEffectsFileShader( "substrate", "" );
    edgeShader = shaderManager->getEffectsFileShader("outline","");
    edgeDilateShader = shaderManager->getEffectsFileShader("dilateOutline","");

    if(!stylizeShader) {
        MGlobal::displayError("No Stylization shader found.");
    }

    if(!crossHatchShader) {
        MGlobal::displayError("No Crosshatching shader found.");
    }

    if(!substrateShader) {
        MGlobal::displayError("No Substrate shader found.");
    }

    if(!edgeShader) {
        MGlobal::displayError("No Edge Shader found.");
    }

    if(!edgeDilateShader) {
        MGlobal::displayError("No Edge Dilation shader found.");
    }
    if(objectShader) {
        vertexRender->mShader = objectShader;

    } else {
        MGlobal::displayError("ERROR: NO SHADER PATH FOUND");
    }

    clearShader = shaderManager->getEffectsFileShader( "clearColor", "" );
    clearQuad -> mShader = clearShader;

    target1 = NULL;
    target2 = NULL;
    target3 = NULL;
    target4 = NULL;
    target5 = NULL;
    target6 = NULL;

    targetDesc1 = new MRenderTargetDescription();
    targetDesc2 = new MRenderTargetDescription();
    targetDesc3 = new MRenderTargetDescription();
    targetDesc4 = new MRenderTargetDescription();
    targetDesc5 = new MRenderTargetDescription();
    targetDesc6 = new MRenderTargetDescription();
    targetDesc7 = new MRenderTargetDescription();

    setTargetDescriptions();

    if(targetManager) {
        target1 = targetManager->acquireRenderTarget(*targetDesc1);
        target2 = targetManager->acquireRenderTarget(*targetDesc2);
        target3 = targetManager->acquireRenderTarget(*targetDesc3);
        target4 = targetManager->acquireRenderTarget(*targetDesc4);
        target5 = targetManager->acquireRenderTarget(*targetDesc5);
        target6 = targetManager->acquireRenderTarget(*targetDesc6);
        target7 = targetManager->acquireRenderTarget(*targetDesc7);

    }

    if(!target1) {
        MGlobal::displayError("NO TARGET1");
    }

    if(!target2) {
        MGlobal::displayError("NO TARGET2");
    }

    if(!target3) {
        MGlobal::displayError("NO TARGET3");
    }

    if(!target4) {
        MGlobal::displayError("NO TARGE4");
    }

    if(!target5) {
        MGlobal::displayError("NO TARGET5");
    }

    if(!target6) {
        MGlobal::displayError("NO TARGET6");
    }

    if(!target7) {
        MGlobal::displayError("NO TARGET7");
    }

    clearQuad->mTargets.push_back(target1); // color image
    clearQuad->mTargets.push_back(target2); // control image
    clearQuad->mTargets.push_back(target3); // depth image

    vertexRender->mTargets.push_back(target2); // control image
    vertexRender->mTargets.push_back(target3); // depth image

    mayaRender->mTargets.push_back(target1); //color image

    blurBleedQuad->mTargets.push_back(target4); //bled image
    blurBleedQuad->mTargets.push_back(target5); // control image

    bleedQuad->mTargets.push_back(target6); //bled image
    bleedQuad->mTargets.push_back(target7); //blurred image
    bleedQuad->mTargets.push_back(target2); //control image
    stylizeQuad->mTargets.push_back(target4); //stylized image
    crossHatchQuad->mTargets.push_back(target6); //hatched image

    //outlines receives stylized/hatched target 6
    outlineQuad->mTargets.push_back(target4); //outlines

    //dilation also receives stylized/hatched target 6,outlines from 4, composites, writes to 7
    dilateQuad->mTargets.push_back(target7); //outlines

    postProcess = NULL;
    postProcess = shaderManager->getEffectsFileShader( "bleedPass0", "" );
    blurBleedQuad->mShader = postProcess;

    bleedShader = shaderManager->getEffectsFileShader( "bleedPass1", "" );
    bleedQuad -> mShader = bleedShader;
    outlineQuad->mShader = edgeShader;
    dilateQuad -> mShader = edgeDilateShader;
    stylizeQuad -> mShader = stylizeShader;
    substrateQuad->mShader = substrateShader;
    crossHatchQuad->mShader = crossHatchShader;

    shaderManager->clearEffectCache();
    updateShaderAttributes();

    MGlobal::displayInfo("done initialization");
}

MStatus nprRenderOverride::passRendersToPostProcess() {
    setTargetDescriptions();

    if(settings.mHatchTextureChanged) {
        MGlobal::displayInfo("changing texture");
        hatchTexture = textureManager->acquireTexture(settings.mHatchTexture);
        settings.mHatchTextureChanged = false;
    }

    if(paperTexture) {
       MHWRender::MTextureAssignment texAssignment;
        texAssignment.texture = paperTexture;
       substrateShader->setParameter("u_PaperNormal", texAssignment);
       stylizeShader->setParameter("u_PaperNormal", texAssignment);


    } else {
        MGlobal::displayError("No Paper Texture found");
    }

    if(noiseTexture) {
       MHWRender::MTextureAssignment texAssignment;
        texAssignment.texture = noiseTexture;
       substrateShader->setParameter("u_NoiseImage", texAssignment);
       stylizeShader->setParameter("u_NoiseImage", texAssignment);


    } else {
        MGlobal::displayError("No Noise Texture found");
    }

    if(hatchTexture) {
       MHWRender::MTextureAssignment texAssignment;
        texAssignment.texture = hatchTexture;
        crossHatchShader->setParameter("u_HatchImage", texAssignment);
    } else {
        MGlobal::displayError("No Noise Texture found");
    }

    if(postProcess) {
        if(target1) {
            target1->updateDescription( *( targetDesc1) );
            MRenderTargetAssignment assignment;
            assignment.target = target1;

            postProcess->setParameter("u_ColorImage", assignment);
            stylizeShader->setParameter("u_ColorImage", assignment);
            crossHatchShader->setParameter("u_ColorImage", assignment);
            bleedShader->setParameter("u_ColorImage", assignment);
            edgeShader->setParameter("u_ColorImage", assignment);

        } else {
            MGlobal::displayError(" no target 1");
        }

        if(target2) {
            target2->updateDescription( *( targetDesc2) );
            MRenderTargetAssignment assignment;
            assignment.target = target2;
            postProcess->setParameter("u_ControlImage", assignment);
            stylizeShader->setParameter("u_ControlImage", assignment);
            crossHatchShader->setParameter("u_ControlImage", assignment);
            substrateShader->setParameter("u_ControlImage", assignment);
            edgeShader->setParameter("u_ControlImage", assignment);
            edgeDilateShader->setParameter("u_ControlImage", assignment);


        } else {
            MGlobal::displayError("no target 2");
        }

        if(target3) {
            target3 -> updateDescription((*targetDesc3));
            MRenderTargetAssignment assignment;
            assignment.target = target3;

            postProcess->setParameter("u_DepthImage", assignment);
            bleedShader->setParameter("u_DepthImage", assignment);
            stylizeShader->setParameter("u_DepthImage", assignment);
            crossHatchShader->setParameter("u_DepthImage", assignment);
            edgeShader->setParameter("u_DepthImage", assignment);

        } else {
            MGlobal::displayError(" no target 3");
        }

        if(target4) {
            //pingpongs with 6
            target4->updateDescription((*targetDesc4));
            MRenderTargetAssignment assignment;
            assignment.target = target4;
            bleedShader->setParameter("u_BleededImage", assignment);
            crossHatchShader->setParameter("u_StylizedImage", assignment);
            edgeDilateShader->setParameter("u_OutlineImage", assignment);

        } else {
            MGlobal::displayError(" no target 4");
        }

        if(target5) {
            target5->updateDescription((*targetDesc5));
            MRenderTargetAssignment assignment;
            assignment.target = target5;
            bleedShader->setParameter("u_ControlImage", assignment);
        } else {
            MGlobal::displayError(" no target 5");
        }

        if(target6) {
            target6->updateDescription((*targetDesc6));
            MRenderTargetAssignment assignment;
            assignment.target = target6;
            stylizeShader->setParameter("u_BleededImage", assignment);
            edgeShader->setParameter("u_StylizedImage",assignment);
            edgeDilateShader->setParameter("u_StylizedImage",assignment);

        } else {
            MGlobal::displayError(" no target 6");
        }

        if(target7) {
            target7->updateDescription((*targetDesc7));
            MRenderTargetAssignment assignment;
            assignment.target = target7;
            stylizeShader->setParameter("u_BlurredImage", assignment);
            crossHatchShader->setParameter("u_BlurredImage", assignment);

            substrateShader->setParameter("u_ColorImage", assignment);


        } else {
            MGlobal::displayError(" no target 7");
        }


    } else {
        MGlobal::displayError("no post");
    }
}


nprRenderOverride::~nprRenderOverride() {
    MGlobal::displayInfo("deleting nprRenderOverride");

    vertexRender->releaseTextures();
    textureManager->releaseTexture(paperTexture);
    textureManager->releaseTexture(noiseTexture);
    textureManager->releaseTexture(hatchTexture);

    MGlobal::displayInfo("delete targets");

    if(target1) {
        targetManager->releaseRenderTarget(target1);
        target1 = NULL;
    }


    if(target2) {
        targetManager->releaseRenderTarget(target2);
        target2 = NULL;
    }

    if(target3) {
        targetManager->releaseRenderTarget(target3);
        target3 = NULL;
    }

    if(target4) {
        targetManager->releaseRenderTarget(target4);
        target4 = NULL;
    }

    if(target5) {
        targetManager->releaseRenderTarget(target5);
        target5 = NULL;
    }

    if(target6) {
        targetManager->releaseRenderTarget(target6);
        target6 = NULL;
    }

    if(target7) {
        targetManager->releaseRenderTarget(target7);
        target7 = NULL;
    }

    MGlobal::displayInfo("delete targ descriptions");

    delete targetDesc1;
    delete targetDesc2;
    delete targetDesc3;
    delete targetDesc4;
    delete targetDesc5;
    delete targetDesc6;
    delete targetDesc7;

    MGlobal::displayInfo("done deleting npr");

}


MStatus nprRenderOverride::setup(const MString &destination) {
    MStatus status = setupShaders();
    MRenderOverride::setup(destination);
    return status;
}

MStatus nprRenderOverride::setupShaders() {
    currOp = 0;
    updateShaderAttributes();
    passRendersToPostProcess();
    return MStatus::kSuccess;

}


bool nprRenderOverride::nextRenderOperation() {
    currOp += 1;
    return currOp < numOps;
}


MRenderOperation* nprRenderOverride::renderOperation(){
    switch( currOp )
    {
    case 0: return clearQuad;
    case 1: return vertexRender;
    case 2: return mayaRender;
    case 3: return blurBleedQuad;
    case 4: return bleedQuad;
    case 5: return stylizeQuad;
    case 6: return crossHatchQuad;
    case 7: return outlineQuad;
    case 8: return dilateQuad;
    case 9: return substrateQuad;
    case 10: return mayaUIRender;
    case 11: return hudRender;
    case 12: return present;
    default: return NULL;
    }
}


MStatus nprRenderOverride::setFixedColor(float r, float g, float b) {
    float col[4];
    col[0] = r;
    col[1] = g;
    col[2] = b;
    col[3] = 1.0;

    objectShader->setParameter("color",&col[0]);
}

MStatus nprRenderOverride::cleanup() {
    if(vertexRender) {

    }
}

MHWRender::DrawAPI nprRenderOverride::supportedDrawAPIs() const
{
    return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);
}

MStatus nprRenderOverride::setTargetDescriptions() {
    targetDesc1 -> setName("target1");
    setTargetDimensionsToScreen(targetDesc1);

    targetDesc2->setName("target2");
    targetDesc2->setRasterFormat(MHWRender::MRasterFormat::kR8G8B8A8_UNORM);
    setTargetDimensionsToScreen(targetDesc2);

    targetDesc3->setName("target3");
    setTargetDimensionsToScreen(targetDesc3);

    targetDesc4->setName("target4");
    targetDesc4->setRasterFormat(MHWRender::MRasterFormat::kR8G8B8A8_UNORM);
    setTargetDimensionsToScreen(targetDesc4);

    targetDesc5->setName("target5");
    setTargetDimensionsToScreen(targetDesc5);

    targetDesc6->setName("target6");
    setTargetDimensionsToScreen(targetDesc6);

    targetDesc7->setName("target7");
    setTargetDimensionsToScreen(targetDesc7);

    return MStatus::kSuccess;

}

MStatus nprRenderOverride::setTargetDimensions(MRenderTargetDescription* desc,
                                               float width, float height) {
    desc->setWidth(width);
    desc->setHeight(height);
    return MStatus::kSuccess;
}


MStatus nprRenderOverride::setTargetDimensionsToScreen(MRenderTargetDescription* desc) {
    unsigned int width = 0;
    unsigned int height = 0;
    renderer->outputTargetSize(width, height);
    return setTargetDimensions(desc, (float) width, float (height));
}

MStatus nprRenderOverride::updateShaderAttributes() {
    if(clearShader) {
        clearShader-> setParameter("u_PaperColor",settings.mPaperColor );
    } else {
        MGlobal::displayError("ERROR: NO SHADER PATH FOUND");
    }

    if(stylizeShader) {
        stylizeShader-> setParameter("u_PaperColor",settings.mPaperColor );
        stylizeShader-> setParameter("substrateHeightScale",settings.mPaperHeight );
        stylizeShader-> setParameter("dryness",settings.mPaperDryness );
        stylizeShader-> setParameter("paperDistortion",settings.mPaperDistortion );
        stylizeShader-> setParameter("lightAngle",settings.mPaperLightAngle );
        stylizeShader-> setParameter("roughness",settings.mPaperRoughness );
        stylizeShader-> setParameter("showControl",settings.mShowControl );
        stylizeShader-> setParameter("opacity",settings.mWaterColorOpacity );


    } else {
        MGlobal::displayError("ERROR: NO SHADER PATH FOUND");
    }


    if(crossHatchShader) {
        crossHatchShader-> setParameter("u_PaperColor",settings.mPaperColor );
        crossHatchShader-> setParameter("u_HatchColor",settings.mHatchColor );
        crossHatchShader-> setParameter("substrateHeightScale",settings.mPaperHeight );
        crossHatchShader-> setParameter("dryness",settings.mPaperDryness );
        crossHatchShader-> setParameter("paperDistortion",settings.mPaperDistortion );
        crossHatchShader-> setParameter("lightAngle",settings.mPaperLightAngle );
        crossHatchShader-> setParameter("roughness",settings.mPaperRoughness );
        crossHatchShader-> setParameter("showControl",settings.mShowControl );
        crossHatchShader-> setParameter("strokeFrequency",settings.mHatchFrequency );
        crossHatchShader-> setParameter("strokeAngle",settings.mHatchAngle );
        crossHatchShader-> setParameter("hatchIntensity",settings.mHatchIntensity );
        crossHatchShader-> setParameter("enabled",settings.mEnableHatching );
        crossHatchShader-> setParameter("outlineEnabled",settings.mEnableOutline );
        crossHatchShader-> setParameter("outlineThreshold",settings.mOutlineThreshold );
        crossHatchShader-> setParameter("cellBlend",settings.mHatchCellBlending );
        crossHatchShader-> setParameter("offsetRand",settings.mHatchOffsetStochasticity );
        crossHatchShader-> setParameter("angleRand",settings.mHatchAngleStochasticity );

    } else {
        MGlobal::displayError("ERROR: NO SHADER PATH FOUND");
    }

    if(edgeShader) {
        edgeShader-> setParameter("outlineEnabled",settings.mEnableOutline );
        edgeShader-> setParameter("outlineThreshold",settings.mOutlineThreshold );
        edgeShader-> setParameter("useDepth",settings.mOutlineUseDepth );

    }

    if(edgeDilateShader) {
        edgeDilateShader->setParameter("thickness", settings.mOutlineThickness);
        edgeDilateShader->setParameter("outlineColor", settings.mOutlineColor);

    }

    if(substrateShader) {
        substrateShader-> setParameter("u_PaperColor",settings.mPaperColor );
        substrateShader-> setParameter("substrateHeightScale",settings.mPaperHeight );
        substrateShader-> setParameter("dryness",settings.mPaperDryness );
        substrateShader-> setParameter("paperDistortion",settings.mPaperDistortion );
        substrateShader-> setParameter("lightAngle",settings.mPaperLightAngle );
        substrateShader-> setParameter("roughness",settings.mPaperRoughness );
        substrateShader-> setParameter("showControl",settings.mShowControl );
    }

    if(objectShader) {
        objectShader-> setParameter("u_PaperColor",settings.mPaperColor );
    } else {
        MGlobal::displayError("ERROR: NO SHADER PATH FOUND");
    }

}


