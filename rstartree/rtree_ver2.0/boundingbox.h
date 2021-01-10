#pragma once
#define axis_x 0
#define axis_y 1;
#define dimensions 2;

#include <string>
#include<iomanip>


class boundingbox {
public:
	double lowedge[2];
	double highedge[2];
public:
	boundingbox() {};
	boundingbox(double x, double y, double xx, double yy){
			lowedge[0] = min(x,xx);
			lowedge[1] = min(y,yy);
			highedge[0] = max(xx,x);
			highedge[1] = max(yy,y);
		}


	void reset() {
		for (int axis = 0; axis < 2; axis++) {
			lowedge[axis] = INT_MAX;
			highedge[axis] = INT_MIN;
		}
	}

	void assignbound(const boundingbox& bb) {
		for (int i = 0; i < 2; i++) {
			lowedge[i] = bb.lowedge[i];
			highedge[i] = bb.highedge[i];
		}

	}
	//拓展box从而包住所有的小box
	bool stretch(double * lowedge, double* highedge) {
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

	inline bool encloses(boundingbox& bb) const {
		for (int axis = 0; axis < 2; axis++) {
			if (bb.lowedge[axis] < lowedge[axis] || highedge[axis] < bb.highedge[axis])
				return false;
			return true;
		}
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

	double overlap(const boundingbox& bb)const {
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

	double distanceFromCenter(const boundingbox& bb)const {
		double distance = 0, t;
		for (int axis = 0; axis < 2; axis++) {
			t = ((double)lowedge[axis] + (double)highedge[axis] + (double)bb.lowedge[axis] + (double)bb.highedge[axis]) / 2.0;
			distance += t * t;
		}
		return distance;
	}
	bool operator==(const boundingbox& bb) {
		for (int axis = 0; axis < 2; axis++) {
			if (lowedge[axis] != bb.lowedge[axis] || highedge[axis] != bb.highedge[axis])
				return false;
		}
		return true;
	}

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
	};

};

struct leafbox {
	double taxi_1[2];
	double taxi_2[2];
	leafbox(double x,double y,double xx,double yy){
		taxi_1[0] = x; taxi_1[1] = y;
		taxi_2[0] = xx; taxi_2[1] = yy;
	}
	leafbox() {};
};