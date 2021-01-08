#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<vector>
#include<cmath>
#include<Windows.h>
#include<random>
#include<ctime>
#define _for(i,a,b) for( int i=a; i<b; ++i)
#define mfor(i,b,a) for(int i=b;i>=a;i--)

using namespace std;

vector<vector<double>> ReadMatrixFromFile(string); // 从文件读取一个m行n列的矩阵
vector<vector<double>> RandomlyGeneratedMatrix();  // 随机产生一个n行n+1列的方阵
void PrintMatrix(vector<vector<double>>&);         // 打印矩阵
void Filtration(vector<vector<double>>&);          // 模拟因为过滤并行产生的干扰
int GaussEliminationWithoutSelection();            // 不列主元的高斯消去法求解线性方程组
int GaussElimination();                            // 列主元素的高斯消去法求解线性方程组
int DoolittleWithoutSelection();                   // 不选主元杜利特尔分解求解线性方程组
int Doolittle();                                   // 选主元的杜利特尔分解求解线性方程组


int main()
{
	int type = 5;
	while (type) {
		cout << "输入1，运行不列主元的高斯消去法；\n输入2，运行列主元素的高斯消去法；\n输入3，运行不选主元杜利特尔分解；\n输入4，运行选主元的杜利特尔分解；\n输入0，程序结束。\n";
		cin >> type;
		switch (type)
		{
		case 0:
			break;
		case 1:
			GaussEliminationWithoutSelection();
			break;
		case 2:
			GaussElimination();
			break;
		case 3:
			DoolittleWithoutSelection();
			break;
		case 4:
			Doolittle();
			break;
		default:
			cout << "请输入0-4！";
		}
	}
	return 0;
}

// 识别文件中的矩阵有几行几列，然后读取出来
vector<vector<double>> ReadMatrixFromFile(string fileName) {

	fstream inputfile;
	inputfile.open(fileName, ios::in | ios::binary);
	if (!inputfile.is_open()) {
		cerr << "File open error!\n";
		return {};
	}

	char c;
	int n_row = 1;                     // 行数
	inputfile.seekg(0, ios::end);      // 指向末尾
	streampos sp = inputfile.tellg();  // 获取文件字符个数

	inputfile.seekg(0, ios::beg);      // 指向文件头
	while (inputfile.tellg() != sp)    // 如果任由其getc下去最后无法指向头部了，所以到sp就刹车   
	{
		inputfile.get(c);
		if (c == '\n') n_row++;        // 文件行数即矩阵的行数
	}

	inputfile.seekg(0, ios::beg);      // 回到文件头
	int n_col = 1;                     // 列数
	while (inputfile.tellg() != sp) {
		inputfile.get(c);
		if (c == ' ')
			n_col++;
		else if (c == '\n')
			break;
	}

	inputfile.seekg(0, ios::beg);      // 回到文件头
	vector<vector<double>> a(n_row, vector<double>(n_col));
	_for(i, 0, n_row)
		_for(j, 0, n_col)
		inputfile >> a[i][j];          // 读取数据
	inputfile.close();
	PrintMatrix(a);
	return a;
}

vector<vector<double>> RandomlyGeneratedMatrix() {
	cout << "请输入矩阵行数：";
	int n;
	cin >> n;
	cout << "请输入矩阵列数：";
	int m;
	cin >> m;
	vector<vector<double>> a(n, vector<double>(m));
	cout << "请输入矩阵最大值：";
	double maxv;
	cin >> maxv;
	cout << "请输入矩阵最小值：";
	double minv;
	cin >> minv;
	cout << endl;
	default_random_engine e((unsigned int)time(0));
	uniform_real_distribution<double> u(minv, maxv);

	_for(i, 0, n)
	{
		_for(j, 0, m)
		{
			a[i][j] = u(e);
			//cout << a[i][j] << "   ";
		}
		//cout << endl;
	}
	cout << endl;
	return a;
}

