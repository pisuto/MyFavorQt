#ifndef OWN_UTIL_H
#define OWN_UTIL_H

#include <QtMath>

namespace mf {

class OwnUtil {
public:
    static float triangleFunc(float x)
    {
        if(x <= 0.5f)
            return x;
        else if(x > 0.5f && x < 1.0f)
            return 1 - x;
        return 0.f;
    }
};

}

#endif // OWN_UTIL_H
