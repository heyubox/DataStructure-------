#include "rstartree.h"
#include<cassert>
#include<algorithm>
#include <fstream>
#include <iomanip>
#include <QDebug>
#include <cstring>

RStarTree::RStarTree() : m_root(NULL), m_size(0) {
    assert(1 <= min_child_item && min_child_item <= max_child_item / 2);
    memset(data_path, 0, sizeof(data_path));
    memset(dataInd_path, 0, sizeof(dataInd_path));
}

void RStarTree::setPath(const char *dr, const char *ir){
    strcpy(data_path, dr);
    strcpy(dataInd_path, ir);
}

void RStarTree::Insert(int ID[], int* time, const boundingbox& bound, double x, double y,double xx,double yy)
{
    leafbox databox(x, y, xx, yy);
    Leaf* newLeaf = new Leaf(time, ID, databox);
    newLeaf->assignbound(bound);

    if (!m_root) {
        m_root = new Node();
        m_root->hasLeaves = true;

        m_root->items.push_back(newLeaf);
        m_root->assignbound(bound);

    }
    else
        InsertInternal(newLeaf, m_root);
    m_size += 1;
}

RStarTree::Node* RStarTree::InsertInternal(Leaf* leaf, Node* node, bool firstInsert) {
    node->stretch(leaf->lowedge, leaf->highedge);
    if (node->hasLeaves) {
        node->items.push_back(leaf);
    }
    else {
        Node* temp_node = InsertInternal(leaf, ChooseSubtree(node, leaf), firstInsert);

        if (!temp_node)
            return NULL;
        node->items.push_back(temp_node);
    }

    if (node->items.size() > max_child_item) {
        return OverflowTreatment(node, firstInsert);
    }

    return NULL;
}

/*
RStarTree::Node* RStarTree::ChooseSubtree(Node* node, const boundingbox* bound) {
    if (static_cast<Node*>(node->items[0])->hasLeaves) {
        if ( node->items.size() > RTREE_CHOOSE_SUBTREE_P) {

            std::partial_sort(node->items.begin(), node->items.begin() + RTREE_CHOOSE_SUBTREE_P, node->items.end(),
                boundingbox::SortByAreaEnlargement(bound));

            return static_cast<Node*>(*std::min_element(node->items.begin(), node->items.begin() + RTREE_CHOOSE_SUBTREE_P,
                boundingbox::SortByOverlapEnlargement(bound)));
        }
        return static_cast<Node*>(*std::min_element(node->items.begin(), node->items.end(),
            boundingbox::SortByOverlapEnlargement(bound)));
    }

    return static_cast<Node*>(*std::min_element(node->items.begin(), node->items.end(),
        boundingbox::SortByAreaEnlargement(bound)));
}*/

RStarTree::Node* RStarTree::ChooseSubtree(Node* node, const boundingbox* bound) {
    return static_cast<Node*>(*std::min_element(node->items.begin(), node->items.end(),
            boundingbox::SortByArea()));
}


RStarTree::Node* RStarTree::OverflowTreatment(Node* level, bool firstInsert) {
    if (level != m_root && firstInsert) {

        Reinsert(level);
        return NULL;
    }

    Node* splitItem = Split(level);

    if (level == m_root) {
        Node* newRoot = new Node();
        newRoot->hasLeaves = false;

        newRoot->items.reserve(min_child_item);
        newRoot->items.push_back(m_root);
        newRoot->items.push_back(splitItem);

        newRoot->reset();
        auto i = newRoot->items.size();
        for (int stretch_i = 0; stretch_i < i; stretch_i++) {
            newRoot->stretch(newRoot->items[stretch_i]->lowedge, newRoot->items[stretch_i]->highedge);
        }

        m_root = newRoot;
        return NULL;
    }

    return splitItem;
}

int RStarTree::ChooseSplitAxix(Node* splitnode) {
    int split_axis = 0;
    double marginR1R2= std::numeric_limits<double>::max();

    boundingbox R1, R2;
    double margin;
    for (int axis = 0; axis < 2 - 1; axis++) {
        margin = 0;


        for (int group = 1; group <= 2; group++) {
            if(group==1)std::sort(splitnode->items.begin(), splitnode->items.end(), boundingbox::SortByLowedge(axis));
            else std::sort(splitnode->items.begin(), splitnode->items.end(), boundingbox::SortByHighedge(axis));
            for (int k = 1; k < max_child_item - min_child_item * 2 + 2; k++)
            {
                R1.reset();
                for (int i = 0; i <= min_child_item + k; i++) {
                    R1.stretch(splitnode->items[i]->lowedge, splitnode->items[i]->highedge);
                }

                R2.reset();
                for (int i = min_child_item + k + 1; i < splitnode->items.size(); i++)
                    R2.stretch(splitnode->items[i]->lowedge, splitnode->items[i]->highedge);

                margin += R1.edgeDeltas() + R2.edgeDeltas();
            }
        }
        if (margin < marginR1R2) {
            marginR1R2 = margin; split_axis = axis;
        }

    }
    return split_axis;
}

