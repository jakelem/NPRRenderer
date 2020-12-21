#include <stdio.h>
#include "quadRender.h"

#include <maya/MString.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MRenderTargetManager.h>

#include <maya/MIOStream.h>
#include <maya/MStreamUtils.h>
#include <maya/MGlobal.h>


quadRender::quadRender(const MString name) : MQuadRender(name) {

}

quadRender::~quadRender() {

}

const MShaderInstance* quadRender::shader() {
    return mShader;
}


MRenderTarget* const* quadRender::targetOverrideList(unsigned int &listSize)  {
    listSize = mTargets.size();
    if(listSize > 0) {
        return mTargets.data();
    } else {
        return NULL;
    }
}

MClearOperation & quadRender::clearOperation()
{
    mClearOperation.setMask((unsigned int) MHWRender::MClearOperation::kClearAll);
    return mClearOperation;
}


MStatus quadRender::addInputTarget(MString *target) {

}


MStatus quadRender::setShaderColorDepth(MTexture *color, MTexture *depth) {
    if(!color) {
        MGlobal::displayInfo("bad color");
    }

    if(!depth) {
        MGlobal::displayInfo("bad depth");

    }
    if(mShader) {
        MHWRender::MTextureAssignment texAssignment;
        texAssignment.texture = (MHWRender::MTexture*) color;

        mShader->setParameter("u_ColorImage", texAssignment);

        MHWRender::MTextureAssignment texAssignment2;
        texAssignment2.texture = (MHWRender::MTexture*) depth;

        mShader->setParameter("u_DepthImage", texAssignment2);

    }
    return MStatus::kSuccess;
}


