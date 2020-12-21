#include <stdio.h>
#include "userRenderOperation.h"

#include <maya/MString.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MRenderTargetManager.h>

#include <maya/MIOStream.h>
#include <maya/MStreamUtils.h>
#include <maya/MGlobal.h>


userRenderOperation::userRenderOperation(const MString name) : MUserRenderOperation(name) {

}


userRenderOperation::~userRenderOperation() {

}