RStarTree::Node* RStarTree::Split(Node* node) {
    double Barea= std::numeric_limits<double>::max(),Boverlaps = std::numeric_limits<double>::max();
    boundingbox R1, R2;
    int Bsplit_axis = ChooseSplitAxix(node);
    int Bgroup, split_index;
    for (int group = 1; group <= 2; group++) {
        if (group == 1)std::sort(node->items.begin(), node->items.end(), boundingbox::SortByLowedge(Bsplit_axis));
        else std::sort(node->items.begin(), node->items.end(), boundingbox::SortByHighedge(Bsplit_axis));
        //double area = 0;
        for (int k = 1; k < max_child_item - min_child_item * 2 + 2; k++)
        {

            double overlap = 0;
            R1.reset();
            for (int i = 0; i <= min_child_item + k; i++) {
                R1.stretch(node->items[i]->lowedge, node->items[i]->highedge);
            }

            R2.reset();
            for (int i = min_child_item + k + 1; i < node->items.size(); i++)
                R2.stretch(node->items[i]->lowedge, node->items[i]->highedge);
            overlap = R1.overlap(R2);

            if (overlap < Boverlaps) {
                Bgroup = group;
                split_index = min_child_item + k;
            }
        }
    }
    if (Bgroup == 1) {
        std::sort(node->items.begin(), node->items.end(), boundingbox::SortByLowedge(Bsplit_axis));
    }
    Node* newNode = new Node();
    newNode->hasLeaves = node->hasLeaves;
    newNode->items.assign(node->items.begin() + split_index, node->items.end());
    node->items.erase(node->items.begin() + split_index, node->items.end());

    node->reset();
    for (int i = 0; i < node->items.size(); i++) {
        node->stretch(node->items[i]->lowedge, node->items[i]->highedge);
    }
    newNode->reset();
    for (int i = 0; i < newNode->items.size(); i++) {
        newNode->stretch(newNode->items[i]->lowedge, newNode->items[i]->highedge);
    }
    return newNode;
}

void RStarTree::Reinsert(Node* node) {

    std::vector<boundingbox*>remove_items;
    const int n_items = node->items.size();
    const int p = (std::size_t)((double)n_items * RTREE_REINSERT_P) > 0 ? (std::size_t)((double)n_items * RTREE_REINSERT_P) : 1;

    assert(n_items == max_child_item + 1);

    std::partial_sort(node->items.begin(), node->items.end() - p, node->items.end(),
        boundingbox::SortByDistanceFromCenter(node));

    remove_items.assign(node->items.end() - p, node->items.end());
    node->items.erase(node->items.end() - p, node->items.end());

    node->reset();
    for (int i = 0; i < node->items.size(); i++) {
        node->stretch(node->items[i]->lowedge, node->items[i]->highedge);
    }

    for (std::vector<boundingbox*>::iterator it = remove_items.begin(); it != remove_items.end(); it++) {
        InsertInternal(static_cast<Leaf*>(*it), m_root, false);
    }
}

void RStarTree::SetArea(double* low, double* high) {
    lbound[0] = low[0]; lbound[1] = low[1];
    hbound[0] = high[0]; hbound[1] = high[1];
}

void RStarTree::SetTime(int tb, int te) {
    t1 = tb;
    t2 = te;
}

int RStarTree::TimeFit(int* time) {
    int flag = 0;
    if (time[0] >= t1&& time[0] < t2)
        flag++;
    if ( time[1]>= t1&& time[1] < t2)
        flag++;
    return flag;
}

bool RStarTree::FindEntry(Node* root, double* lowedge, double* highedge, int tbegin, int tend) {
    found.clear();
    SetArea(lowedge, highedge);
    SetTime(tbegin, tend);
    Findleaf(root);
    return true;
}

