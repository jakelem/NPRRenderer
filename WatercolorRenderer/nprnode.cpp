#include "nprnode.h"

MTypeId nprNode::id(0x80000);
MObject     nprNode::paperHeight;
MObject     nprNode::paperColor;
MObject     nprNode::paperDryness;
MObject     nprNode::paperDistortion;
MObject     nprNode::paperLightAngle;
MObject     nprNode::paperRoughness;
MObject     nprNode::hatchAngle;
MObject     nprNode::hatchFrequency;
MObject     nprNode::hatchIntensity;
MObject     nprNode::hatchTextureFile;
MObject     nprNode::hatchCellBlending;
MObject     nprNode::hatchColor;

MObject     nprNode::hatchEnabled;
MObject     nprNode::outlineEnabled;
MObject     nprNode::outlineThreshold;
MObject     nprNode::outlineThickness;
MObject     nprNode::outlineColor;
MObject     nprNode::outlineUseDepth;

MObject     nprNode::hatchAngleStochasticity;
MObject     nprNode::hatchOffsetStochasticity;

MObject     nprNode::watercolorOpacity;
MObject     nprNode::showControl;

MObject     nprNode::output;
nprRenderOverride* nprNode::nprRenderer;

nprNode::nprNode()
{
}

nprNode::~nprNode()
{
}

MStatus nprNode::compute(const MPlug& plug, MDataBlock& data) {

    MStatus returnStatus;
    MGlobal::displayInfo("compute");
/*
    if(npr) {
        MDataHandle inputData = data.inputValue( input, &returnStatus );

        MGlobal::displayInfo("yes npr");

        npr ->paper[0] = inputData.asFloat();
        npr->refresh();
    } else {
        MGlobal::displayInfo("no npr");
    }*/

    if( plug == output )
    {
            MDataHandle paperCol = data.inputValue( paperColor, &returnStatus );
            MDataHandle paperH = data.inputValue( paperHeight, &returnStatus );
            MDataHandle paperDry = data.inputValue( paperDryness, &returnStatus );
            MDataHandle paperDisp = data.inputValue( paperDistortion, &returnStatus );
            MDataHandle paperAngle = data.inputValue( paperLightAngle, &returnStatus );
            MDataHandle paperRough = data.inputValue( paperRoughness, &returnStatus );

            MDataHandle hatchEn = data.inputValue( hatchEnabled, &returnStatus );
            MDataHandle hatchFreq = data.inputValue( hatchFrequency, &returnStatus );
            MDataHandle hatchAng = data.inputValue( hatchAngle, &returnStatus );
            MDataHandle hatchInten = data.inputValue( hatchIntensity, &returnStatus );
            MDataHandle hatchTex = data.inputValue( hatchTextureFile, &returnStatus );
            MDataHandle hatchBlend = data.inputValue( hatchCellBlending, &returnStatus );
            MDataHandle hatchCol = data.inputValue( hatchColor, &returnStatus );


            MDataHandle hatchAngStoch = data.inputValue( hatchAngleStochasticity, &returnStatus );
            MDataHandle hatchOffStoch = data.inputValue( hatchOffsetStochasticity, &returnStatus );

            MDataHandle watercolorOp = data.inputValue( watercolorOpacity, &returnStatus );

            MDataHandle outlineEn = data.inputValue( outlineEnabled, &returnStatus );
            MDataHandle outlineThresh = data.inputValue( outlineThreshold, &returnStatus );
            MDataHandle outlineThick = data.inputValue( outlineThickness, &returnStatus );
            MDataHandle outlineCol = data.inputValue( outlineColor, &returnStatus );
            MDataHandle outlineDepth = data.inputValue( outlineUseDepth, &returnStatus );

            MDataHandle showCtrl = data.inputValue( showControl, &returnStatus );

            MFloatVector& pc = paperCol.asFloatVector();
            MFloatVector& hc = hatchCol.asFloatVector();
            MFloatVector& oc = outlineCol.asFloatVector();


            if( returnStatus != MS::kSuccess )
                    cerr << "ERROR getting data" << endl;
            else
            {
                if(nprRenderer) {
                    MGlobal::displayInfo("yes npr");
                    nprRenderer->settings.mPaperColor[0] = pc.x;
                    nprRenderer->settings.mPaperColor[1] = pc.y;
                    nprRenderer->settings.mPaperColor[2] = pc.z;
                    nprRenderer->settings.mPaperHeight = paperH.asFloat();
                    nprRenderer->settings.mPaperDryness = paperDry.asFloat();
                    nprRenderer->settings.mPaperDistortion = paperDisp.asFloat();
                    nprRenderer->settings.mPaperRoughness = paperRough.asFloat();

                    nprRenderer->settings.mWaterColorOpacity = watercolorOp.asFloat();
                    nprRenderer->settings.mHatchAngle = hatchAng.asFloat();
                    nprRenderer->settings.mHatchFrequency = hatchFreq.asFloat();
                    nprRenderer->settings.mHatchIntensity = hatchInten.asFloat();
                    nprRenderer->settings.mHatchAngleStochasticity = hatchAngStoch.asFloat();
                    nprRenderer->settings.mHatchOffsetStochasticity = hatchOffStoch.asFloat();
                    nprRenderer->settings.mOutlineThickness = outlineThick.asInt();
                    nprRenderer->settings.mOutlineUseDepth = outlineDepth.asBool();

                    nprRenderer->settings.mHatchColor[0] = hc.x;
                    nprRenderer->settings.mHatchColor[1] = hc.y;
                    nprRenderer->settings.mHatchColor[2] = hc.z;

                    nprRenderer->settings.mPaperLightAngle = utils::degreesToRadians(paperAngle.asFloat());
                    nprRenderer->settings.mEnableHatching = hatchEn.asBool();
                    nprRenderer->settings.mHatchCellBlending = hatchBlend.asFloat();

                    nprRenderer->settings.updateHatchTexture(hatchTex.asString());
                    nprRenderer->settings.mEnableOutline = outlineEn.asBool();
                    nprRenderer->settings.mOutlineThreshold = outlineThresh.asFloat();

                    nprRenderer->settings.mOutlineColor[0] = oc.x;
                    nprRenderer->settings.mOutlineColor[1] = oc.y;
                    nprRenderer->settings.mOutlineColor[2] = oc.z;

                    nprRenderer->settings.mShowControl = showCtrl.asBool();

                   // std::string f = std::to_string(nprRenderer->settings.mPaperColor[0]);
                   // MGlobal::displayInfo(MString(f.c_str()));

                   // nprRenderer->refresh();

                } else {
                    MGlobal::displayInfo("no npr");

                }
                    float result = sinf( paperH.asFloat() ) * 10.0f;
                    result += pc.x;
                   // MDataHandle outputHandle = data.outputValue( output );
                   // outputHandle.set( result );
                    data.setClean(plug);
            }
    } else {
            return MStatus::kUnknownParameter;
    }

    return MStatus::kSuccess;
}


