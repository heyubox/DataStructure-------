#ifndef RSTARTREE_H
#define RSTARTREE_H

#include"boundingbox.h"
#include<vector>

#define RTREE_REINSERT_P 0.30
#define RTREE_CHOOSE_SUBTREE_P 32
#define min_child_item 32
#define max_child_item 64
#define FILE_TIME_BEG 2000000
#define FILE_TIME_END 9000000
#define TUPLE_AMOUNT 16196704

struct leafbox {
    double taxi_1[2];
    double taxi_2[2];
    leafbox(){};
    leafbox(double x,double y,double xx,double yy){
        taxi_1[0] = x; taxi_1[1] = y;
        taxi_2[0] = xx; taxi_2[1] = yy;
    }
};

class RStarNode :public boundingbox {
public:
    std::vector<boundingbox*> items;
    bool hasLeaves;
};

class RStarLeaf : public boundingbox {
    bool hasLeaves;
    int time[2];
    int name[2];
    leafbox taxipos;
public:
     RStarLeaf(){
        hasLeaves = true;
        time[0] = 0; time[1] = 0;
        name[0] = 0; name[1] = 0;
    }
    RStarLeaf(int* time, int* id, leafbox taxibox) {
        taxipos = taxibox;
        name[0] = id[0]; name[1] = id[1];
        this->time[0] = time[0]; this->time[1] = time[1];
    }
    int* getTime() {
        return time;
    }
    void getTaxi1(int& time, int& id, double& x, double& y) {
        time = this->time[0]; id = name[0]; x = taxipos.taxi_1[0]; y = taxipos.taxi_1[1];
    }
    void getTaxi2(int& time, int& id, double& x, double& y) {
        time = this->time[1]; id = name[1]; x = taxipos.taxi_2[0]; y = taxipos.taxi_2[1];
    }
};

struct foundNode {
    int ID;
    int time;
    double lng;
    double lat;
    foundNode(int &id, int &t, double &x, double &y):ID(id), time(t), lng(x), lat(y){}
    foundNode(){};
};


struct plcNode{
    unsigned char area;
    int time;
    plcNode(char ch, int t){area = ch;  time = t;}
    plcNode(){}
};


struct AdjList{
    std::vector<plcNode> taxi;
};



class RStarTree {
    int t1, t2;
    double lbound[2];
    double hbound[2];

public:
    typedef RStarLeaf Leaf;
    typedef RStarNode Node;

    RStarTree();
    void setPath(const char *, const char *);
    void Insert(int ID[], int* time, const boundingbox& bound, double x, double y,double xx,double yy);
    Node* InsertInternal(Leaf* leaf, Node* node, bool firstInsert = true);
    Node *ChooseSubtree(Node* node, const boundingbox* bound);
    Node* OverflowTreatment(Node* level, bool firstInsert);
    int ChooseSplitAxix(Node* splitnode);
    Node* Split(Node* node);
    void Reinsert(Node* node);

    void SetArea(double* low, double* high);
    void SetTime(int tb, int te);
    int TimeFit(int* time);
    bool FindEntry(Node* root, double* lowedge, double* highedge, int tbegin, int tend);
    bool CoverFind(Node* root);
    bool Findleaf(Node* root);
    bool check_pushback(Leaf* node,bool check_only_time);
    bool check_pushback(Leaf* node);
    bool checkcor(double x, double y);
    void pushback_cor(int ID, int time, double x, double y);


    void locateBeg(int &time, int beg, int end, int &ind, bool &ok);
    void locateEnd(int &time, int beg, int end, int &ind, bool &ok);
    int trafficVolume5(Node* root, double p1_low[2], double p1_high[2], double p2_low[2], double p2_high[2],
                        int tbegin = FILE_TIME_BEG, int tend = FILE_TIME_END);
    int trafficVolume6(Node* root, double p1_low[2], double p1_high[2], int tbegin = FILE_TIME_BEG, int tend = FILE_TIME_END);
    int Route(Node* root, double p1_low[2], double p1_high[2], double p2_low[2], double p2_high[2], const char *);



    void foundToJsonp(const char*);

public:
    Node *m_root;
    int m_size;
    char data_path[200];
    char dataInd_path[200];
    std::vector<foundNode> found;
    std::vector<foundNode> trafficVolume;
    std::vector<int> volumeTime;
};

#endif // RSTARTREE_H
