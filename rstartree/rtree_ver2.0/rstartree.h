#pragma once
#include<list>
#include<vector>
#include<iostream>
#include<cassert>
#include<algorithm>
#include<functional>
#include<iterator>  
#include<map>
#include<set>
#include<queue>
#include"boundingbox.h"

#define RTREE_REINSERT_P 0.30
#define RTREE_CHOOSE_SUBTREE_P 32
#define min_child_item 32
#define max_child_item 64

int t_grow(int tbegin,int tend,int growth=60) {
	if (growth > 3600)growth = growth % 60 + (int(growth/60)%60)*100+int(growth / 3600) * 10000;
	else if(growth > 60)growth = growth % 60 + int(growth / 60) * 100;
	tbegin += growth;
	if (tbegin % 100 + growth % 100 > 60)tbegin += 100-60 + growth;
	if (tbegin % 10000 > 6000)tbegin += 10000-6000;
	if (tbegin % 1000000 > 230000)tbegin += 1000000 -240000;
	return tbegin>tend?tend:tbegin;
}

class RStarNode :public boundingbox {
public:
	std::vector<boundingbox*> items;
	bool hasLeaves;
};

class RStarLeaf : public boundingbox {
	bool hasLeaves;
	int leaf[2];
	int name[2];
	leafbox taxipos;
public:
	RStarLeaf() {
		hasLeaves = true;
		leaf[2] = { 0 };
		name[2] = { 0 };
	}
	RStarLeaf(int* time, int* id, leafbox taxibox) {
		taxipos = taxibox;
		name[0] = id[0]; name[1] = id[1];
		leaf[0] = time[0]; leaf[1] = time[1];
	}
	int* getTime() {
		return leaf;
	}
	void getTaxi1(int& time, int& id, double& x, double& y) {
		time = leaf[0]; id = name[0]; x = taxipos.taxi_1[0]; y = taxipos.taxi_1[1];
	}
	void getTaxi2(int& time, int& id, double& x, double& y) {
		time = leaf[1]; id = name[1]; x = taxipos.taxi_2[0]; y = taxipos.taxi_2[1];
	}
};


struct foundNode {						//ԭ���汾��found_vector�����׸�Сvector������ֱ������һ���ṹ�����������Сvector
	int ID;
	int time;
	double lng;
	double lat;
	foundNode(int &id, int &t, double &x, double &y):ID(id), time(t), lng(x), lat(y){}
};

class RStarTree {
	//��ʱ���box��Ϊ��Ա����С�������ε�ϵͳ������
	int t1, t2;//ʱ���ѯ��Χ
	double lbound[2];
	double hbound[2];
public:
	typedef boundingbox boundingbox;
	typedef foundNode foundNode;
	typedef RStarLeaf Leaf;
	typedef RStarNode Node;


	RStarTree() : m_root(NULL), m_size(0) {
		assert(1 <= min_child_item && min_child_item <= max_child_item / 2);
	}

	// @x,y xx,yy ���⳵λ����Ϣ
	// @bound �淶������С�������
	// @id taxi1��taxi2��id
	// @time taxi1��taxi2��time
	void Insert(int ID[], int* time, const boundingbox& bound,double x,double y,double xx,double yy) {
		leafbox databox(x, y, xx, yy);
		Leaf* newLeaf = new Leaf(time,ID,databox);
		newLeaf->assignbound(bound);
		//std::cout << "time is " << time << std::endl;

		if (!m_root) {
			m_root = new Node();
			m_root->hasLeaves = true;

			//reserve memory
			//m_root Ϊ�� Ϊʲô����reserve
			//m_root->items.reserve(min_child_item);
			//�����vector������»��������
			//����Ҳ֪�������ܷŽ�ȥ�ġ�vector�������ڴ�ġ����仰˵��ÿ��Ԫ�صĴ�С����ʱ�͹̶��ˡ�
			//����Ԫ�ض�����Ȼ����Ҫ����ʱ��vector����ûλ�÷�������Ԫ�صġ�
			//��ʹ����ת�����ͷŽ�ȥ����Ҳ�Ǳ��ü��˵Ķ��󣨶��ֻ��ʣ�������еĻ��ಿ�֣���������Ƹ�lz��Ҫ����ȫ�������ġ�
			//Ҫ����ָ��Ͳ���������������⣬��Ϊ����������ָ�뻹�ǻ���ָ�룬
			//���ǵĴ�С��һ���ģ�������Ϊ��̬������ָ���ǿ���ָ���������ġ����������������ϻ��Ǵ�С�϶�û���⡣
			m_root->items.push_back(newLeaf);
			m_root->assignbound(bound);

		}
		else
			InsertInternal(newLeaf, m_root);
		m_size += 1;

	}