void PrintMatrix(vector<vector<double>>& a) {
	cout << "\n";
	int n = a.size(), m = a[0].size();
	_for(i, 0, n) {
		_for(j, 0, m)
			cout << a[i][j] << " ";
		cout << endl;
	}
	cout << "\n";
}

void Filtration(vector<vector<double>>& a) {
	_for(i, 0, int(a.size()))
		_for(j, 0, int(a[0].size())) {
		int v = (int)a[i][j];
		switch (v % 3)
		{
		case 0:
			a[i][j] = (int)((0.5 + pow(0.5, v)) * v + 0.5);
			break;
		case 1:
			a[i][j] = (int)(v * (2.0 - 2.0 / (1.0 + exp(0.01 - 0.01 * v))) + 0.5);
			break;
		case 2:
			a[i][j] = (int)(pow(v, 0.95) + 0.5);
			break;
		}
	}
}

int GaussEliminationWithoutSelection() {
	int gw = 1;
	cout << "不列主元的高斯消去法\n\n随机生成矩阵，请输入1；从文件读取矩阵，请输入2：";
	cin >> gw;                         // 1代表随机生成，2代表从文件读取

	vector<vector<double>> A;          // 资源使用矩阵N
	vector<vector<double>> b;          // 资源数量矩阵n^T
	vector<vector<double>> r;          // 作业需求数量矩阵r
	vector<double> s;                  // 作业满足比例  
	string fileName = "";
	if (gw == 1) {
		A = RandomlyGeneratedMatrix();
		b = RandomlyGeneratedMatrix();
		r = RandomlyGeneratedMatrix();
	}
	else {
		/*cout << "请输入资源使用矩阵N的文件路径：";
		cin >> fileName;
		A = ReadMatrixFromFile(fileName);
		cout << "请输入资源数量矩阵n的文件路径：";
		cin >> fileName;
		b = ReadMatrixFromFile(fileName);
		cout << "请输入作业需求数量矩阵r的文件路径：";
		cin >> fileName;
		r = ReadMatrixFromFile(fileName);*/
		A = ReadMatrixFromFile("A.txt");
		b = ReadMatrixFromFile("b.txt");
		r = ReadMatrixFromFile("r.txt");
	}

	int n = A.size();                  // 资源种数
	int K = b[0].size();               // 方程组组数
	s.resize(r.size());
	if (n != b.size()) {
		cerr << "资源使用矩阵N的行数与资源数量矩阵列数不同！无法求解。\n";
		return 3;
	}
	if (n != A[0].size()) {
		cerr << "资源使用矩阵不是方阵！\n";
		return 4;
	}
	if (n != r.size()) {
		cerr << "作业需求数量矩阵行数不是作业种数！\n";
		return 5;
	}

	double run_time;
	LARGE_INTEGER time_start;	           // 开始时间
	LARGE_INTEGER time_over;	           // 结束时间
	double dqFreq;		                   // 计时器频率
	LARGE_INTEGER f;	                   // 计时器频率
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	QueryPerformanceCounter(&time_start);// 计时开始

	int num_mul_div = 0;                   // 乘除法次数
	int num_add_sub = 0;                   // 加减法次数
	vector<vector<double>> x(K, vector<double>(n));     // K个方程组的解  
	vector<vector<double>> a(n, vector<double>(n + 1)); // 把A和b的每一列合并构造增广矩阵

	Filtration(b);

	_for(q, 0, K) {                        // 遍历求解每个方程组

		_for(i, 0, n)
			_for(j, 0, n)
			a[i][j] = A[i][j];
		_for(i, 0, n)                      // 增广矩阵
			a[i][n] = b[i][q];

		double det = 1.0;
		_for(k, 0, n - 1) {
			
			if (a[k][k] == 0) {
				cerr << "此时无法使用高斯消去!\n";
				return 1;
			}

			double m;
			_for(i, k + 1, n) {            // 消元计算
				m = a[i][k] / a[k][k];
				a[i][k] = 0.0;
				_for(j, k + 1, n + 1)
					a[i][j] = a[i][j] - m * a[k][j];
				num_mul_div += n - k + 1;
				num_add_sub += n - k;
			}
			det = det * a[k][k];
		}

		if (a[n - 1][n - 1] == 0.0) {
			if (a[n - 1][n] == 0.0)
				cout << "有无穷多解！\n";
			else cerr << "方程组无解！\n";
			return 2;
		}

		mfor(i, n - 1, 0) {
			double tempSum = 0.0;
			_for(j, i + 1, n)
				tempSum += a[i][j] * x[q][j];
			x[q][i] = (a[i][n] - tempSum) / a[i][i];
			num_mul_div += n - i;
			num_add_sub += n - i;
		}
	}

	_for(i, 0, n) {                        // 计算每种作业可以满足其需求的比例
		int rSum = 0;
		_for(j, 0, K)
			rSum += (int)(x[j][i] + 0.1);    // x[j][i]可能很接近上面一个整数！系统误差！
		s[i] = min(rSum, r[i][0]) / r[i][0];
	}
	num_mul_div += n;
	num_add_sub += K * n;

	QueryPerformanceCounter(&time_over);   // 计时结束
	run_time = 1000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
	printf("\n运行时间：%.3fms\n", run_time);

	double e = 0.0;                        // 平均相对误差
	_for(q, 0, K)
		_for(i, 0, n) {
		double dc = 0.0;
		_for(j, 0, n)
			dc += A[i][j] * (int)(x[q][j] + 0.1);       // 要用原始的A！！！
		e += abs(dc - b[i][q]) / b[i][q];
	}
	e = e / n / K;

	cout << " x = \n";
	_for(i, 0, K) {
		cout << "[";
		_for(j, 0, n - 1)
			cout << (int)(x[i][j] + 0.1) << ", ";
		cout << (int)(x[i][n - 1] + 0.1) << "]^T" << endl;
	}
	cout << "\n s=(";
	_for(i, 0, n - 1)
		cout << s[i] << ", ";
	cout << s[n - 1] << ")^T\n";
	cout << "平均相对误差 e = " << e << endl;
	cout << "乘除法次数：" << num_mul_div << endl;
	cout << "加减法次数：" << num_add_sub << "\n\n\n";

	return 0;
}