bool RStarTree::CoverFind(Node* root) {
    if (root == NULL) {
        return false;
    }
    if (root->hasLeaves == false) {

        for (int i = 0; i < root->items.size(); i++) {
                CoverFind(static_cast<Node*>(root->items[i]));
        }
    }
    else if (root->hasLeaves == true) {

        for (int i = 0; i < root->items.size(); i++) {
            Leaf* leafnode = static_cast<Leaf*>(root->items[i]);
            check_pushback(leafnode,true);
        }
    }
    return true;
}

bool RStarTree::Findleaf(Node* root) {
    if (root == NULL) {
        return false;
    }
    if (root->hasLeaves == false) {

        for (int i = 0; i < root->items.size(); i++) {
            if (root->Cover(root->items[i], lbound, hbound)) {
                CoverFind(static_cast<Node*>(root->items[i]));
            }
            else if (root->Overlap(root->items[i], lbound, hbound))
            {
                Findleaf(static_cast<Node*>(root->items[i]));
            }
        }
    }
    else if (root->hasLeaves == true) {
        int time, id;
        double x, y;
        for (int i = 0; i < root->items.size(); i++) {
            Leaf* leafnode = static_cast<Leaf*>(root->items[i]);
            int timefit = TimeFit(leafnode->getTime());
            if (timefit==2 &&leafnode->Cover(leafnode, lbound, hbound)) {
                leafnode->getTaxi1(time, id, x, y);
                pushback_cor(id, time,x,y);
                leafnode->getTaxi2(time, id, x, y);
                pushback_cor(id, time, x, y);
            }
            else if (timefit==1) {
                check_pushback(leafnode);
            }
        }
    }
    return true;
}

bool RStarTree::check_pushback(Leaf* node,bool check_only_time) {
    int time, id;
    double x, y;
    node->getTaxi1(time, id, x, y);
    if (time >= t1&& time < t2 ) {
        pushback_cor(id, time, x, y);
    }
    node->getTaxi2(time, id, x, y);
    if (time >= t1&& time < t2 ) {
        pushback_cor(id, time, x, y);
    }
    return true;
}

bool RStarTree::check_pushback(Leaf* node) {
    int time, id;
    double x, y;
    node->getTaxi1(time, id, x, y);
    if (time>t1 && time<t2 && checkcor(x, y)) {
        pushback_cor(id, time, x, y);
    }
    node->getTaxi2(time, id, x, y);
    if (time > t1&& time < t2 && checkcor(x, y)) {
        pushback_cor(id, time, x, y);
    }
    return true;
}

bool RStarTree::checkcor(double x, double y) {
    return lbound[0]<x && hbound[0]>x && lbound[1]<y && hbound[1]>y;
}

void RStarTree::pushback_cor(int ID, int time, double x, double y) {
    found.push_back(foundNode(ID, time, x, y));
}

void RStarTree::foundToJsonp(const char* path){

    std::ofstream fout(path);
    fout.setf(std::ios::fixed);
    fout.precision(5);
    fout << "callback({\"data\":[";

    std::vector<foundNode>::iterator iter;
    iter = found.begin();
    fout << "[" << iter->lng << "," << iter->lat << "]";
    for(iter = found.begin() + 1; iter != found.end(); ++iter){
        fout << ",[" << iter->lng << "," << iter->lat << "]";
    }
    fout << "]})";
    fout.close();
}

void RStarTree::locateBeg(int &time, int beg, int end, int &ind, bool &ok){
    if(end < beg)
        return ;
    int mid = (beg + end) / 2;
    int t = trafficVolume[mid].time;
    if(t == time){
        int j = mid - 1;
        while(j >= 0 && trafficVolume[j].time == time)
            j--;
        ind = j + 1;
        ok = true;
        return;
    }else if(time < t){
        locateBeg(time, beg, mid - 1, ind, ok);
    }else{
        locateBeg(time, mid + 1, end, ind, ok);
    }
}

void RStarTree::locateEnd(int &time, int beg, int end, int &ind, bool &ok){
    if(end < beg)
        return ;
    int mid = (beg + end) / 2;
    int t = trafficVolume[mid].time;
    if(t == time){
        int j = mid + 1;
        while(j < trafficVolume.size() && trafficVolume[j].time == time)
            j++;
        ind = j - 1;
        ok = true;
        return;
    }else if(time < t){
        locateEnd(time, beg, mid - 1, ind, ok);
    }else{
        locateEnd(time, mid + 1, end, ind, ok);
    }
}

