#include "boundingbox.h"
#include<algorithm>

boundingbox::boundingbox(){};

boundingbox::boundingbox(double x, double y, double xx, double yy){
    lowedge[0] = std::min(x,xx);
    lowedge[1] = std::min(y,yy);
    highedge[0] = std::max(xx,x);
    highedge[1] = std::max(yy,y);
}

void boundingbox::reset() {
    for (int axis = 0; axis < 2; axis++) {
        lowedge[axis] = INT_MAX;
        highedge[axis] = INT_MIN;
    }
}

void boundingbox::assignbound(const boundingbox& bb) {
    for (int i = 0; i < 2; i++) {
        lowedge[i] = bb.lowedge[i];
        highedge[i] = bb.highedge[i];
    }
}

bool boundingbox::stretch(double * lowedge, double* highedge) {
    bool ret = false;
    for (int axis = 0; axis < 2; axis++) {
        if (this->lowedge[axis] > lowedge[axis]) {
            this->lowedge[axis] = lowedge[axis];
            ret = true;
        }
        if (this->highedge[axis] < highedge[axis]) {
            this->highedge[axis] = highedge[axis];
            ret = true;
        }
    }
    return ret;
}

double boundingbox::overlap(const boundingbox& bb)const {
    double area = 1.0;
    for (int axis = 0; area && axis < 2; axis++) {
        const double x1 = lowedge[axis];
        const double x2 = highedge[axis];
        const double x11 = bb.lowedge[axis];
        const double x22 = bb.highedge[axis];
        if (x1 < x11) {
            if (x11 < x2) {
                if (x22 < x2)area *= (double)(x22 - x11);
                else area *= (double)(x2 - x11);

                continue;
            }
        }
        else if (x1 < x22) {
            if (x2 < x22)area *= (double)(x2 - x1);
            else area *= (double)(x22 - x1);

            continue;
        }

        return 0.0;
    }
    return area;
}

double boundingbox::distanceFromCenter(const boundingbox& bb)const {
    double distance = 0, t;
    for (int axis = 0; axis < 2; axis++) {
        t = ((double)lowedge[axis] + (double)highedge[axis] + (double)bb.lowedge[axis] + (double)bb.highedge[axis]) / 2.0;
        distance += t * t;
    }
    return distance;
}

bool boundingbox::operator==(const boundingbox& bb) {
    for (int axis = 0; axis < 2; axis++) {
        if (lowedge[axis] != bb.lowedge[axis] || highedge[axis] != bb.highedge[axis])
            return false;
    }
    return true;
}