int GaussElimination() {
	int gw = 1;
	cout << "列主元的高斯消去法\n\n随机生成矩阵，请输入1；从文件读取矩阵，请输入2：";
	cin >> gw;                         // 1代表随机生成，2代表从文件读取

	vector<vector<double>> A;          // 资源使用矩阵N
	vector<vector<double>> b;          // 资源数量矩阵n^T
	vector<vector<double>> r;          // 作业需求数量矩阵r
	vector<double> s;                  // 作业满足比例  
	string fileName = "";
	if (gw == 1) {
		A = RandomlyGeneratedMatrix();
		b = RandomlyGeneratedMatrix();
		r = RandomlyGeneratedMatrix();
	}
	else {
		/*cout << "请输入资源使用矩阵N的文件路径：";
		cin >> fileName;
		A = ReadMatrixFromFile(fileName);
		cout << "请输入资源数量矩阵n的文件路径：";
		cin >> fileName;
		b = ReadMatrixFromFile(fileName);
		cout << "请输入作业需求数量矩阵r的文件路径：";
		cin >> fileName;
		r = ReadMatrixFromFile(fileName);*/
		A = ReadMatrixFromFile("A.txt");
		b = ReadMatrixFromFile("b.txt");
		r = ReadMatrixFromFile("r.txt");
	}

	int n = A.size();                  // 资源种数
	int K = b[0].size();               // 方程组组数
	s.resize(r.size());
	if (n != b.size()) {
		cerr << "资源使用矩阵N的行数与资源数量矩阵列数不同！无法求解。\n";
		return 3;
	}
	if (n != A[0].size()) {
		cerr << "资源使用矩阵不是方阵！\n";
		return 4;
	}
	if (n != r.size()) {
		cerr << "作业需求数量矩阵行数不是作业种数！\n";
		return 5;
	}

	double run_time;
	LARGE_INTEGER time_start;	           // 开始时间
	LARGE_INTEGER time_over;	           // 结束时间
	double dqFreq;		                   // 计时器频率
	LARGE_INTEGER f;	                   // 计时器频率
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	QueryPerformanceCounter(&time_start);// 计时开始

	int num_mul_div = 0;                   // 乘除法次数
	int num_add_sub = 0;                   // 加减法次数
	vector<vector<double>> x(K, vector<double>(n));     // K个方程组的解  
	vector<vector<double>> a(n, vector<double>(n + 1)); // 把A和b的每一列合并构造增广矩阵

	Filtration(b);

	_for(q, 0, K) {                        // 遍历求解每个方程组

		_for(i, 0, n)
			_for(j, 0, n)
			a[i][j] = A[i][j];
		_for(i, 0, n)                      // 增广矩阵
			a[i][n] = b[i][q];

		double det = 1.0;
		_for(k, 0, n - 1) {
			int maxi = k;
			_for(i, k+1, n)                  // 选列主元
				if (abs(a[i][k]) > abs(a[maxi][k]))
					maxi = i;
			if (a[maxi][k] == 0) {
				cerr << "此时无法使用高斯消去!\n";
				return 1;
			}
			if (maxi != k) {               // 交换两行
				double swap;
				_for(i, 0, n + 1) {
					swap = a[k][i];
					a[k][i] = a[maxi][i];
					a[maxi][i] = swap;
				}
				det = -det;                // 行列式的值变号
			}

			double m;
			_for(i, k + 1, n) {            // 消元计算
				m = a[i][k] / a[k][k];
				a[i][k] = 0.0;
				_for(j, k + 1, n + 1)
					a[i][j] = a[i][j] - m * a[k][j];
				num_mul_div += n - k + 1;
				num_add_sub += n - k;
			}
			det = det * a[k][k];
		}

		if (a[n - 1][n - 1] == 0.0) {
			if (a[n - 1][n] == 0.0)
				cout << "有无穷多解！\n";
			else cerr << "方程组无解！\n";
			return 2;
		}

		mfor(i, n - 1, 0) {
			double tempSum = 0.0;
			_for(j, i + 1, n)
				tempSum += a[i][j] * x[q][j];
			x[q][i] = (a[i][n] - tempSum) / a[i][i];
			num_mul_div += n - i;
			num_add_sub += n - i;
		}
	}

	_for(i, 0, n) {                        // 计算每种作业可以满足其需求的比例
		int rSum = 0;
		_for(j, 0, K)
			rSum += (int)(x[j][i] + 0.1);    // x[j][i]可能很接近上面一个整数！系统误差！
		s[i] = min(rSum, r[i][0]) / r[i][0];
	}
	num_mul_div += n;
	num_add_sub += K * n;

	QueryPerformanceCounter(&time_over);   // 计时结束
	run_time = 1000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
	printf("\n运行时间：%.3fms\n", run_time);

	double e = 0.0;                        // 平均相对误差
	_for(q, 0, K)
		_for(i, 0, n) {
		double dc = 0.0;
		_for(j, 0, n)
			dc += A[i][j] * (int)(x[q][j] + 0.1);       // 要用原始的A！！！
		e += abs(dc - b[i][q]) / b[i][q];
	}
	e = e / n / K;

	cout << " x = \n";
	_for(i, 0, K) {
		cout << "[";
		_for(j, 0, n - 1)
			cout << (int)(x[i][j] + 0.1) << ", ";
		cout << (int)(x[i][n - 1] + 0.1) << "]^T" << endl;
	}
	cout << "\n s=(";
	_for(i, 0, n - 1)
		cout << s[i] << ", ";
	cout << s[n - 1] << ")^T\n";
	cout << "平均相对误差 e = " << e << endl;
	cout << "乘除法次数：" << num_mul_div << endl;
	cout << "加减法次数：" << num_add_sub << "\n\n\n";

	return 0;
}

