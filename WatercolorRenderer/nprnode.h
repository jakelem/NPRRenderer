#pragma once
#include <nprRenderOverride.h>

#include <maya/MPxNode.h>
#include <nprRenderOverride.h>
#include <maya/MGlobal.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnStringData.h>

#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MVector.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MObject.h>
#include <utils.h>

class nprNode : public MPxNode
{
public:
    nprNode();
    ~nprNode();
    MStatus compute(const MPlug& plug, MDataBlock& data) override;

    static MStatus tryAddAttribute(MObject attr);
    static MStatus tryAttributeAffects(MObject attr1, MObject attr2);

    static  void*           creator();
    static nprRenderOverride* nprRenderer;
    static  MStatus         initialize();

    static  MObject         watercolorOpacity;

    static  MObject         paperHeight;
    static  MObject         paperColor;
    static  MObject         paperDryness;
    static  MObject         paperDistortion;
    static  MObject         paperLightAngle;
    static  MObject         paperRoughness;

    static  MObject         hatchEnabled;
    static  MObject         hatchAngle;
    static  MObject         hatchAngleStochasticity;
    static  MObject         hatchOffsetStochasticity;

    static  MObject         hatchFrequency;
    static  MObject         hatchIntensity;
    static  MObject         hatchTextureFile;
    static  MObject         hatchCellBlending;
    static  MObject         hatchColor;

    static  MObject         outlineEnabled;
    static  MObject         outlineThreshold;
    static  MObject         outlineThickness;
    static  MObject         outlineColor;
    static  MObject         outlineUseDepth;

    static  MObject         showControl;

    static  MObject         output;        // The output value.
    static  MTypeId         id;


};