	//��������ڸ�����ڵ�
	//����InsertInternalѰ�Ҳ����
	//�����Ժ������еĵݹ�Ѱ��
	Node* InsertInternal(Leaf* leaf, Node* node, bool firstInsert = true) {
		node->stretch(leaf->lowedge, leaf->highedge);
		if (node->hasLeaves) {
			node->items.push_back(leaf);
		}
		else {
			Node* temp_node = InsertInternal(leaf, ChooseSubtree(node, leaf), firstInsert);
			//leaf is the newnode to be inserted.give the current node(under searching )and newnode to be inserted 
			if (!temp_node)
				return NULL;
			node->items.push_back(temp_node);
		}

		if (node->items.size() > max_child_item) {
			return OverflowTreatment(node, firstInsert);
		}

		return NULL;
	}

	//ѡ���ʺϵĲ����
	//���ز���ýڵ��������������ص������С�Ľ��
	Node* ChooseSubtree(Node* node, const boundingbox* bound) {
		//If the child pointers point ot leaves
		//[ȷ������Ƶ���С�����㷨]
		//��N�еľ������򣬰�����Ҫ���������ݵľ��������չ�����������С�
		//	��A���������ǰp����Ŀ������A�е���Ŀ���д��������洦��N�е�
		//	������Ŀ��ѡ���������Ҫ��С������չ����Ŀ�����в�ѯ��������������
		//	������ѡ����չ���ν�С�Ľڵ������ѯ��
		//	�ڶ�ά������£���p��Ϊ32ͨ���Ͳ��ή�Ͳ�ѯ���������ܡ�
		if (static_cast<Node*>(node->items[0])->hasLeaves) {
			//node�к���ָ��ָ��Ҷ�ڵ㣬��С���Ǵ���
			if ( node->items.size() > RTREE_CHOOSE_SUBTREE_P) {
				//ǰ32�Ľ�
				std::partial_sort(node->items.begin(), node->items.begin() + RTREE_CHOOSE_SUBTREE_P, node->items.end(),
					boundingbox::SortByAreaEnlargement(bound));

				return static_cast<Node*>(*std::min_element(node->items.begin(), node->items.begin() + RTREE_CHOOSE_SUBTREE_P,
					boundingbox::SortByOverlapEnlargement(bound)));
			}
			return static_cast<Node*>(*std::min_element(node->items.begin(), node->items.end(),
				boundingbox::SortByOverlapEnlargement(bound)));
		}
		//node�к���ָ�벻ָ��Ҷ�ڵ㣬��С�����������
		return static_cast<Node*>(*std::min_element(node->items.begin(), node->items.end(),
			boundingbox::SortByAreaEnlargement(bound)));
	}