int DoolittleWithoutSelection() {
	int gw = 1;
	cout << "不选主元的LU分解法\n\n随机生成矩阵，请输入1；从文件读取矩阵，请输入2：";
	cin >> gw;                         // 1代表随机生成，2代表从文件读取

	vector<vector<double>> A;          // 资源使用矩阵N
	vector<vector<double>> b;          // 资源数量矩阵n^T
	vector<vector<double>> re;         // 作业需求数量矩阵r
	vector<double> s;                  // 作业满足比例  
	string fileName = "";
	if (gw == 1) {
		A = RandomlyGeneratedMatrix();
		b = RandomlyGeneratedMatrix();
		re = RandomlyGeneratedMatrix();
	}
	else {
		/*cout << "请输入资源使用矩阵N的文件路径：";
		cin >> fileName;
		A = ReadMatrixFromFile(fileName);
		cout << "请输入资源数量矩阵n的文件路径：";
		cin >> fileName;
		b = ReadMatrixFromFile(fileName);
		cout << "请输入作业需求数量矩阵r的文件路径：";
		cin >> fileName;
		r = ReadMatrixFromFile(fileName);*/
		A = ReadMatrixFromFile("A.txt");
		b = ReadMatrixFromFile("b.txt");
		re = ReadMatrixFromFile("r.txt");
	}

	int n = A.size();                  // 资源种数
	int K = b[0].size();               // 方程组组数
	s.resize(re.size());
	if (n != b.size()) {
		cerr << "资源使用矩阵N的行数与资源数量矩阵列数不同！无法求解。\n";
		return 3;
	}
	if (n != A[0].size()) {
		cerr << "资源使用矩阵不是方阵！\n";
		return 4;
	}
	if (n != re.size()) {
		cerr << "作业需求数量矩阵行数不是作业种数！\n";
		return 5;
	}

	double run_time;
	LARGE_INTEGER time_start;	           // 开始时间
	LARGE_INTEGER time_over;	           // 结束时间
	double dqFreq;		                   // 计时器频率
	LARGE_INTEGER f;	                   // 计时器频率
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	QueryPerformanceCounter(&time_start);// 计时开始

	int num_mul_div = 0;                   // 乘除法次数
	int num_add_sub = 0;                   // 加减法次数
	vector<vector<double>> x(K, vector<double>(n));     // K个方程组的解  
	vector<vector<double>> a(n, vector<double>(n + 1)); // 把A和b的每一列合并构造增广矩阵
	
	Filtration(b);
	_for(i, 0, n)
		_for(j, 0, n)
		a[i][j] = A[i][j];

	_for(r, 0, n) {                    // LU分解(只要分解一次！！！）

		_for(i, r, n)                  // 计算si
		{
			_for(k, 0, r)
				a[i][r] = a[i][r] - a[i][k] * a[k][r];
			num_mul_div += r;
			num_add_sub += r;
		}

		_for(i, r + 1, n)              // 计算Lir
			a[i][r] = a[i][r] / a[r][r];
		_for(i, r + 1, n)              // 计算Uri
			_for(k, 0, r)
			a[r][i] = a[r][i] - a[r][k] * a[k][i];
		num_mul_div += (n - r - 1) * (1 + r);
		num_add_sub += (n - r - 1) * r;
	}

	_for(q, 0, K) {                        // 遍历求解每个方程组

		_for(i, 0, n)                      // 增广矩阵
			a[i][n] = b[i][q];

		_for(i, 1, n)
			_for(k, 0, i)
			a[i][n] = a[i][n] - a[i][k] * a[k][n];
		num_mul_div += (n - 1) * n / 2;
		num_add_sub += (n - 1) * n / 2;

		mfor(i, n - 1, 0) {
			_for(k, i + 1, n)
				a[i][n] = a[i][n] - a[i][k] * x[q][k];
			x[q][i] = a[i][n] / a[i][i];
			num_mul_div += n - i;
			num_add_sub += n - i - 1;
		}

	}

	_for(i, 0, n) {                        // 计算每种作业可以满足其需求的比例
		int rSum = 0;
		_for(j, 0, K)
			rSum += (int)(x[j][i] + 0.1);    // x[j][i]可能很接近一个整数，系统表示问题
		s[i] = min(rSum, re[i][0]) / re[i][0];
	}
	num_mul_div += n;
	num_add_sub += K * n;

	QueryPerformanceCounter(&time_over);   // 计时结束
	run_time = 1000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
	printf("\n运行时间：%.3fms\n", run_time);

	double e = 0.0;                        // 平均相对误差
	_for(q, 0, K)
		_for(i, 0, n) {
		double dc = 0.0;
		_for(j, 0, n)
			dc += A[i][j] * (int)(x[q][j] + 0.1);
		e += abs(dc - b[i][q]) / b[i][q];
	}
	e = e / n / K;

	cout << " x = \n";
	_for(i, 0, K) {
		cout << "[";
		_for(j, 0, n - 1)
			cout << (int)(x[i][j] + 0.1) << ", ";
		cout << (int)(x[i][n - 1] + 0.1) << "]^T" << endl;
	}
	cout << "\n s=(";
	_for(i, 0, n - 1)
		cout << s[i] << ", ";
	cout << s[n - 1] << ")^T\n";
	cout << "平均相对误差 e = " << e << endl;
	cout << "乘除法次数：" << num_mul_div << endl;
	cout << "加减法次数：" << num_add_sub << "\n\n\n";

	return 0;
}

