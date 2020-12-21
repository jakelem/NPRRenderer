#ifndef RENDERERSETTINGS_H
#define RENDERERSETTINGS_H

#include <utils.h>
class rendererSettings
{
public:
    rendererSettings();
    float mWaterColorOpacity = 1.0;

    float mPaperColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float mHatchColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    float mPaperHeight = 1.0;
    float mPaperDistortion = 1.7;
    float mPaperDryness = 1.0;
    float mPaperLightAngle = utils::degreesToRadians(90.0);
    float mPaperRoughness = 1.0;

    bool mShowControl = false;

   //crosshatch settings
    bool mEnableHatching = false;

    bool mEnableOutline = true;
    float mOutlineThreshold = 0.0;
    int mOutlineThickness = 0;
    float mOutlineColor[3] = {0.0f,0.0f,0.0f};
    bool mOutlineUseDepth = false;


    float mHatchCellBlending = 1.5;
    float mHatchFrequency = 5.0;
    float mHatchAngleStochasticity = 100.0;
    float mHatchOffsetStochasticity = 0.8;

    float mHatchAngle = 90.0;
    float mHatchIntensity = 3.0;

    MString mHatchTexture = "stripes2.png";
    bool mHatchTextureChanged = true;

    void updateHatchTexture(MString texturePath) {
        if(texturePath != mHatchTexture) {
            mHatchTextureChanged = true;
            mHatchTexture = texturePath;
        }
    }

};

#endif // RENDERERSETTINGS_H
