#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#define axis_x 0
#define axis_y 1;
#define dimensions 2;

class boundingbox
{

public:
    double lowedge[2];
    double highedge[2];

public:
    boundingbox();
    boundingbox(double x, double y, double xx, double yy);
    void reset();
    void assignbound(const boundingbox& bb);
    bool stretch(double * lowedge, double* highedge);
    inline double edgeDeltas()const {
        double distance = 0;
        for (int axis = 0; axis < 2; axis++) {
            distance += highedge[axis] - lowedge[axis];
        }
        return distance;
    }
    inline double area()const {
        double area = 1;
        for (int axis = 0; axis < 2; axis++) {
            area *= (double)(highedge[axis] - lowedge[axis]);
        }
        return area;
    }
    inline bool Overlap(boundingbox* node, double* lowedge, double* highedge) {
        for (int axis = 0; axis < 2; axis++) {
            if (!(lowedge[axis] < node->highedge[axis]) || !(node->lowedge[axis] < highedge[axis]))
                return false;
        }
        return true;
    }
    inline bool Cover(boundingbox* node, double* lowedge, double* highedge) {

        if ((node->lowedge[0] > lowedge[0] && node->highedge[0] < highedge[0])
            && (node->lowedge[0] < highedge[0] && node->highedge[0] > lowedge[0])
            && (node->lowedge[1] > lowedge[1] && node->highedge[1] < highedge[1])
            && (node->lowedge[1] < highedge[1] && node->highedge[1] > lowedge[1]))
            return true;
        return false;

    }
    double overlap(const boundingbox& bb)const;
    double distanceFromCenter(const boundingbox& bb) const;
    bool operator==(const boundingbox& bb);

public:
    struct SortByLowedge {
    public:
        const int m_axis;
        explicit SortByLowedge(const int axis) :m_axis(axis) {}
        bool operator()(const boundingbox* const bi1, const boundingbox* const bi2)const {
            return bi1->lowedge[m_axis] < bi2->lowedge[m_axis];
        }
    };
    struct SortByHighedge {
    public:
        const int m_axis;
        explicit SortByHighedge(const int axis) :m_axis(axis) {}
        bool operator()(const boundingbox* const bi1, const boundingbox* const bi2)const {
            return bi1->highedge[m_axis] < bi2->highedge[m_axis];
        }
    };
    struct SortByDistanceFromCenter {
    public:
        const boundingbox* const m_center;
        explicit SortByDistanceFromCenter(const boundingbox* const center) :m_center(center) {}
        bool operator()(const boundingbox* const bi1, const boundingbox* const bi2)const {
            return bi1->distanceFromCenter(*m_center) < bi2->distanceFromCenter(*m_center);
        }
    };
    struct SortByArea {
    public:
        explicit SortByArea(){}
        bool operator()(const boundingbox* const bi1, const boundingbox* const bi2)const {
            return bi1->area() >  bi2->area();
        }
    };/*
    struct SortByAreaEnlargement {
        public:
            const double area;
            explicit SortByAreaEnlargement(const boundingbox * center) :area(center->area()) {}
            bool operator()(const boundingbox* const bi1, const boundingbox* const bi2)const {
                return area - bi1->area() < area - bi2->area();
            }
    };
    struct SortByOverlapEnlargement {
    public:
        const boundingbox* const m_center;
        explicit SortByOverlapEnlargement(const boundingbox * const center) :m_center(center) {}
        bool operator()(const boundingbox*const bi1, const boundingbox* const bi2)const {
            return bi1->overlap(*m_center) < bi2->overlap(*m_center);
        }
    };*/

};

#endif // BOUNDINGBOX_H
