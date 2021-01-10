#pragma once
#define axis_x 0
#define axis_y 1;
#define dimensions 2;

#include <string>
#include<iomanip>


class boundingbox {
public:
	double lowedge[2];//box左下角的坐标
	double highedge[2];//box右上角的坐标
public:
	boundingbox() {};
	boundingbox(double x, double y, double xx, double yy){
			lowedge[0] = min(x,xx);
			lowedge[1] = min(y,yy);
			highedge[0] = max(xx,x);
			highedge[1] = max(yy,y);
		}

	//重置
	void reset() {
		for (int axis = 0; axis < 2; axis++) {
			lowedge[axis] = INT_MAX;
			highedge[axis] = INT_MIN;
		}
	}

	//赋值
	void assignbound(const boundingbox& bb) {
		for (int i = 0; i < 2; i++) {
			lowedge[i] = bb.lowedge[i];
			highedge[i] = bb.highedge[i];
		}

	}

	//拓展box从而包住所有的小box
	bool stretch(const double * lowedge,const double* highedge) {
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

	//计算margin周长
	inline double edgeDeltas()const {
		double distance = 0;
		for (int axis = 0; axis < 2; axis++) {
			distance += highedge[axis] - lowedge[axis];
		}
		return distance;
	}

	//计算面积
	inline double area()const {
		double area = 1;
		for (int axis = 0; axis < 2; axis++) {
			area *= (double)(highedge[axis] - lowedge[axis]);
		}
		return area;
	}

	//判断是否重叠
	inline bool Overlap(boundingbox* node, double* lowedge, double* highedge) {
		for (int axis = 0; axis < 2; axis++) {
			if (!(lowedge[axis] < node->highedge[axis]) || !(node->lowedge[axis] < highedge[axis]))
				return false;
		}
		return true;
	}

	//判断是否包含关系
	inline bool Cover(boundingbox* node, double* lowedge, double* highedge) {

		if ((node->lowedge[0] > lowedge[0] && node->highedge[0] < highedge[0])
			&& (node->lowedge[0] < highedge[0] && node->highedge[0] > lowedge[0])
			&& (node->lowedge[1] > lowedge[1] && node->highedge[1] < highedge[1])
			&& (node->lowedge[1] < highedge[1] && node->highedge[1] > lowedge[1]))
			return true;
		return false;

	}

	//计算重叠面积
	double overlap_area(const boundingbox& bb)const {
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

	//两个box中心点之间的距离
	double distanceFromCenter(const boundingbox& bb)const {
		double distance = 0, t;
		for (int axis = 0; axis < 2; axis++) {
			t = ((double)lowedge[axis] + (double)highedge[axis] + (double)bb.lowedge[axis] + (double)bb.highedge[axis]) / 2.0;
			distance += t * t;
		}
		return distance;
	}


	//根据box左下小坐标的某一维度排序
	struct SortByLowedge {
	public:
		const int m_axis;
		 SortByLowedge(const int axis) :m_axis(axis) {}
		bool operator()(const boundingbox* const bi1, const boundingbox* const bi2)const {
			return bi1->lowedge[m_axis] < bi2->lowedge[m_axis];
		}
	};

	//根据box右上大坐标的某一维度排序
	struct SortByHighedge {
	public:
		const int m_axis;
		 SortByHighedge(const int axis) :m_axis(axis) {}
		bool operator()(const boundingbox* const bi1, const boundingbox* const bi2)const {
			return bi1->highedge[m_axis] < bi2->highedge[m_axis];
		}
	};


	//根据距离排序
	struct SortByDistanceFromCenter {
	public:
		const boundingbox* const m_center;
		 SortByDistanceFromCenter(const boundingbox* const center) :m_center(center) {}
		bool operator()(const boundingbox* const bi1, const boundingbox* const bi2)const {
			return bi1->distanceFromCenter(*m_center) < bi2->distanceFromCenter(*m_center);
		}
	};

	
	/*
	//根据面积增加耗费排序，选择面积增加最小的
	struct SortByArea {
	public:
		const boundingbox* r;
		 SortByArea(const boundingbox * center) :r(center) {}
		bool operator()(const boundingbox* const bi1, const boundingbox* const bi2)const {
			boundingbox r1, r2;
			r1.assignbound(*r); r2.assignbound(*r);
			r1.stretch(bi1->lowedge, bi1->highedge); r2.stretch(bi2->lowedge, bi2->highedge);
			double r1area = r1.area(); double r2area = r2.area();
			double bi1area = bi1->area(); double bi2area = bi2->area();
			if (r1area - bi1area == r2area - bi2area) {
				return bi1area < bi2area;
			}
			else {
				return r1area -bi1area < r2area - bi2area;
			}
		}
	};*/

	//根据面积大小的排序，选择面积最小的
	struct SortByArea {
	public:
		explicit SortByArea(){}
		bool operator()(const boundingbox* const bi1, const boundingbox* const bi2)const {
			return bi1->area() >  bi2->area();
		}
	};
	/*
	//根据重叠面积大小排序，选择面积最小的
	struct SortByOverlapEnlargement {
	public:
		const boundingbox* const m_center;
		 SortByOverlapEnlargement(const boundingbox * const center) :m_center(center) {}
		bool operator()(const boundingbox*const bi1, const boundingbox* const bi2)const {
			double sum1, sum2;
			return bi1->overlap_area(*m_center) < bi2->overlap_area(*m_center);
		}
	};*/

};

//存放真实地理位置信息
struct leafbox {
	double taxi_1[2];
	double taxi_2[2];
	leafbox(double x,double y,double xx,double yy){
		taxi_1[0] = x; taxi_1[1] = y;
		taxi_2[0] = xx; taxi_2[1] = yy;
	}
	leafbox() {};
};