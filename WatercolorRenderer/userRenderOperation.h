#pragma once

#include <stdio.h>

#include <maya/MString.h>
#include <maya/MColor.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MRenderTargetManager.h>
#include <maya/MShaderManager.h>
class userRenderOperation : public MUserRenderOperation {
    userRenderOperation(const MString name);
    ~userRenderOperation() override;


};
