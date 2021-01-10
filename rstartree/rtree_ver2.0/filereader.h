#pragma warning( disable : 4996)
#include <cstdio>
#include <cstring>
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
using namespace std;

#define MAX_FILE_NUM_LEN 8 

/*读文件的对象FileReader, 每调用一次constructBox函数就是读取两条记录并且将两个点的x，y，ID，time保存到main.cpp里面的x[], y[], id[], time[]
读完10357.txt ,继续打开10358时会返回false
*/




class FileReader {

private:
	FILE *fp;
	string root;
	bool is_open;
	char bufferZone[50];
	int fileNum;
	int dot_id, dot_time;
	double dot_lng, dot_lat;


public:
	FileReader(string rt)
	{
		fp = NULL;
		is_open = false;
		fileNum = 0;
		dot_id = dot_time = 0;
		dot_lng = dot_lat = 0;
		memset(bufferZone, 0, sizeof(bufferZone));
		root = rt;
	}

	bool cdNext() {										//跳到下一个txt文件
		if (fp) fclose(fp);
		fp = NULL;
		is_open = false;
		string num;
		int temp = ++fileNum;
		char str[MAX_FILE_NUM_LEN];
		int i = 0;
		while (temp) {
			str[i++] = temp % 10 + '0';
			temp /= 10;
		}
		while (i--)
			num += str[i];
		num += ".txt";
		fp = fopen(string(root + num).c_str(), "r");
		if (!fp) return false;
		is_open = true;
		return true;
	}

	bool parseData() {								//解析一行记录数据
		int len = strlen(bufferZone) - 1;
		bufferZone[len] = '\0';
		if (len < 17) return false;	//不规范数据去除
		int i = 0, temp = 0, dec = 0;
		bool flag = false;

		while (bufferZone[i] != ',') {
			temp = temp * 10 + (bufferZone[i++] - '0');
		}dot_id = temp;	++i;  temp = 0;

		while (bufferZone[i] != ',') {
			temp = temp * 10 + (bufferZone[i++] - '0');
		}dot_time = temp;	++i;  temp = 0; dec = 0;

		while (bufferZone[i] != ',') {
			if (bufferZone[i] == '.') {
				flag = true;  ++i;	continue;
			}
			temp = temp * 10 + (bufferZone[i++] - '0');
			if (flag)	dec++;
		}
		dot_lng = temp * pow(0.1, dec);
		flag = false; ++i;	temp = 0; dec = 0;

		while (i < len) {
			if (bufferZone[i] == '.') {
				flag = true;  ++i;	continue;
			}
			temp = temp * 10 + (bufferZone[i++] - '0');
			if (flag)	dec++;
		}
		dot_lat = temp * pow(0.1, dec);
		return true;
	}

	bool constructBox(double x[], double y[], int ID[], int time[]) {		//读取的记录会保留在x[], y[], ID[], time[]中
		int need_dot = 2;
		while (need_dot) {
			if (!is_open) {
				if (!cdNext()) {
					if (need_dot == 2)
						return false;
					if (need_dot == 1)
						x[1] = x[0]; y[1] = y[0]; ID[1] = ID[0]; time[1] = time[0];
					return true;
				}
			}
			else {
				if (fgets(bufferZone, 50, fp)) {
					if (parseData()) {
						--need_dot;  ID[need_dot] = dot_id;	 time[need_dot] = dot_time; x[need_dot] = dot_lng; y[need_dot] = dot_lat;
					}
				}
				else {
					fclose(fp);
					is_open = false;
				}
			}
		}
		return true;
	}
};