	Node* OverflowTreatment(Node* level, bool firstInsert) {
		if (level != m_root && firstInsert) {
			//ǿ�����²���
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
	
	int ChooseSplitAxix(Node* splitnode) {
		int split_axis = 0;// ���ѵ�γ��0��1 ��x��y��,���ά�ȳ�ʼΪ0
		double marginR1R2= std::numeric_limits<double>::max();
		//max dimension = 1
		boundingbox R1, R2;
		double margin;
		for (int axis = 0; axis < 2 - 1; axis++) {
			margin = 0;
			
			
			for (int group = 1; group <= 2; group++) {//�����飬��ǰά����������½�����
				if(group==1)std::sort(splitnode->items.begin(), splitnode->items.end(), boundingbox::SortByLowedge(axis));
				else std::sort(splitnode->items.begin(), splitnode->items.end(), boundingbox::SortByHighedge(axis));
				for (int k = 1; k < max_child_item - min_child_item * 2 + 2; k++) //������� 33/32 32/33
				{
					double area = 0;

					R1.reset();
					for (int i = 0; i <= min_child_item + k; i++) {
						R1.stretch(splitnode->items[i]->lowedge, splitnode->items[i]->highedge);
					}

					R2.reset();
					for (int i = min_child_item + k + 1; i < splitnode->items.size(); i++)
						R2.stretch(splitnode->items[i]->lowedge, splitnode->items[i]->highedge);

					margin += R1.edgeDeltas() + R2.edgeDeltas();
					
					//area += R1.area() + R2.area();
					//overlap = R1.overlap(R2);
				}
			}
			if (margin < marginR1R2) {
				marginR1R2 = margin; split_axis = axis;//�õ���ѷ���ά��
			}
			
		}
		return split_axis;
	}

	Node* Split(Node* node) {
		double Barea= std::numeric_limits<double>::max(),Boverlaps = std::numeric_limits<double>::max();
		boundingbox R1, R2;
		int Bsplit_axis = ChooseSplitAxix(node);
		int Bgroup, split_index;
		for (int group = 1; group <= 2; group++) {//�����飬��ǰά����������½�����
			if (group == 1)std::sort(node->items.begin(), node->items.end(), boundingbox::SortByLowedge(Bsplit_axis));
			else std::sort(node->items.begin(), node->items.end(), boundingbox::SortByHighedge(Bsplit_axis));
			//double area = 0;
			for (int k = 1; k < max_child_item - min_child_item * 2 + 2; k++) //������� 33/32 32/33
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
				//area += R1.area() + R2.area();
				if (overlap < Boverlaps) {
					Bgroup = group;
					split_index = min_child_item + k;
				}
				else if(overlap == Boverlaps)
				{
					cout << "opps";
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
	
	/* Abolished  
	//����װ���µĽ��
	Node* Split(Node* node) {
		//std::cout << "doing Split" << std::endl;
		Node* newNode = new Node();
		newNode->hasLeaves = node->hasLeaves;
		const std::size_t n_items = node->items.size();
		const std::size_t distribution_count = n_items - 2 * min_child_item + 1;
		std::size_t split_axis = 2 + 1, split_edge = 0, split_index = 0;//2 is dimensions
		int split_margin = 0;

		boundingbox R1, R2;

		assert(n_items == max_child_item + 1);
		assert(distribution_count > 0);
		assert(min_child_item + distribution_count - 1 <= n_items);
		//std::cout << "assert done" << std::endl;

		for (int axis = 0; axis < 2; axis++) {
			int margin = 0;
			double overlap = 0, dist_area, dist_overlap;
			int dist_edge = 0, dist_index = 0;
			dist_area = dist_overlap = std::numeric_limits<double>::max();

			for (int edge = 0; edge < 2; edge++) {
				//
				if (edge == 0) {
					std::sort(node->items.begin(), node->items.end(), boundingbox::SortByHighedge(axis));
				}
				else
					std::sort(node->items.begin(), node->items.end(), boundingbox::SortByHighedge(axis));
				//std::cout << "pause" << std::endl;
				for (int k = 0; k < distribution_count; k++) {
					double area = 0;

					R1.reset();
					for (int i = 0; i <= min_child_item + k; i++) {
						R1.stretch(node->items[i]->lowedge, node->items[i]->highedge);
					}

					R2.reset();
					for (int i = min_child_item + k + 1; i < node->items.size(); i++)
						R2.stretch(node->items[i]->lowedge, node->items[i]->highedge);

					margin += R1.edgeDeltas() + R2.edgeDeltas();
					area += R1.area() + R2.area();
					overlap = R1.overlap(R2);

					if (overlap < dist_overlap || (overlap == dist_overlap && area < dist_area)) {
						dist_edge = edge;
						dist_index = min_child_item + k;
						dist_overlap = overlap;
						dist_area = area;
					}
				}
			}
			if (split_axis == 2 + 1 || split_margin > margin) {
				split_axis = axis;
				split_margin = margin;
				split_edge = dist_edge;
				split_index = dist_index;

			}
		}
		if (split_edge == 0)
			std::sort(node->items.begin(), node->items.end(), boundingbox::SortByLowedge(split_axis));
		else if (split_axis != 2 - 1)
			std::sort(node->items.begin(), node->items.end(), boundingbox::SortByHighedge(split_axis));

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


	}*/

	//����ǿ�Ʋ����Լ�С�ص�
	void Reinsert(Node* node) {
		//std::cout << "doing Reinsert" << std::endl;
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



	//this is for testing static_cast
	void visit(Node* node) {
		if (node->hasLeaves == false) {
			Node* tempNode = static_cast<Node*> (node->items[0]);
			visit(tempNode);
		}
		else {
			Leaf* templeaf = static_cast<Leaf*>(node->items[0]);
			cout << templeaf->getTime();
		}
		
	}

	void SetArea(double* low, double* high) {
		lbound[0] = low[0]; lbound[1] = low[1];
		hbound[0] = high[0]; hbound[1] = high[1];
	}
	//�趨��ѯʱ��
	void SetTime(int tb, int te) {
		t1 = tb;
		t2 = te;
	}
	//�����ڲ�ѯ��Χ�ڵ�Ŀ�������0��1��2��
	int TimeFit(int* time) {
		int flag = 0;
		if (time[0] > t1&& time[0] < t2)
			flag++;
		if ( time[1]> t1&& time[1] < t2)
			flag++;
		return flag;
	}

	//��Ϊ��ѯ����ʽ���
	bool FindEntry(Node* root, double* lowedge, double* highedge, int tbegin, int tend) {
		SetArea(lowedge, highedge);//���ô���Ѱ��box
		SetTime(tbegin, tend);//���ô���Ѱ��ʱ���
		Findleaf(root);
		return true;
	}

	bool CoverFind(Node* root) {
		if (root == NULL) {
			return false;
		}
		if (root->hasLeaves == false) {
			//not a leaf. check every item in this node to see if the node is to use Findleaf
			//until each node has been checked.
			for (int i = 0; i < root->items.size(); i++) {
					CoverFind(static_cast<Node*>(root->items[i]));
			}
		}
		else if (root->hasLeaves == true) {
			//ͨ������ɸѡ�������ϵĵ�ѹ��found
			int time, id;
			double x, y;
			for (int i = 0; i < root->items.size(); i++) {
				Leaf* leafnode = static_cast<Leaf*>(root->items[i]);
				check_pushback(leafnode,true);
			}
		}
	}
	//��FindEntry
	bool Findleaf(Node* root) {
		if (root == NULL) {
			return false;
		}
		if (root->hasLeaves == false) {
			//not a leaf. check every item in this node to see if the node is to use Findleaf
			//until each node has been checked.
			for (int i = 0; i < root->items.size(); i++) {
				if (root->Overlap(root->items[i], lbound, hbound))
				{
					Findleaf(static_cast<Node*>(root->items[i]));
				}
				else if (root->Cover(root->items[i], lbound, hbound)) {
					CoverFind(static_cast<Node*>(root->items[i]));
				}
			}
		}
		else if (root->hasLeaves == true) {
			//ͨ������ɸѡ�������ϵĵ�ѹ��found
			int time, id;
			double x, y;
			for (int i = 0; i < root->items.size(); i++) {
				Leaf* leafnode = static_cast<Leaf*>(root->items[i]);
				int timefit = TimeFit(leafnode->getTime());//�����ڲ�ѯ��Χ�ڵ�Ŀ�������0��1��2��
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
	}

	bool check_pushback(Leaf* node,bool check_only_time) {
		if (!check_only_time) cout << "wrong function called" << endl;
		int time, id;
		double x, y;
		node->getTaxi1(time, id, x, y);
		if (time > t1&& time < t2 ) {
			pushback_cor(id, time, x, y);
			return true;
		}
		node->getTaxi2(time, id, x, y);
		if (time > t1&& time < t2 ) {
			pushback_cor(id, time, x, y);
			return true;
		}
		return false;
	}

	bool check_pushback(Leaf* node) {
		int time, id;
		double x, y;
		node->getTaxi1(time, id, x, y);
		if (time>t1 && time<t2 && checkcor(x, y)) {
			pushback_cor(id, time, x, y);
			return true;
		}
		node->getTaxi2(time, id, x, y);
		if (time > t1&& time < t2 && checkcor(x, y)) {
			pushback_cor(id, time, x, y);
			return true;
		}
		return false;
	}

	bool checkcor(double x, double y) {
		return lbound[0]<x && hbound[0]>x && lbound[1]<y && hbound[1]>y;
	}

	void pushback_cor(int ID, int time, double x, double y) {
		found.push_back(foundNode(ID, time, x, y));								//�������һ�£�ֱ��found����push_back�ṹ
	}



	bool clearfound() {
		found.clear();
		found.shrink_to_fit();
		//found.reserve(10);
		return true;
	}

	bool difArea(double p1_low[2], double p1_high[2], const foundNode& a, foundNode& b) {
		bool aa = false, bb = false;
		if (a.lat <= p1_high[0] && a.lat >= p1_low[0] && a.lng <= p1_high[1] && a.lng >= p1_low[1])aa = true;//��area1Ϊ��
		if (b.lat <= p1_high[0] && b.lat >= p1_low[0] && b.lng <= p1_high[1] && b.lng >= p1_low[1])bb = true;//��area1Ϊ��
		return (!aa) && bb || (!bb) && aa;//���
	}//���������Ƿ��ڲ�ͬ������Ϊ��

	int trafficVolume5(Node* root, double p1_low[2], double p1_high[2], double p2_low[2], double p2_high[2], int tbegin, int tend) {
		map <int, vector<foundNode>>area;
		map <int, vector<foundNode>>::iterator iter;
		int count = 0, s, i;
		cout << "Start search in area1..." << endl;
		FindEntry(root, p1_low, p1_high, tbegin, tend);//��area1����ĵ�
		cout << found.size() << endl;
		for (i = 0; i < found.size(); i++) {
			s = found[i].ID;
			area[s].push_back(found[i]);
		}
		found.clear();
		cout << "Start search in area2..." << endl;
		FindEntry(root, p2_low, p2_high, tbegin, tend);//��area2����ĵ�
		for (i = 0; i < found.size(); i++) {
			s = found[i].ID;
			if (area.count(s)) {
				area[s].push_back(found[i]);
			}
		}//�ҵ��ĵ��������area1����ᱻ����map
		cout << "Counting traffic volume between area1 and area2..." << endl;
		for (iter = area.begin(); iter != area.end(); iter++) {
			sort((iter->second).begin(), (iter->second).end(), [](const foundNode& a, foundNode& b) {return a.time >= b.time; });
			for (i = 0; i < iter->second.size() - 1; i++) {
				if (difArea(p1_low, p1_high, (iter->second)[i], (iter->second)[i + 1]))count++;
			}
		}
		return count;
	}

	bool trafficVolume6(Node* root, double p1_low[2], double p1_high[2], int tbegin, int tend) {
		set <int>area1, area2, C, D;
		int count = 0, i;
		cout << "Counting traffic volume..." << endl;
		while (tbegin < tend) {	
			FindEntry(root, p1_low, p1_high, tbegin, t_grow(tbegin, tend));//��area1����ĵ�
		for (i = 0; i < found.size(); i++) {
				area1.insert(found[i].ID);
		}
		found.clear();
		FindEntry(root, p1_low, p1_high, tbegin, t_grow(tbegin, tend));
		for (i = 0; i < found.size(); i++) {
			area2.insert(found[i].ID);
		}
		set_intersection(area1.begin(), area1.end(), area2.begin(), area2.end(), inserter(C, C.begin()));
		set_union(area1.begin(), area1.end(), area2.begin(), area2.end(), inserter(D, D.begin()));
		trafficVolume.push_back(D.size()-C.size());
		tbegin = t_grow(tbegin,tend);
		C.clear();
		D.clear();
		}
		cout << trafficVolume.size() << endl;
		return true;
	}
public:
	Node *m_root = NULL;
	int m_size;
	std::vector<foundNode> found;	//foundNode���͵�found vector
	std::vector<int>trafficVolume;
};

