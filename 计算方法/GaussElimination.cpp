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

vector<vector<double>> ReadMatrixFromFile(string); // ���ļ���ȡһ��m��n�еľ���
vector<vector<double>> RandomlyGeneratedMatrix();  // �������һ��n��n+1�еķ���
void PrintMatrix(vector<vector<double>>&);         // ��ӡ����
void Filtration(vector<vector<double>>&);          // ģ����Ϊ���˲��в����ĸ���
int GaussEliminationWithoutSelection();            // ������Ԫ�ĸ�˹��ȥ��������Է�����
int GaussElimination();                            // ����Ԫ�صĸ�˹��ȥ��������Է�����
int DoolittleWithoutSelection();                   // ��ѡ��Ԫ�����ض��ֽ�������Է�����
int Doolittle();                                   // ѡ��Ԫ�Ķ����ض��ֽ�������Է�����


int main()
{
	int type = 5;
	while (type) {
		cout << "����1�����в�����Ԫ�ĸ�˹��ȥ����\n����2����������Ԫ�صĸ�˹��ȥ����\n����3�����в�ѡ��Ԫ�����ض��ֽ⣻\n����4������ѡ��Ԫ�Ķ����ض��ֽ⣻\n����0�����������\n";
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
			cout << "������0-4��";
		}
	}
	return 0;
}

// ʶ���ļ��еľ����м��м��У�Ȼ���ȡ����
vector<vector<double>> ReadMatrixFromFile(string fileName) {

	fstream inputfile;
	inputfile.open(fileName, ios::in | ios::binary);
	if (!inputfile.is_open()) {
		cerr << "File open error!\n";
		return {};
	}

	char c;
	int n_row = 1;                     // ����
	inputfile.seekg(0, ios::end);      // ָ��ĩβ
	streampos sp = inputfile.tellg();  // ��ȡ�ļ��ַ�����

	inputfile.seekg(0, ios::beg);      // ָ���ļ�ͷ
	while (inputfile.tellg() != sp)    // ���������getc��ȥ����޷�ָ��ͷ���ˣ����Ե�sp��ɲ��   
	{
		inputfile.get(c);
		if (c == '\n') n_row++;        // �ļ����������������
	}

	inputfile.seekg(0, ios::beg);      // �ص��ļ�ͷ
	int n_col = 1;                     // ����
	while (inputfile.tellg() != sp) {
		inputfile.get(c);
		if (c == ' ')
			n_col++;
		else if (c == '\n')
			break;
	}

	inputfile.seekg(0, ios::beg);      // �ص��ļ�ͷ
	vector<vector<double>> a(n_row, vector<double>(n_col));
	_for(i, 0, n_row)
		_for(j, 0, n_col)
		inputfile >> a[i][j];          // ��ȡ����
	inputfile.close();
	PrintMatrix(a);
	return a;
}

