//#define _CRT_SECURE_NO_WARNINGS 1
//
//#include<windows.h>
//#include<iostream>
//#include <time.h>
//
//using namespace std;
//
//void Test()
//{
//	int i = 0;
//	int cout = 0;
//	for (i = 0;i<10000000;i++)
//	{
//		cout++;
//	}
//}
//
//void FunTest()
//{
//	//�涨Ҫ���������
//	//ԭ�����£�
//	//typedef   union   _LARGE_INTEGER
//	//{
//	//	struct
//	//	{
//	//		DWORD   LowPart;   //   4�ֽ�������  
//	//		LONG  HighPart;   //   4�ֽ�������  
//	//	};  
//	//	LONGLONG  QuadPart;  
//
//	//	//   8�ֽ�������  
//	//}   LARGE_INTEGER;  
//	LARGE_INTEGER test;
//	LONGLONG time1,time2;
//	double dfMinus,dfFreq,dfTim;
//	//�ȼ�������ʱ��Ƶ��
//	QueryPerformanceFrequency(&test);
//	dfFreq = (double)test.QuadPart;
//	//Ȼ�������ʼʱ��
//	QueryPerformanceCounter(&test);
//	time1 = test.QuadPart;
//
//	Test();
//
//	//�����βʱ��
//	QueryPerformanceCounter(&test);
//	time2 = test.QuadPart;
//	dfMinus = (double)(time2-time1);
//	//��ֵ��ʱ��Ƶ�ʿɼ��������ʱ��
//	dfTim = dfMinus/dfFreq;
//	cout<<dfTim<<endl;
//}
//
//int main()
//{
//	FunTest();
//	system("pause");	
//	return 0;
//}