void* nprNode::creator()
{
    return new nprNode();
}

MStatus nprNode::tryAddAttribute(MObject attr) {
    MStatus status = addAttribute(attr);
    if(!status) {
        MGlobal::displayError("addAttribute failed");
    }

    return status;
}


MStatus nprNode::tryAttributeAffects(MObject attr1, MObject attr2) {
    MStatus status = attributeAffects(attr1, attr2);
    if(!status) {
        MGlobal::displayError("attributeAffects failed");
    }

    return status;
}

MStatus nprNode::initialize()
{
        MFnNumericAttribute nAttr;
        MStatus                         stat;

        MFnTypedAttribute tAttr;

        watercolorOpacity = nAttr.create( "watercolorOpacity", "wo", MFnNumericData::kFloat, nprRenderer->settings.mWaterColorOpacity );
        nAttr.setSoftMin(0.0);
        nAttr.setSoftMax(1.0);

        paperHeight = nAttr.create( "paperHeight", "ph", MFnNumericData::kFloat, nprRenderer->settings.mPaperHeight );
        nAttr.setStorable(true);
        nAttr.setSoftMin(0.0);
        nAttr.setSoftMax(8.0);

        paperColor = nAttr.createColor("paperColor","pc");
        nAttr.setDefault(nprRenderer->settings.mPaperColor[0], nprRenderer->settings.mPaperColor[1], nprRenderer->settings.mPaperColor[2]);
        paperDryness = nAttr.create( "paperDryness", "pd", MFnNumericData::kFloat, nprRenderer->settings.mPaperDryness );
        nAttr.setSoftMin(0.0);
        nAttr.setSoftMax(5.0);

        paperDistortion = nAttr.create( "paperDistortion", "pt", MFnNumericData::kFloat, nprRenderer->settings.mPaperDistortion );
        nAttr.setSoftMin(0.0);
        nAttr.setSoftMax(300.0);

        paperLightAngle = nAttr.create( "paperLightAngle", "pl", MFnNumericData::kFloat, utils::radiansToDegrees(nprRenderer->settings.mPaperLightAngle) );
        nAttr.setSoftMin(0.0);
        nAttr.setSoftMax(180.0);

        paperRoughness = nAttr.create( "paperRoughness", "pr", MFnNumericData::kFloat, nprRenderer->settings.mPaperRoughness );
        nAttr.setSoftMin(0.0);
        nAttr.setSoftMax(10.0);

        hatchTextureFile = tAttr.create("hatchTextureFile","ht",MFnData::kString,MFnStringData().create("stripes2.png"));

        hatchCellBlending = nAttr.create( "hatchCellBlending", "hc", MFnNumericData::kFloat, nprRenderer->settings.mHatchCellBlending );
        nAttr.setSoftMin(0.0);
        nAttr.setSoftMax(2.0);

        hatchAngle = nAttr.create( "hatchAngle", "ha", MFnNumericData::kFloat, nprRenderer->settings.mHatchAngle );
        nAttr.setSoftMin(0.0);
        nAttr.setSoftMax(360.0);

        hatchFrequency = nAttr.create( "hatchFrequency", "hf", MFnNumericData::kFloat, nprRenderer->settings.mHatchFrequency );
        nAttr.setSoftMin(0.1);
        nAttr.setSoftMax(50.0);

        hatchIntensity = nAttr.create( "hatchIntensity", "hi", MFnNumericData::kFloat, nprRenderer->settings.mHatchIntensity );
        nAttr.setSoftMin(0.0);
        nAttr.setSoftMax(50.0);

        hatchAngleStochasticity = nAttr.create( "hatchAngleStochasticity", "hs", MFnNumericData::kFloat, nprRenderer->settings.mHatchAngleStochasticity );
        nAttr.setSoftMin(0.0);
        nAttr.setSoftMax(360.0);

        hatchOffsetStochasticity = nAttr.create( "hatchOffsetStochasticity", "ho", MFnNumericData::kFloat, nprRenderer->settings.mHatchOffsetStochasticity );
        nAttr.setSoftMin(0.0);
        nAttr.setSoftMax(1.0);

        hatchColor = nAttr.createColor("hatchColor","hl");
        nAttr.setDefault(nprRenderer->settings.mHatchColor[0], nprRenderer->settings.mHatchColor[1], nprRenderer->settings.mHatchColor[2]);

        hatchEnabled = nAttr.create( "hatchEnabled", "he", MFnNumericData::kBoolean, nprRenderer->settings.mEnableHatching );
        nAttr.setStorable(true);

        outlineEnabled = nAttr.create( "outlineEnabled", "oe", MFnNumericData::kBoolean, nprRenderer->settings.mEnableOutline );
        nAttr.setStorable(true);


        outlineUseDepth = nAttr.create( "outlineUseDepth", "od", MFnNumericData::kBoolean, nprRenderer->settings.mOutlineUseDepth );
        nAttr.setStorable(true);

        outlineThreshold = nAttr.create( "outlineThreshold", "ot", MFnNumericData::kFloat, nprRenderer->settings.mOutlineThreshold );
        nAttr.setSoftMin(0.0);
        nAttr.setSoftMax(1.0);

        outlineThickness = nAttr.create( "outlineThickness", "oh", MFnNumericData::kInt, nprRenderer->settings.mOutlineThickness );
        nAttr.setMin(-5);
        nAttr.setMax(5);

        outlineColor = nAttr.createColor("outlineColor","oc");
        nAttr.setDefault(nprRenderer->settings.mOutlineColor[0], nprRenderer->settings.mOutlineColor[1], nprRenderer->settings.mOutlineColor[2]);

        showControl = nAttr.create( "debugControl", "dc", MFnNumericData::kBoolean, false );


        output = nAttr.create( "output", "out", MFnNumericData::kFloat, 0.0 );
        nAttr.setWritable(false);
        nAttr.setStorable(false);

        std::vector<MObject*> allAttributes;
        allAttributes.push_back(&paperColor);
        allAttributes.push_back(&paperHeight);
        allAttributes.push_back(&paperDryness);
        allAttributes.push_back(&paperDistortion);
        allAttributes.push_back(&paperLightAngle);
        allAttributes.push_back(&paperRoughness);
        allAttributes.push_back(&showControl);
        allAttributes.push_back(&hatchAngle);
        allAttributes.push_back(&hatchFrequency);
        allAttributes.push_back(&hatchIntensity);
        allAttributes.push_back(&hatchAngleStochasticity);
        allAttributes.push_back(&hatchOffsetStochasticity);

        allAttributes.push_back(&hatchTextureFile);
        allAttributes.push_back(&hatchCellBlending);
        allAttributes.push_back(&hatchColor);

        allAttributes.push_back(&hatchEnabled);
        allAttributes.push_back(&outlineEnabled);
        allAttributes.push_back(&outlineThreshold);
        allAttributes.push_back(&outlineThickness);
        allAttributes.push_back(&outlineColor);
        allAttributes.push_back(&outlineUseDepth);

        allAttributes.push_back(&watercolorOpacity);

        stat = addAttribute( output );
                if (!stat) { stat.perror("addAttribute"); return stat;}

        for(int i =0; i < allAttributes.size(); i++) {
            stat = tryAddAttribute(*allAttributes[i]);
            stat = tryAttributeAffects(*allAttributes[i], output);

        }

        return MS::kSuccess;
}
