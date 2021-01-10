#pragma once
#include<string>
#include<ctime>
#include <time.h>
#include<fstream>
#include "filereader.h" 
#include"rstartree.h"

using namespace std;


int writetojson(std::vector<foundNode> file) {//参数:found
	string value = "create a new file!";
	cout << value << endl << "writing" << endl;
	ofstream outfile;
	outfile.open("data.json", ios::out);
	int maxfile = 100000;
	if (maxfile > file.size())maxfile = file.size();
	outfile << "taxidata([";								//回调函数的接受的json名字是taxidata
	outfile << "{\"cod\":[" << file[0].lng << ',' << file[0].lat << "]}";
	for (int i = 1; i < maxfile; i++) {
		outfile << ",{\"cod\":[" << file[i].lng << ',' << file[i].lat << "]}";
	}
	outfile << "])";
	outfile.close();
	return 0;
}

int main() {
	RStarTree tree;
	FileReader reader("d:\\new_7\\");	//一堆txt文件的路径

	clock_t t1 = clock();
	std::cout << "Timing begin" << std::endl;
	cout << tree.found.max_size() << endl;

	double x[2],y[2];	//相当于原来的x, xx, y, yy
	int time[2],ID[2];
		
	while (reader.constructBox(x, y, ID, time)) {
		tree.Insert(ID, time, boundingbox(x[0], y[0], x[1], y[1]), x[0], y[0], x[1], y[1]);
	}
	//tree.visit(tree.m_root);   //for testing
	cout << "Insertion cost " << (clock() - t1) * 1.0 / CLOCKS_PER_SEC << "s in total" << endl;
	cout << tree.found.max_size() << endl;
	while (true) {
		double lowbound[2], lowbound2[2];
		double highbound[2], highbound2[2];
		int tbegin, tend;//时间范围
		std::cout << "What do you want to do?" << std::endl << "1. Find a car；" << std::endl;
		std::cout << "2. Count traffic volume between two area；" << std::endl << "3. Count traffic volume between one area and other area；" << std::endl;
		std::cout <<"4. Quit" << std::endl<<"Please enter your choice:";
		int opt;
		std::cin >> opt;
		switch (opt)
		{ 
			case 1: {
				std::cout << "enter the searching area{(x,y),(xx,yy)}" << std::endl;
				std::cout << "x:";
				std::cin >> lowbound[0];
				std::cout << "y:";
				std::cin >> lowbound[1];
				std::cout << "xx:";
				std::cin >> highbound[0];
				std::cout << "yy:";
				std::cin >> highbound[1];
				cout << "time range: [2000000 , 9000000]" << endl;
				cout << "begining of the time: "; cin >> tbegin;
				cout << "end of the time: "; cin >> tend;
				assert(lowbound[0] < highbound[0] && lowbound[1] < highbound[1]);
				clock_t t2 = clock();
				cout << "Start search in area {(x,y),(xx,yy)}" << endl;
				tree.FindEntry(tree.m_root, lowbound, highbound, tbegin, tend);//加入时间范围的搜索
				//sort(tree.found.begin(), tree.found.end(), [](const foundNode& a, const foundNode& b) {return a.time < b.time; }); //比较函数相应改了一下
				cout << "Searching cost " << (clock() - t2) * 1.0 / CLOCKS_PER_SEC * 1000 << "ms in total" << endl;
				cout << "Visit " << tree.found.size() << " positions among " << endl;
				writetojson(tree.found);//更改，将时间范围移除
				break;
			}
			case 2: {
				std::cout << "enter the searching area1{(x,y),(xx,yy)}" << std::endl;
				std::cout << "x1:";
				std::cin >> lowbound[0];
				std::cout << "y1:";
				std::cin >> lowbound[1];
				std::cout << "xx1:";
				std::cin >> highbound[0];
				std::cout << "yy1:";
				std::cin >> highbound[1];
				std::cout << "enter the searching area2{(x,y),(xx,yy)}" << std::endl;
				std::cout << "x2:";
				std::cin >> lowbound2[0];
				std::cout << "y2:";
				std::cin >> lowbound2[1];
				std::cout << "xx2:";
				std::cin >> highbound2[0];
				std::cout << "yy2:";
				std::cin >> highbound2[1];
				cout << "time range: [2000000 , 9000000]" << endl;
				cout << "begining of the time: "; cin >> tbegin;
				cout << "end of the time: "; cin >> tend;
				assert(lowbound[0] < highbound[0] && lowbound[1] < highbound[1]);
				assert(lowbound2[0] < highbound2[0] && lowbound2[1] < highbound2[1]);
				cout << tree.trafficVolume5(tree.m_root, lowbound, highbound, lowbound2, highbound2, tbegin, tend) << endl;
				break; 
			}
			case 3: {
				std::cout << "enter the searching area1{(x,y),(xx,yy)}" << std::endl;
				std::cout << "x1:";
				std::cin >> lowbound[0];
				std::cout << "y1:";
				std::cin >> lowbound[1];
				std::cout << "xx1:";
				std::cin >> highbound[0];
				std::cout << "yy1:";
				std::cin >> highbound[1];
				cout << "time range: [2000000 , 9000000]" << endl;
				cout << "begining of the time: "; cin >> tbegin;
				cout << "end of the time: "; cin >> tend;
				assert(lowbound[0] < highbound[0] && lowbound[1] < highbound[1]);
				tree.trafficVolume6(tree.m_root, lowbound, highbound, tbegin, tend);
				break; 
			}
			  default: break;
		}
		cout << "Done. Search again? (y/n)"<<endl;
		tree.clearfound();
		char again;
		cin >> again;
		if (again == 'n')
			break;
	}
	return 0;
}