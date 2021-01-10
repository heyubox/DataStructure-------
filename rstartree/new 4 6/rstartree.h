#pragma once
#include<list>
#include<vector>
#include<iostream>
#include<cassert>
#include<algorithm>
#include<functional>
#include <iterator>  
#include<map>
#include<set>

#include"boundingbox.h"

#define RTREE_REINSERT_P 0.30
#define RTREE_CHOOSE_SUBTREE_P 32
#define min_child_item 32
#define max_child_item 64


struct RStarNode :boundingbox {
	std::vector<RStarNode*> items;
	bool hasLeaves;
	int leaf[2] = { 0 };
	int name[2] = { 0 };
};


struct foundNode {						//ԭ���汾��found_vector�����׸�Сvector������ֱ������һ���ṹ�����������Сvector
	int ID;
	int time;
	double lng;
	double lat;
	foundNode(int &id, int &t, double &x, double &y):ID(id), time(t), lng(x), lat(y){}
};

class RStarTree {
public:
	typedef boundingbox boundingbox;
	typedef foundNode foundNode;
	typedef RStarNode Leaf;
	typedef RStarNode Node;


	RStarTree() : m_root(NULL), m_size(0) {
		assert(1 <= min_child_item && min_child_item <= max_child_item / 2);
	}

	void Insert(int ID[], int* time, const boundingbox& bound) {
		Leaf* newLeaf = new Leaf();
		newLeaf->assignbound(bound);
		newLeaf->leaf[0] = time[0];
		newLeaf->leaf[1] = time[1];
		newLeaf->name[0] = ID[0];
		newLeaf->name[1] = ID[1];
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
			if (max_child_item > (RTREE_CHOOSE_SUBTREE_P * 2) / 3 && node->items.size() > RTREE_CHOOSE_SUBTREE_P) {
				//we never get into this part
				//��֪�������ɲ����� ����ʲô����
				//�˴��ƺ�������
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

	}


	//��������ڸ�����ڵ�
	//����InsertInternalѰ�Ҳ����
	//�����Ժ������еĵݹ�Ѱ��
	Node* InsertInternal(Leaf* leaf, Node* node, bool firstInsert = true) {
		node->stretch(leaf->lowedge, leaf->highedge);
		//std::cout << "doing InsertInternal" << std::endl;
		//
		if (node->hasLeaves) {
			node->items.push_back(leaf);
			//std::cout << "eaily push back the new node(leaf)" << std::endl;
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


	Node* OverflowTreatment(Node* level, bool firstInsert) {
		//std::cout << "start overflowtreatment" << std::endl;
		if (level != m_root && firstInsert) {
			//ǿ�����²���
			Reinsert(level);
			return NULL;
		}

		Node* splitItem = Split(level);

		if (level == m_root) {
			Node* newRoot = new Node();
			newRoot->hasLeaves = false;
			//std::cout << "reserve memory" << std::endl;
			//reserve memory
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


	}


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
	//�趨��ѯʱ��
	void SetTime(int tb, int te) {
		t1 = tb;
		t2 = te;
	}
	//�����ڲ�ѯ��Χ�ڵ�Ŀ�������0��1��2��
	int TimeFit(int leaf0, int leaf1) {
		int flag = 0;
		if (leaf0 > t1&& leaf0 < t2)
			flag++;
		if (leaf1 > t1&& leaf1 < t2)
			flag++;
		return flag;
	}

	bool Findleaf(Node* root, double *lowedge, double *highedge,int tbegin,int tend) {
		SetTime(tbegin, tend);
		if (root == NULL) {
			return false;
		}
		if (root->hasLeaves == false) {
			//not a leaf. check every item in this node to see if the node is to use Findleaf
			//until each node has been checked.
			for (int i = 0; i < root->items.size(); i++) {
				if (Overlap(root->items[i], lowedge, highedge) || Cover(root->items[i], lowedge, highedge))
				{
					Findleaf(root->items[i], lowedge, highedge,t1,t2);
				}
			}
		}
		else if (root->hasLeaves == true) {
			//��һ��������
			//ͨ������ɸѡ�������ϵĵ�ѹ��found
			
			for (int i = 0; i < root->items.size(); i++) {
				int timefit = TimeFit(root->items[i]->leaf[0], root->items[i]->leaf[1]);//�����ڲ�ѯ��Χ�ڵ�Ŀ�������0��1��2��
				if (timefit==2 &&Cover(root->items[i], lowedge, highedge)) {
					pushback_cor(root->items[i]->name[0], root->items[i]->leaf[0], root->items[i]->lowedge[0], root->items[i]->lowedge[1]);
					pushback_cor(root->items[i]->name[1], root->items[i]->leaf[1], root->items[i]->highedge[0], root->items[i]->highedge[1]);
				}
				else if (timefit==1 && Overlap(root->items[i], lowedge, highedge)) {
					check_pushback(root->items[i], lowedge, highedge);
				}
			}
		}
	}

	bool check_pushback(Node* node, double* bound_lowedge, double* bound_highedge) {
		double x = node->lowedge[0];
		double y = node->lowedge[1];
		double xx = node->highedge[0];
		double yy = node->highedge[1];
		if (node->leaf[0]>t1 && node->leaf[0]<t2 && checkcor(x, y, bound_lowedge, bound_highedge)) {
			pushback_cor(node->name[0], node->leaf[0], x, y);
			return true;
		}
		else if (node->leaf[1] > t1&& node->leaf[1] < t2&& checkcor(xx, yy, bound_lowedge, bound_highedge)) {
			pushback_cor(node->name[1], node->leaf[1], xx, yy);
			return true;
		}
		return false;
	}

	bool checkcor(double x, double y, double* bound_l, double* bound_h) {
		return bound_l[0]<x && bound_h[0]>x && bound_l[1]<y && bound_h[1]>y;
	}

	void pushback_cor(int ID, int time, double x, double y) {
		found.push_back(foundNode(ID, time, x, y));								//�������һ�£�ֱ��found����push_back�ṹ
	}

	bool Overlap(Node* node, double* lowedge, double* highedge) {
		for (int axis = 0; axis < 2; axis++) {
			if (!(lowedge[axis] < node->highedge[axis]) || !(node->lowedge[axis] < highedge[axis]))
				return false;
		}
		return true;
	}

	bool Cover(Node* node, double* lowedge, double* highedge) {

		if ((node->lowedge[0] > lowedge[0] && node->highedge[0] < highedge[0])
			&& (node->lowedge[0] < highedge[0] && node->highedge[0] > lowedge[0])
			&& (node->lowedge[1] > lowedge[1] && node->highedge[1] < highedge[1])
			&& (node->lowedge[1] < highedge[1] && node->highedge[1] > lowedge[1]))
			return true;
		return false;

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

	int trafficVolume5(Node* root, double p1low[2], double p1_high[2], double p2_low[2], double p2_high[2], int tbegin, int tend) {
		map <int, vector<foundNode>>area;
		map <int, vector<foundNode>>::iterator iter;
		int count = 0, s, i;
		Findleaf(root, p1low, p1_high, tbegin, tend);//��area1����ĵ�
		for (i = 0; i < found.size(); i++) {
			s = found[i].ID;
			area[s].push_back(found[i]);
		}
		found.clear();
		Findleaf(root, p2_low, p2_high, tbegin, tend);//��area2����ĵ�
		for (i = 0; i < found.size(); i++) {
			if (area.count(s)) {
				s = found[i].ID;
				area[s].push_back(found[i]);
			}
		}//�ҵ��ĵ��������area1����ᱻ����map
		for (iter = area.begin(); iter != area.end(); iter++) {
			sort((iter->second).begin(), (iter->second).end());
			for (i = 0; i < iter->second.size() - 1; i++) {
				if (difArea(p1low, p1_high, (iter->second)[i], (iter->second)[i + 1]))count++;
			}
		}
		return count;
	}

	int trafficVolume6(Node* root, double p1_low[2], double p1_high[2], int tbegin, int tend) {
		set <int>area1, area2, C, D, E;
		int count = 0, s, i;
		Findleaf(root, p1_low, p1_high, tbegin, tend);//��area1����ĵ�
		for (i = 0; i < found.size(); i++) {
			s = found[i].ID;
			area1.insert(s);
		}
		found.clear();
		Findleaf(root, p1_low, p1_high, tend, 2 * tend - tbegin);
		for (i = 0; i < found.size(); i++) {
			s = found[i].ID;
			area2.insert(s);
		}
		set_intersection(area1.begin(), area1.end(), area2.begin(), area2.end(), inserter(C, C.begin()));
		set_union(area1.begin(), area1.end(), area2.begin(), area2.end(), inserter(D, D.begin()));
		set_difference(C.begin(), C.end(), D.begin(), D.end(), inserter(E, E.begin()));
		return E.size();
	}
public:
	Node *m_root = NULL;
	int m_size;
	std::vector<foundNode> found;	//foundNode���͵�found vector
	int t1, t2;//ʱ���ѯ��Χ
};