int RStarTree::trafficVolume5(Node* root, double p1_low[2], double p1_high[2], double p2_low[2], double p2_high[2], int tbegin, int tend) {
    clock_t t1 = clock();

    volumeTime.clear();

    AdjList taxiList[10358];
    bool mark[10358] = {false};
    int id, i, j;

    qDebug() << "Start search in area1...";

    FindEntry(root, p1_low, p1_high, tbegin, tend);
    for (i = 0; i < found.size(); i++) {
        id = found[i].ID;
        mark[id] = true;
        taxiList[id].taxi.push_back(plcNode('\x00',found[i].time));
    }

    found.clear();

    qDebug() << "Start search in area2...";
    FindEntry(root, p2_low, p2_high, tbegin, tend);
    for (i = 0; i < found.size(); i++) {
        id = found[i].ID;
        if(!mark[id])
            continue;
        taxiList[id].taxi.push_back(plcNode('\xff',found[i].time));
    }

    qDebug() << "Counting traffic volume between area1 and area2...";

    for(i = 1; i <= 10357; i++){
        if(!mark[i])
            continue;
        std::sort(taxiList[i].taxi.begin(), taxiList[i].taxi.end(),
                  [](const plcNode &n1, const plcNode &n2){return n1.time < n2.time;} );
        for(j = 0; j < taxiList[i].taxi.size() - 1; j++){
            if(taxiList[i].taxi[j].area != taxiList[i].taxi[j+1].area)
                volumeTime.push_back(taxiList[i].taxi[j+1].time);
        }
    }
    std::sort(volumeTime.begin(), volumeTime.end());
    qDebug() << "Total Counted: " << volumeTime.size();
    qDebug() << "Insertion cost " << (clock() - t1) * 1.0 / CLOCKS_PER_SEC << "s in total";
    return volumeTime.size();
}

int RStarTree::trafficVolume6(Node* root, double p1_low[2], double p1_high[2], int tbegin, int tend){
    clock_t t1 = clock();

    volumeTime.clear();
    int len = 0;
    int indexTbl[10359];
    int word = sizeof(int) + sizeof(int) + sizeof(double) + sizeof(double);

    FILE *fp = NULL;
    fp = fopen(dataInd_path, "rb");
    assert(fp != NULL);
    for(int i = 0; i <= 10358; ++i)
        fread(indexTbl+i, sizeof(int), 1, fp);
    fclose(fp);   fp = NULL;
    assert(indexTbl[10358] == TUPLE_AMOUNT);

    found.clear();   trafficVolume.clear();  //locus.clear();
    FindEntry(m_root, p1_low, p1_high, tbegin, tend);
    std::sort(found.begin(), found.end(), [](const foundNode &a, const foundNode &b){
        if(a.ID < b.ID){return true;}
        else if(a.ID == b.ID){return a.time < b.time;}
        else {return false;}
    });
    len = found.size();

    fp = fopen(data_path, "rb");
    assert(fp != NULL);

    int now_id = 0, begiter = 0, enditer = 0, sizes = 0;
    bool ok = false, inArea = true;

    for(int i = 0; i < len; i++){

        if(now_id != found[i].ID){
            foundNode tmp;
            now_id = found[i].ID;
            trafficVolume.clear();
            fseek(fp, (long)(indexTbl[now_id] * word), SEEK_SET);
            int num = indexTbl[now_id + 1] - indexTbl[now_id];
            while(num--){
                fread(&tmp.ID, sizeof(int), 1, fp);
                fread(&tmp.time, sizeof(int), 1, fp);
                fread(&tmp.lng, sizeof(double), 1, fp);
                fread(&tmp.lat, sizeof(double), 1, fp);
                trafficVolume.push_back(tmp);
            }
            sizes = trafficVolume.size();

            locateBeg(found[i].time, 0, sizes - 1, begiter, ok);
            assert(ok);
            if(begiter && (trafficVolume[begiter-1].lng < p1_low[0] || trafficVolume[begiter-1].lng > p1_high[0]
                           || trafficVolume[begiter-1].lat < p1_low[1] || trafficVolume[begiter-1].lat > p1_high[1])){
                 volumeTime.push_back(trafficVolume[begiter-1].time);
            }
            while(i < len && found[i].ID == now_id)
                i++;
            i -= 1;   ok = false;
            locateEnd(found[i].time, 0, sizes-1, enditer, ok);
            assert(ok);
            if(enditer != sizes - 1 && trafficVolume[enditer+1].time < tend && (trafficVolume[enditer+1].lng < p1_low[0] ||
                trafficVolume[enditer+1].lng > p1_high[0] || trafficVolume[enditer+1].lat < p1_low[1] ||
                trafficVolume[enditer+1].lat > p1_high[1])){
                 volumeTime.push_back(trafficVolume[enditer+1].time);
            }
        }

        for(int j = begiter;  j < enditer; j++){
            if((trafficVolume[j].lng > p1_low[0] && trafficVolume[j].lng < p1_high[0]
                        && trafficVolume[j].lat > p1_low[1] && trafficVolume[j].lat < p1_high[1]) ^ inArea){
                volumeTime.push_back(trafficVolume[j].time);
                inArea = !inArea;
            }
        }
    }

    qDebug() << "Total count done 6:" << volumeTime.size();

    qDebug() << "Insertion cost " << (clock() - t1) * 1.0 / CLOCKS_PER_SEC << "s in total";
    found.clear();   trafficVolume.clear();
    found.shrink_to_fit();   trafficVolume.shrink_to_fit();
    fclose(fp); fp = NULL;

    std::sort(volumeTime.begin(), volumeTime.end());

    return volumeTime.size();
}

