#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

int flag[3000000];
int tmp;
int repeatTimes = 500;

const int mk = 20;
const int maxn = 500;
const int trainTimes = 200;
int turn = 1;
int n = 500;
int m = 6;
int p = 1;

enum ConditionNum {
	unknow,
	know,
	approve,
	finish
};

struct Node {
	float confidence;
	float input;
	float T;
	float b;
	float lambda;
	bool flag;
	ConditionNum condition;

	Node() {
		flag = false;
		b = 0.8;
		lambda = 0.2;
		T = 1;
		input = 0;
		confidence = 1;
		condition = unknow;
	}

	void transferCond() {
		if (condition == know && flag == true) {
			int x = int(((lambda - T) * exp((input - 1) * (-b)) + T) * 10000);
			int p = rand() % 10000;
			if (p < x) condition = approve;
		}
		flag = false;
	}

	void setCond(ConditionNum con) {
		condition = con;
	}
};

struct Edge {
	float transferProb;
	float acceptProb;
	int transferTime;
	int from;
	int to;

	Edge() {
		acceptProb = 1;
		transferProb = 1;
		transferTime = 1;
	}
};

vector<vector<Edge> > edges;
vector<int> edgeTot;
vector<Node> nodes;

Node popularNode() {
	Node res = Node();
	res.flag = false;
	res.b = 0.8;
	res.lambda = 0.2;
	res.T = 1;
	res.input = 0;
	res.confidence = 1;
	res.condition = unknow;
	return res;
}

Node normalNode() {
	Node res = Node();
	res.flag = false;
	res.b = 0.8;
	res.lambda = 0.2;
	res.T = 1;
	res.input = 0;
	res.confidence = 1;
	res.condition = unknow;
	return res;
}

Node mediaNode() {
	Node res = Node();
	res.flag = false;
	res.b = 0.8;
	res.lambda = 0.2;
	res.T = 1;
	res.input = 0;
	res.confidence = 1;
	res.condition = unknow;
	return res;
}

void addPopularNormal(int x, int y) {
	Edge edge;
	edge.from = x;
	edge.to = y;
	edge.transferTime = 1;
	edge.transferProb = 0.75;
	edge.acceptProb = 1;
	edgeTot[x]++;
	edges[x].push_back(edge);
}

void addNormalNormal(int x, int y) {
	Edge edge;
	edge.from = x;
	edge.to = y;
	edge.transferTime = 1;
	edge.transferProb = 0.75;
	edge.acceptProb = 1;
	edgeTot[x]++;
	edges[x].push_back(edge);
}

void addMediaPeople(int x, int y) {
	Edge edge;
	edge.from = x;
	edge.to = y;
	edge.transferTime = 1;
	edge.transferProb = 0.75;
	edge.acceptProb = 1;
	edgeTot[x]++;
	edges[x].push_back(edge);
}

void addPeopleMedia(int x, int y) {
	Edge edge;
	edge.from = x;
	edge.to = y;
	edge.transferTime = 1;
	edge.transferProb = 0.75;
	edge.acceptProb = 1;
	edgeTot[x]++;
	edges[x].push_back(edge);
}

void initialNetwork() {
	vector<int> x; x.clear();
	vector<int> y; y.clear();
	edges.clear();
	edgeTot.clear();
	vector<Edge> gg; gg.clear();
	for (int i = 0; i < n; i++) {
		edgeTot.push_back(0);
		edges.push_back(gg);
	}
	nodes.clear();
	for (int i = 0; i < n; i++) {
		nodes.push_back(normalNode());
		if (i <= m) {
			for (int j = 0; j < i; j++) {
				x.push_back(j);
				y.push_back(i);
			}
		} else {
			tmp++;
			for (int j = 0; j < m / 2; j++) {
				int k = rand() % (i * 3);
				while (flag[k] == tmp) {
					k = rand() % (i * 3);
				}
				flag[k] = tmp;
				x.push_back(y[k]);
				y.push_back(i);
				y[k] = i;
			}
		}
	}
	for (int i = 0; i < x.size(); i++) {
		addNormalNormal(x[i], y[i]);
		addNormalNormal(y[i], x[i]);
	}
	for (int i = 0; i < p; i++) {
		nodes[i] = popularNode();
		tmp++;
		for (int j = 0; j < m * mk; j++) {
			int k = rand() % (n - p) + p;
			while (flag[k] == tmp) {
				k = rand() % (n - p) + p;
			}
			flag[k] = tmp;
			addPopularNormal(i, k);
		}
	}

	edgeTot.push_back(0);
	edges.push_back(gg);
	nodes.push_back(mediaNode());
	for (int i = 0; i < n; i++) {
		addMediaPeople(n, i);
		addPeopleMedia(i, n);
	}
}

void work() {
	vector<vector<Node> >opt;
	vector<int> res;

	for (int i = 0; i < trainTimes; i++)
		res.push_back(0);
	for (int rep = 0; rep < repeatTimes; rep++) {
	if (rep % 40 ==0)
			initialNetwork();
	opt.clear();
	for (int i = 0; i <= turn; i++)
		opt.push_back(nodes);
	opt[0][n-1].condition = approve;
	for (int loop = 0; loop < trainTimes; loop++) {
		int tip = loop % (turn + 1);
		opt[(loop + turn) % (turn + 1)] = nodes;
		for (int i = edges.size() - 1; i >= 0; i--) {
			opt[tip][i].transferCond();
			if (opt[tip][i].condition == approve) {
				for (int j = 0; j < edgeTot[i]; j++) {
					int k = edges[i][j].to;
					int nexttip = (loop + edges[i][j].transferTime) % (turn + 1);
					opt[nexttip][k].input += edges[i][j].transferProb * edges[i][j].acceptProb * opt[tip][i].confidence;
					opt[nexttip][k].flag = true;
					if (opt[nexttip][k].condition == unknow)
						opt[nexttip][k].condition = know;
				}
				opt[(tip+1) % (turn+1)][i].condition = finish;
				res[loop]++;
			} else if (opt[tip][i].condition == finish) {
				opt[(tip+1) % (turn+1)][i].condition = finish;
				res[loop]++;
			}
			else if (opt[tip][i].condition == know) {
				opt[(tip+1) % (turn+1)][i].condition = know;
				opt[(tip+1) % (turn+1)][i].input += opt[tip][i].input;
			}
		}
	}
	}

	FILE * fout = fopen("log.txt", "w");
	for (int i = 0; i < trainTimes; i++) {
		printf( "%f\n", res[i] / (float)(repeatTimes));
		fprintf(fout, "%f\n", res[i] / (float)(repeatTimes));
	}
	fclose(fout);
}

int main() {
	srand(time(NULL));
	initialNetwork();
	work();
	return 0;
}