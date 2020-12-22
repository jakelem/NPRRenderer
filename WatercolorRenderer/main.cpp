#include <stdio.h>
#include <maya/MString.h>
#include <maya/MFnPlugin.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MGlobal.h>

#include "nprRenderOverride.h"

#include "nprnode.h"

#include <maya/MDrawRegistry.h>
#include <maya/MPxShaderOverride.h>
#include <maya/MDrawContext.h>
#include <maya/MStateManager.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MShaderManager.h>
#include <maya/MHWShaderSwatchGenerator.h>
#include <maya/MPxCommand.h>
#include <maya/MArgList.h>

static const MString watercolorId("watercolorId");
MStatus initializePlugin(MObject obj)
{
    MStatus status;
    MFnPlugin plugin(obj, PLUGIN_COMPANY, "1.0", "Any");

    MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
    if (renderer)
    {
        npr = new nprRenderOverride("test");
        renderer->registerOverride(npr);
    }

    if(npr) {
        MGlobal::displayInfo("Npr Node Successful");

    } else {
        MGlobal::displayError("Npr failed");

}
    if (status != MS::kSuccess) return status;

    nprNode::nprRenderer = npr;
    nprNode* node = new nprNode();

    status = plugin.registerNode( "nprNode", nprNode::id, nprNode::creator,
                                  nprNode::initialize );
    if (!status) {
        status.perror("registerNode");
        return status;
    } else {
        MGlobal::displayInfo("registered node complete");
    }


    MString nodeCommand(   "if( `objExists nprSettings` ) {"
                               "select nprSettings;"
                           "} else {"
                               "createNode locator -n nprDummyNode;"
                               "createNode nprNode -n nprSettings;"
                               "connectAttr nprSettings.output nprDummyNode.lpx;"
                               "select nprSettings;"
                           "}"
                        );

    MGlobal::executeCommand(nodeCommand);

    return status;
}

MStatus uninitializePlugin(MObject obj)
{
    MStatus status;
    MFnPlugin plugin(obj);
    MGlobal::displayInfo("uninitialize plugin");

    MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
    MGlobal::displayInfo("deregisterNode");

    if (renderer)
    {
        if (npr)
        {
            MGlobal::displayInfo("now deregistering npr");
            renderer->deregisterOverride(npr);
            MGlobal::displayInfo("now deleting npr");

            delete npr;
        }
        MGlobal::displayInfo("finish npr deregister");
        npr = NULL;
    }

    status = plugin.deregisterNode( nprNode::id );
    if (!status) {
        status.perror("deregisterNode");
        return status;
    }

    return status;
}



int main() {
    return 0;
}