vector<vector<double>> RandomlyGeneratedMatrix() {
	cout << "���������������";
	int n;
	cin >> n;
	cout << "���������������";
	int m;
	cin >> m;
	vector<vector<double>> a(n, vector<double>(m));
	cout << "������������ֵ��";
	double maxv;
	cin >> maxv;
	cout << "�����������Сֵ��";
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
	cout << "������Ԫ�ĸ�˹��ȥ��\n\n������ɾ���������1�����ļ���ȡ����������2��";
	cin >> gw;                         // 1����������ɣ�2������ļ���ȡ

	vector<vector<double>> A;          // ��Դʹ�þ���N
	vector<vector<double>> b;          // ��Դ��������n^T
	vector<vector<double>> r;          // ��ҵ������������r
	vector<double> s;                  // ��ҵ�������  
	string fileName = "";
	if (gw == 1) {
		A = RandomlyGeneratedMatrix();
		b = RandomlyGeneratedMatrix();
		r = RandomlyGeneratedMatrix();
	}
	else {
		/*cout << "��������Դʹ�þ���N���ļ�·����";
		cin >> fileName;
		A = ReadMatrixFromFile(fileName);
		cout << "��������Դ��������n���ļ�·����";
		cin >> fileName;
		b = ReadMatrixFromFile(fileName);
		cout << "��������ҵ������������r���ļ�·����";
		cin >> fileName;
		r = ReadMatrixFromFile(fileName);*/
		A = ReadMatrixFromFile("A.txt");
		b = ReadMatrixFromFile("b.txt");
		r = ReadMatrixFromFile("r.txt");
	}

	int n = A.size();                  // ��Դ����
	int K = b[0].size();               // ����������
	s.resize(r.size());
	if (n != b.size()) {
		cerr << "��Դʹ�þ���N����������Դ��������������ͬ���޷���⡣\n";
		return 3;
	}
	if (n != A[0].size()) {
		cerr << "��Դʹ�þ����Ƿ���\n";
		return 4;
	}
	if (n != r.size()) {
		cerr << "��ҵ����������������������ҵ������\n";
		return 5;
	}

	double run_time;
	LARGE_INTEGER time_start;	           // ��ʼʱ��
	LARGE_INTEGER time_over;	           // ����ʱ��
	double dqFreq;		                   // ��ʱ��Ƶ��
	LARGE_INTEGER f;	                   // ��ʱ��Ƶ��
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	QueryPerformanceCounter(&time_start);// ��ʱ��ʼ

	int num_mul_div = 0;                   // �˳�������
	int num_add_sub = 0;                   // �Ӽ�������
	vector<vector<double>> x(K, vector<double>(n));     // K��������Ľ�  
	vector<vector<double>> a(n, vector<double>(n + 1)); // ��A��b��ÿһ�кϲ������������

	Filtration(b);

	_for(q, 0, K) {                        // �������ÿ��������

		_for(i, 0, n)
			_for(j, 0, n)
			a[i][j] = A[i][j];
		_for(i, 0, n)                      // �������
			a[i][n] = b[i][q];

		double det = 1.0;
		_for(k, 0, n - 1) {
			
			if (a[k][k] == 0) {
				cerr << "��ʱ�޷�ʹ�ø�˹��ȥ!\n";
				return 1;
			}

			double m;
			_for(i, k + 1, n) {            // ��Ԫ����
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
				cout << "�������⣡\n";
			else cerr << "�������޽⣡\n";
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

	_for(i, 0, n) {                        // ����ÿ����ҵ��������������ı���
		int rSum = 0;
		_for(j, 0, K)
			rSum += (int)(x[j][i] + 0.1);    // x[j][i]���ܺܽӽ�����һ��������ϵͳ��
		s[i] = min(rSum, r[i][0]) / r[i][0];
	}
	num_mul_div += n;
	num_add_sub += K * n;

	QueryPerformanceCounter(&time_over);   // ��ʱ����
	run_time = 1000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
	printf("\n����ʱ�䣺%.3fms\n", run_time);

	double e = 0.0;                        // ƽ��������
	_for(q, 0, K)
		_for(i, 0, n) {
		double dc = 0.0;
		_for(j, 0, n)
			dc += A[i][j] * (int)(x[q][j] + 0.1);       // Ҫ��ԭʼ��A������
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
	cout << "ƽ�������� e = " << e << endl;
	cout << "�˳���������" << num_mul_div << endl;
	cout << "�Ӽ���������" << num_add_sub << "\n\n\n";

	return 0;
}

int GaussElimination() {
	int gw = 1;
	cout << "����Ԫ�ĸ�˹��ȥ��\n\n������ɾ���������1�����ļ���ȡ����������2��";
	cin >> gw;                         // 1����������ɣ�2������ļ���ȡ

	vector<vector<double>> A;          // ��Դʹ�þ���N
	vector<vector<double>> b;          // ��Դ��������n^T
	vector<vector<double>> r;          // ��ҵ������������r
	vector<double> s;                  // ��ҵ�������  
	string fileName = "";
	if (gw == 1) {
		A = RandomlyGeneratedMatrix();
		b = RandomlyGeneratedMatrix();
		r = RandomlyGeneratedMatrix();
	}
	else {
		/*cout << "��������Դʹ�þ���N���ļ�·����";
		cin >> fileName;
		A = ReadMatrixFromFile(fileName);
		cout << "��������Դ��������n���ļ�·����";
		cin >> fileName;
		b = ReadMatrixFromFile(fileName);
		cout << "��������ҵ������������r���ļ�·����";
		cin >> fileName;
		r = ReadMatrixFromFile(fileName);*/
		A = ReadMatrixFromFile("A.txt");
		b = ReadMatrixFromFile("b.txt");
		r = ReadMatrixFromFile("r.txt");
	}

	int n = A.size();                  // ��Դ����
	int K = b[0].size();               // ����������
	s.resize(r.size());
	if (n != b.size()) {
		cerr << "��Դʹ�þ���N����������Դ��������������ͬ���޷���⡣\n";
		return 3;
	}
	if (n != A[0].size()) {
		cerr << "��Դʹ�þ����Ƿ���\n";
		return 4;
	}
	if (n != r.size()) {
		cerr << "��ҵ����������������������ҵ������\n";
		return 5;
	}

	double run_time;
	LARGE_INTEGER time_start;	           // ��ʼʱ��
	LARGE_INTEGER time_over;	           // ����ʱ��
	double dqFreq;		                   // ��ʱ��Ƶ��
	LARGE_INTEGER f;	                   // ��ʱ��Ƶ��
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	QueryPerformanceCounter(&time_start);// ��ʱ��ʼ

	int num_mul_div = 0;                   // �˳�������
	int num_add_sub = 0;                   // �Ӽ�������
	vector<vector<double>> x(K, vector<double>(n));     // K��������Ľ�  
	vector<vector<double>> a(n, vector<double>(n + 1)); // ��A��b��ÿһ�кϲ������������

	Filtration(b);

	_for(q, 0, K) {                        // �������ÿ��������

		_for(i, 0, n)
			_for(j, 0, n)
			a[i][j] = A[i][j];
		_for(i, 0, n)                      // �������
			a[i][n] = b[i][q];

		double det = 1.0;
		_for(k, 0, n - 1) {
			int maxi = k;
			_for(i, k+1, n)                  // ѡ����Ԫ
				if (abs(a[i][k]) > abs(a[maxi][k]))
					maxi = i;
			if (a[maxi][k] == 0) {
				cerr << "��ʱ�޷�ʹ�ø�˹��ȥ!\n";
				return 1;
			}
			if (maxi != k) {               // ��������
				double swap;
				_for(i, 0, n + 1) {
					swap = a[k][i];
					a[k][i] = a[maxi][i];
					a[maxi][i] = swap;
				}
				det = -det;                // ����ʽ��ֵ���
			}

			double m;
			_for(i, k + 1, n) {            // ��Ԫ����
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
				cout << "�������⣡\n";
			else cerr << "�������޽⣡\n";
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

	_for(i, 0, n) {                        // ����ÿ����ҵ��������������ı���
		int rSum = 0;
		_for(j, 0, K)
			rSum += (int)(x[j][i] + 0.1);    // x[j][i]���ܺܽӽ�����һ��������ϵͳ��
		s[i] = min(rSum, r[i][0]) / r[i][0];
	}
	num_mul_div += n;
	num_add_sub += K * n;

	QueryPerformanceCounter(&time_over);   // ��ʱ����
	run_time = 1000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
	printf("\n����ʱ�䣺%.3fms\n", run_time);

	double e = 0.0;                        // ƽ��������
	_for(q, 0, K)
		_for(i, 0, n) {
		double dc = 0.0;
		_for(j, 0, n)
			dc += A[i][j] * (int)(x[q][j] + 0.1);       // Ҫ��ԭʼ��A������
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
	cout << "ƽ�������� e = " << e << endl;
	cout << "�˳���������" << num_mul_div << endl;
	cout << "�Ӽ���������" << num_add_sub << "\n\n\n";

	return 0;
}

int DoolittleWithoutSelection() {
	int gw = 1;
	cout << "��ѡ��Ԫ��LU�ֽⷨ\n\n������ɾ���������1�����ļ���ȡ����������2��";
	cin >> gw;                         // 1����������ɣ�2������ļ���ȡ

	vector<vector<double>> A;          // ��Դʹ�þ���N
	vector<vector<double>> b;          // ��Դ��������n^T
	vector<vector<double>> re;         // ��ҵ������������r
	vector<double> s;                  // ��ҵ�������  
	string fileName = "";
	if (gw == 1) {
		A = RandomlyGeneratedMatrix();
		b = RandomlyGeneratedMatrix();
		re = RandomlyGeneratedMatrix();
	}
	else {
		/*cout << "��������Դʹ�þ���N���ļ�·����";
		cin >> fileName;
		A = ReadMatrixFromFile(fileName);
		cout << "��������Դ��������n���ļ�·����";
		cin >> fileName;
		b = ReadMatrixFromFile(fileName);
		cout << "��������ҵ������������r���ļ�·����";
		cin >> fileName;
		r = ReadMatrixFromFile(fileName);*/
		A = ReadMatrixFromFile("A.txt");
		b = ReadMatrixFromFile("b.txt");
		re = ReadMatrixFromFile("r.txt");
	}

	int n = A.size();                  // ��Դ����
	int K = b[0].size();               // ����������
	s.resize(re.size());
	if (n != b.size()) {
		cerr << "��Դʹ�þ���N����������Դ��������������ͬ���޷���⡣\n";
		return 3;
	}
	if (n != A[0].size()) {
		cerr << "��Դʹ�þ����Ƿ���\n";
		return 4;
	}
	if (n != re.size()) {
		cerr << "��ҵ����������������������ҵ������\n";
		return 5;
	}

	double run_time;
	LARGE_INTEGER time_start;	           // ��ʼʱ��
	LARGE_INTEGER time_over;	           // ����ʱ��
	double dqFreq;		                   // ��ʱ��Ƶ��
	LARGE_INTEGER f;	                   // ��ʱ��Ƶ��
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	QueryPerformanceCounter(&time_start);// ��ʱ��ʼ

	int num_mul_div = 0;                   // �˳�������
	int num_add_sub = 0;                   // �Ӽ�������
	vector<vector<double>> x(K, vector<double>(n));     // K��������Ľ�  
	vector<vector<double>> a(n, vector<double>(n + 1)); // ��A��b��ÿһ�кϲ������������
	
	Filtration(b);
	_for(i, 0, n)
		_for(j, 0, n)
		a[i][j] = A[i][j];

	_for(r, 0, n) {                    // LU�ֽ�(ֻҪ�ֽ�һ�Σ�������

		_for(i, r, n)                  // ����si
		{
			_for(k, 0, r)
				a[i][r] = a[i][r] - a[i][k] * a[k][r];
			num_mul_div += r;
			num_add_sub += r;
		}

		_for(i, r + 1, n)              // ����Lir
			a[i][r] = a[i][r] / a[r][r];
		_for(i, r + 1, n)              // ����Uri
			_for(k, 0, r)
			a[r][i] = a[r][i] - a[r][k] * a[k][i];
		num_mul_div += (n - r - 1) * (1 + r);
		num_add_sub += (n - r - 1) * r;
	}

	_for(q, 0, K) {                        // �������ÿ��������

		_for(i, 0, n)                      // �������
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

	_for(i, 0, n) {                        // ����ÿ����ҵ��������������ı���
		int rSum = 0;
		_for(j, 0, K)
			rSum += (int)(x[j][i] + 0.1);    // x[j][i]���ܺܽӽ�һ��������ϵͳ��ʾ����
		s[i] = min(rSum, re[i][0]) / re[i][0];
	}
	num_mul_div += n;
	num_add_sub += K * n;

	QueryPerformanceCounter(&time_over);   // ��ʱ����
	run_time = 1000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
	printf("\n����ʱ�䣺%.3fms\n", run_time);

	double e = 0.0;                        // ƽ��������
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
	cout << "ƽ�������� e = " << e << endl;
	cout << "�˳���������" << num_mul_div << endl;
	cout << "�Ӽ���������" << num_add_sub << "\n\n\n";

	return 0;
}

int Doolittle() {                        // ѡ��Ԫ�Ķ����ض����Ƿֽⷨ
	int gw = 1;
	cout << "ѡ��Ԫ��LU�ֽⷨ\n\n������ɾ���������1�����ļ���ȡ����������2��";
	cin >> gw;                         // 1����������ɣ�2������ļ���ȡ

	vector<vector<double>> A;          // ��Դʹ�þ���N
	vector<vector<double>> b;          // ��Դ��������n^T
	vector<vector<double>> re;         // ��ҵ������������r
	vector<double> s;                  // ��ҵ�������  
	string fileName = "";
	if (gw == 1) {
		A = RandomlyGeneratedMatrix();
		b = RandomlyGeneratedMatrix();
		re = RandomlyGeneratedMatrix();
	}
	else {
		/*cout << "��������Դʹ�þ���N���ļ�·����";
		cin >> fileName;
		A = ReadMatrixFromFile(fileName);
		cout << "��������Դ��������n���ļ�·����";
		cin >> fileName;
		b = ReadMatrixFromFile(fileName);
		cout << "��������ҵ������������r���ļ�·����";
		cin >> fileName;
		r = ReadMatrixFromFile(fileName);*/
		A = ReadMatrixFromFile("A.txt");
		b = ReadMatrixFromFile("b.txt");
		re = ReadMatrixFromFile("r.txt");
	}

	int n = A.size();                  // ��Դ����
	int K = b[0].size();               // ����������
	s.resize(re.size());
	if (n != b.size()) {
		cerr << "��Դʹ�þ���N����������Դ��������������ͬ���޷���⡣\n";
		return 3;
	}
	if (n != A[0].size()) {
		cerr << "��Դʹ�þ����Ƿ���\n";
		return 4;
	}
	if (n != re.size()) {
		cerr << "��ҵ����������������������ҵ������\n";
		return 5;
	}

	double run_time;
	LARGE_INTEGER time_start;	           // ��ʼʱ��
	LARGE_INTEGER time_over;	           // ����ʱ��
	double dqFreq;		                   // ��ʱ��Ƶ��
	LARGE_INTEGER f;	                   // ��ʱ��Ƶ��
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	QueryPerformanceCounter(&time_start);// ��ʱ��ʼ

	int num_mul_div = 0;                   // �˳�������
	int num_add_sub = 0;                   // �Ӽ�������
	vector<vector<double>> x(K, vector<double>(n));     // K��������Ľ�  
	vector<vector<double>> a(n, vector<double>(n + 1)); // ��A��b��ÿһ�кϲ������������
	vector<int> Ip(n);                     // ��¼�������

	Filtration(b);
	_for(i, 0, n)
		_for(j, 0, n)
		a[i][j] = A[i][j];

	_for(r, 0, n) {                    // LU�ֽ�(ֻҪ�ֽ�һ�Σ�������

		_for(i, r, n)                  // ����si
		{
			_for(k, 0, r)
				a[i][r] = a[i][r] - a[i][k] * a[k][r];
			num_mul_div += r;
			num_add_sub += r;
		}

		int maxi = r;
		_for(i, r + 1, n)              // ѡ��Ԫ
			if (a[maxi][r] < a[i][r])
				maxi = i;
		Ip[r] = maxi;

		double temp;
		_for(i, 0, n + 1)              // ��������
		{
			temp = a[r][i];
			a[r][i] = a[maxi][i];
			a[maxi][i] = temp;
		}

		_for(i, r + 1, n)              // ����Lir
			a[i][r] = a[i][r] / a[r][r];
		_for(i, r + 1, n)              // ����Uri
			_for(k, 0, r)
			a[r][i] = a[r][i] - a[r][k] * a[k][i];
		num_mul_div += (n - r - 1) * (1 + r);
		num_add_sub += (n - r - 1) * r;
	}

	_for(q, 0, K) {                        // �������ÿ��������

		_for(i, 0, n)                      // �������
			a[i][n] = b[i][q];

		_for(i, 0, n)                      // ���佻����
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

	_for(i, 0, n) {                        // ����ÿ����ҵ��������������ı���
		int rSum = 0;
		_for(j, 0, K)
			rSum += (int)(x[j][i] + 0.1);    // x[j][i]���ܺܽӽ�һ��������ϵͳ��ʾ����
		s[i] = min(rSum, re[i][0]) / re[i][0];
	}
	num_mul_div += n;
	num_add_sub += K * n;

	QueryPerformanceCounter(&time_over);   // ��ʱ����
	run_time = 1000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
	printf("\n����ʱ�䣺%.3fms\n", run_time);

	double e = 0.0;                        // ƽ��������
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
	cout << "ƽ�������� e = " << e << endl;
	cout << "�˳���������" << num_mul_div << endl;
	cout << "�Ӽ���������" << num_add_sub << "\n\n\n";

	return 0;
}