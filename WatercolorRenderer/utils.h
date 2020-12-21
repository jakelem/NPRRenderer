#ifndef UTILS_H
#define UTILS_H
# define M_PI           3.14159265358979323846
#include <maya/MString.h>
#include <maya/MColor.h>
#include <maya/MGlobal.h>
class utils
{
public:
    utils();
    static MString floatToMString(float f) {
        return MString(std::to_string(f).c_str());
    }

    static float radiansToDegrees(float f) {
        return f * 180.f / M_PI;
    }


    static float degreesToRadians(float f) {
        return f * M_PI / 180.f;
    }

};

#endif // UTILS_H