int Doolittle() {                        // 选主元的杜利特尔三角分解法
	int gw = 1;
	cout << "选主元的LU分解法\n\n随机生成矩阵，请输入1；从文件读取矩阵，请输入2：";
	cin >> gw;                         // 1代表随机生成，2代表从文件读取

	vector<vector<double>> A;          // 资源使用矩阵N
	vector<vector<double>> b;          // 资源数量矩阵n^T
	vector<vector<double>> re;         // 作业需求数量矩阵r
	vector<double> s;                  // 作业满足比例  
	string fileName = "";
	if (gw == 1) {
		A = RandomlyGeneratedMatrix();
		b = RandomlyGeneratedMatrix();
		re = RandomlyGeneratedMatrix();
	}
	else {
		/*cout << "请输入资源使用矩阵N的文件路径：";
		cin >> fileName;
		A = ReadMatrixFromFile(fileName);
		cout << "请输入资源数量矩阵n的文件路径：";
		cin >> fileName;
		b = ReadMatrixFromFile(fileName);
		cout << "请输入作业需求数量矩阵r的文件路径：";
		cin >> fileName;
		r = ReadMatrixFromFile(fileName);*/
		A = ReadMatrixFromFile("A.txt");
		b = ReadMatrixFromFile("b.txt");
		re = ReadMatrixFromFile("r.txt");
	}

	int n = A.size();                  // 资源种数
	int K = b[0].size();               // 方程组组数
	s.resize(re.size());
	if (n != b.size()) {
		cerr << "资源使用矩阵N的行数与资源数量矩阵列数不同！无法求解。\n";
		return 3;
	}
	if (n != A[0].size()) {
		cerr << "资源使用矩阵不是方阵！\n";
		return 4;
	}
	if (n != re.size()) {
		cerr << "作业需求数量矩阵行数不是作业种数！\n";
		return 5;
	}

	double run_time;
	LARGE_INTEGER time_start;	           // 开始时间
	LARGE_INTEGER time_over;	           // 结束时间
	double dqFreq;		                   // 计时器频率
	LARGE_INTEGER f;	                   // 计时器频率
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	QueryPerformanceCounter(&time_start);// 计时开始

	int num_mul_div = 0;                   // 乘除法次数
	int num_add_sub = 0;                   // 加减法次数
	vector<vector<double>> x(K, vector<double>(n));     // K个方程组的解  
	vector<vector<double>> a(n, vector<double>(n + 1)); // 把A和b的每一列合并构造增广矩阵
	vector<int> Ip(n);                     // 记录交换情况

	Filtration(b);
	_for(i, 0, n)
		_for(j, 0, n)
		a[i][j] = A[i][j];

	_for(r, 0, n) {                    // LU分解(只要分解一次！！！）

		_for(i, r, n)                  // 计算si
		{
			_for(k, 0, r)
				a[i][r] = a[i][r] - a[i][k] * a[k][r];
			num_mul_div += r;
			num_add_sub += r;
		}

		int maxi = r;
		_for(i, r + 1, n)              // 选主元
			if (a[maxi][r] < a[i][r])
				maxi = i;
		Ip[r] = maxi;

		double temp;
		_for(i, 0, n + 1)              // 交换两行
		{
			temp = a[r][i];
			a[r][i] = a[maxi][i];
			a[maxi][i] = temp;
		}

		_for(i, r + 1, n)              // 计算Lir
			a[i][r] = a[i][r] / a[r][r];
		_for(i, r + 1, n)              // 计算Uri
			_for(k, 0, r)
			a[r][i] = a[r][i] - a[r][k] * a[k][i];
		num_mul_div += (n - r - 1) * (1 + r);
		num_add_sub += (n - r - 1) * r;
	}

	_for(q, 0, K) {                        // 遍历求解每个方程组

		_for(i, 0, n)                      // 增广矩阵
			a[i][n] = b[i][q];

		_for(i, 0, n)                      // 记忆交换！
			swap(a[i][n], a[Ip[i]][n]);

		_for(i, 1, n)
			_for(k, 0, i)
			a[i][n] = a[i][n] - a[i][k] * a[k][n];
		num_mul_div += (n - 1) * n / 2;
		num_add_sub += (n - 1) * n / 2;

		mfor(i, n - 1, 0) {
			_for(k, i + 1, n)
				a[i][n] = a[i][n] - a[i][k] * x[q][k];
			x[q][i] = a[i][n] / a[i][i];
			num_mul_div += n - i;
			num_add_sub += n - i - 1;
		}

	}

	_for(i, 0, n) {                        // 计算每种作业可以满足其需求的比例
		int rSum = 0;
		_for(j, 0, K)
			rSum += (int)(x[j][i] + 0.1);    // x[j][i]可能很接近一个整数，系统表示问题
		s[i] = min(rSum, re[i][0]) / re[i][0];
	}
	num_mul_div += n;
	num_add_sub += K * n;

	QueryPerformanceCounter(&time_over);   // 计时结束
	run_time = 1000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
	printf("\n运行时间：%.3fms\n", run_time);

	double e = 0.0;                        // 平均相对误差
	_for(q, 0, K)
		_for(i, 0, n) {
		double dc = 0.0;
		_for(j, 0, n)
			dc += A[i][j] * (int)(x[q][j] + 0.1);
		e += abs(dc - b[i][q]) / b[i][q];
	}
	e = e / n / K;

	cout << " x = \n";
	_for(i, 0, K) {
		cout << "[";
		_for(j, 0, n - 1)
			cout << (int)(x[i][j] + 0.1) << ", ";
		cout << (int)(x[i][n - 1] + 0.1) << "]^T" << endl;
	}
	cout << "\n s=(";
	_for(i, 0, n - 1)
		cout << s[i] << ", ";
	cout << s[n - 1] << ")^T\n";
	cout << "平均相对误差 e = " << e << endl;
	cout << "乘除法次数：" << num_mul_div << endl;
	cout << "加减法次数：" << num_add_sub << "\n\n\n";

	return 0;
}