int RStarTree::Route(Node* root, double p1_low[2], double p1_high[2], double p2_low[2], double p2_high[2], const char *rt){

    int mark[10358][2];

    int s, len, begiter, enditer, tol = 0;
    int indexTbl[10359];
    int word = sizeof(int) + sizeof(int) + sizeof(double) + sizeof(double);

    FILE* fp = NULL, *wp = NULL;
    fp = fopen(dataInd_path, "rb");
    assert(fp != NULL);
    for (int i = 0; i <= 10358; ++i)
       fread(indexTbl + i, sizeof(int), 1, fp);
    fclose(fp);
    fp = NULL;

    for(int i = 1; i <= 10357; ++i){
        mark[i][0] = 9000000;
        mark[i][1] = -1;
    }

    qDebug() << "Start search in area1...";
    FindEntry(root, p1_low, p1_high, 2000000, 9000000);
    len = found.size();
    for(int i = 0; i < len; ++i){
        s = found[i].ID;
        mark[s][0] = std::min(mark[s][0], found[i].time);
    }
    found.clear();
    qDebug() << "Start search in area2...";
    FindEntry(root, p2_low, p2_high, 2000000, 9000000);
    len = found.size();
    for(int i = 0; i < len; ++i){
        s = found[i].ID;
        if(mark[s][0] == 9000000) continue;
        mark[s][1] = std::max(mark[s][1], found[i].time);
    }
    found.clear();

    qDebug() << "Counting traffic volume between area1 and area2...";
    fp = fopen(data_path, "rb");
    wp = fopen(rt, "w");
    assert(fp != NULL);
    assert(wp != NULL);
    foundNode tmp;
    bool ok = false, flag = false;

    fprintf(wp, "callback({\"data\":[");

    for(int i = 1; i <= 10357; ++i){
        if(mark[i][0] > mark[i][1]) continue;

        trafficVolume.clear();
        fseek(fp, (long)(indexTbl[i] * word), SEEK_SET);
        int num = indexTbl[i + 1] - indexTbl[i];
        while(num--){
            fread(&tmp.ID, sizeof(int), 1, fp);
            fread(&tmp.time, sizeof(int), 1, fp);
            fread(&tmp.lng, sizeof(double), 1, fp);
            fread(&tmp.lat, sizeof(double), 1, fp);
            trafficVolume.push_back(tmp);
        }
        len = trafficVolume.size();

        locateBeg(mark[i][0], 0, len, begiter, ok); assert(ok); ok = false;
        locateEnd(mark[i][1], 0, len - 1, enditer, ok); assert(ok); ok = false;
        tol += enditer - begiter;

        for(int j = begiter; j <= enditer; j++){
            if(flag)
                fprintf(wp, ",{\"lng\":%.5lf,\"lat\":%.5lf,\"count\":1}", trafficVolume[j].lng, trafficVolume[j].lat);
            else{
                fprintf(wp, "{\"lng\":%.5lf,\"lat\":%.5lf,\"count\":1}", trafficVolume[j].lng, trafficVolume[j].lat);
                flag = true;
            }
        }
    }

    fprintf(wp,"]})");
    fclose(fp);  fclose(wp);
    trafficVolume.clear();
    return tol;
}
