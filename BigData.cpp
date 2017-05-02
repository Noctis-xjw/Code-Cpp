#include "BigData.h"

const INT64 UN_INT = 0xcccccccccccccccc;
const INT64 MAXValue = 9223372036854775807;
const INT64 MINValue = (9223372036854775807 + 1);

BigData::BigData(INT64 value = UN_INT)
:_value(value)
{
	//�����ַŵ��ַ�����
	char symbol = '+';
	//ֻ�е���_valueС��0ʱ��valueӦ�ñ�Ϊ��������Ϊ�����Ѿ��洢�����԰�����ͳһȫ����Ϊ��������
	if (_value < 0)
	{
		symbol = '-';
		_value = 0 - _value;
	}

	_strData.append(1, symbol);
	int count = 1;

	if (_value == 0)
	{
		_strData.append(1, '0');
		return;
	}

	while (_value > 0)
	{
		char temp = _value % 10 + '0';
		_value = _value / 10;
		_strData.append(1, temp);
	}
	std::reverse(_strData.begin() + 1, _strData.end());
}

BigData::BigData(const std::string& strData)
: _value(0)
, _strData("+0")
{
	//����ַ���Ϊ��
	if (strData.empty())
		return;

	//"      012345"  "        "
	//�������հ��ַ�
	char* pData = (char*)strData.c_str();
	while (isspace(*pData))
		pData++;

	if (*pData == '\0')
		return;

	//"+1234567890" "-123456789"
	//�ѿհ��ַ��������޷Ǿ��������ַ����߷���λ
	//���ֱ���Ƿ���λ������ �����������ַ���ֱ�Ӱ���������
	char symbol = *pData;
	if (*pData == '+' || *pData == '-')
		pData++;
	else if (*pData >= '0' && *pData <= '9')
		symbol = '+';
	else return;

	//���������ַ�ǰ�õ�0
	//���ж��������ַ�����ֻ��һ��0
	if (strData.size() == 2 && '\0' == *pData)
	{
		_value = 0;
		return;
	}

	while ('0' == *pData)
		pData++;

	if ('\0' == pData)
		return;

	//������Ǵ洢�ַ���
	_strData.resize(strlen(pData) + 1);
	_strData[0] = symbol;

	size_t count = 1;
	while (*pData >= '0' && *pData <= '9')
	{
		_value = _value * 10 + *pData - '0';
		_strData[count++] = *pData;
		pData++;
	}

	if (symbol == '-')
		_value = 0 - _value;

	_strData.resize(count);
}

BigData BigData::operator+(const BigData& b)
{
	if (!IsINT64OverFlow() && !b.IsINT64OverFlow())
	{
		//��������෴ ֱ����Ӽ��� ��Ϊ������β��ᳬ����Χ
		if (_strData[0] != b._strData[0])
			return BigData(_value + b._value);
		else
		{
			//���������ͬ ���Ϊͬ������ͬ�� �����ͬ����������Χ��ֱ����Ӽ���
			//ͬ�� 10  >= 2+8
			//ͬ�� -1 + -2 <= -10
			if ((_strData[0] == '+' && MAXValue - _value >= b._value) ||
				(_strData[0] == '-' && MINValue - _value <= b._value))
				return BigData(_value + b._value);
		}
	}
	//���һ����������������Χ��
	if (_strData[0] == b._strData[0])
		return BigData(Add(_strData, b._strData));
	else
		return BigData(Sub(_strData, b._strData));
}

BigData BigData::operator-(BigData& b)
{
	if (!IsINT64OverFlow() && !b.IsINT64OverFlow())
	{
		//�ڷ�Χ���ҷ�����ͬ ֱ�Ӵ�����С������
		if (_strData[0] == b._strData[0])
			return Sub(_strData, b._strData);
		else
		{
			//������Ų�ͬ ���Ϊͬ������ͬ�� �����ͬ����������Χ��ֱ����Ӽ���
			//ͬ�� 10  >= 2+8
			//ͬ�� -1 + -2 <= -10
			if ((_strData[0] == '+' && b._strData[0] == '-' && MAXValue >= _value - b._value) ||
				(_strData[0] == '-' && b._strData[0] == '+' && MINValue <= _value - b._value))
			{
				b._value = 0 - b._value;
				return BigData(_value + b._value);
			}
		}
	}
	//���һ����������������Χ��
	//��ʱ��Ϊ�������
	//(-1) - (-1) = (-1) + 1
	//1 - 1  = 1 +(-1)
	//�����������Ӧ�õ��ü��� ���߷�����ͬ
	//1-(-1) = 1 + 1
	//(-1) - 1 = -1 + (-1)
	//�����������Ų�ͬ ����ȹ��ڶ���Ϊ- ���Ϊ�� ���� ��Ϊ- ���üӷ�
	//���Կ�д���´���
	if (_strData[0] == b._strData[0])
	{
		return BigData(Sub(_strData, b._strData));
	}
	else
	{
		if (b._strData[0] == '-')
			b._strData[0] = '+';
		else
			b._strData[0] = '-';
		return BigData(Add(_strData, b._strData));
	}

}

BigData BigData::operator*(BigData& b)
{
	if (_value == 0 || b._value == 0)
		return BigData(0);
	else if (strcmp(_strData.c_str() + 1, "1") == 0)
	{
		if (_strData[0] == '-')
			b._strData[0] = '-';
		return BigData(b._strData);
	}

	else if (strcmp(b._strData.c_str() + 1, "1") == 0)
	{
		if (_strData[0] == '-')
			_strData[0] = '-';
		return BigData(_strData);
	}

	else
		return BigData(Mul(_strData, b._strData));
}

BigData BigData::operator/(const BigData& b)
{
	//������Ϊ0��-1,1������������

	//��ĸ����Ϊ0
	if (b._value == 0)
	{
		cout << "��������Ϊ0" << endl;
		return BigData(0);
	}

	if (!IsINT64OverFlow() && !b.IsINT64OverFlow())
		return BigData(_value / b._value);

	//��Ϊ0 ����Ϊ0
	//����С�ڷ�ĸ
	if ("+0" == _strData || _strData.size() < b._strData.size() ||
		(_strData.size() == b._strData.size() && strcmp(_strData.c_str() + 1, b._strData.c_str() + 1)))
		return BigData(0);

	//����1
	if (strcmp(_strData.c_str() + 1, b._strData.c_str() + 1) == 0)
	{
		BigData bg(1);
		if (_strData[0] != b._strData[0])
		{
			bg._value = -1;
			bg._strData[0] = '-';
		}

		return bg;
	}

	//����ԭ����ֵ
	if (strcmp(b._strData.c_str() + 1, "1") == 0)
	{
		BigData bg(_strData);
		if ('-' == b._strData[0])
		{
			if (_strData[0] == '+')
				bg._strData[0] = '-';
			else
				bg._strData[0] = '+';
		}
		return bg;
	}

	//������ǵ��ó���
	return Div(_strData, b._strData);
}

std::string BigData::Div(std::string left, std::string right)
{
	char symbol = '+';
	if (left[0] != right[0])
		symbol = '-';
	std::string strRet;
	strRet[0] = symbol;

	char* Pleft = (char*)left.c_str() + 1;
	char* Pright = (char*)right.c_str() + 1;
	int Lsize = left.size() - 1;
	int Rsize = right.size() - 1;
	int len = Lsize;

	while (*(Pleft + len - 1) != '\0')
	{
		//�����߱��ұ�С �����һλ��Ϊ0 
		if (!IsLeftBig(Pleft, Lsize, Pright, Rsize))
		{
			len++;
			strRet.append(1, '0');
			continue;
		}
		//�����߱��ұߴ� �����
		else{
			strRet.append(1, SubLoop(Pleft, len, Pright, Rsize));
			len++;
		}
	}
	return strRet;
}

std::string BigData::Add(std::string left, std::string right)
{
	int LSize = left.size();
	int RSize = right.size();

	//��λ�����Ļ������
	if (LSize < RSize)
	{
		std::swap(left, right);
		std::swap(LSize, RSize);
	}

	std::string strRet;
	//����Ҫ��һλ�Ƿ���λ
	strRet.resize(LSize + 1);
	strRet[0] = left[0];

	char step = 0;
	for (int idx = 1; idx < LSize; ++idx)
	{
		char temp = left[LSize - idx] - '0' + step;
		if (RSize > idx)
			temp += right[RSize - idx] - '0';
		step = temp / 10;
		strRet[LSize + 1 - idx] = temp % 10 + '0';
	}
	strRet[1] = step + '0';
	return strRet;
}

//��Ϊ������ʱ�� ����Ϊһ��һ����� ����Ϊͬ�����
std::string BigData::Sub(std::string left, std::string right)
{
	int LSize = left.size();
	int RSize = right.size();
	char symbol = '+';

	//��ͬ�� �ѽϴ�����ַŵ����
	if (LSize < RSize ||
		(LSize == RSize && left < right))
	{
		std::swap(left, right);
		std::swap(LSize, RSize);
		if (right[0] == '+')
			symbol = '-';
	}
	//��������������Ǹ��� ��ô����һ���ȵڶ������ʱ�� ���Ϊ-
	else if (right[0] == '-' && strcmp(left.c_str(), right.c_str()) > 0)
		symbol = '-';

	std::string strRet;
	strRet.resize(LSize + 1);
	strRet[0] = symbol;

	char step = 0;
	char temp;
	int idx;
	for (idx = 1; idx < LSize; ++idx)
	{
		temp = left[LSize - idx] - '0';
		if (RSize > idx)
			temp -= right[RSize - idx] - '0';
		//��λ
		if (temp < 0)
		{
			left[LSize - idx - 1] -= 1;
			temp += 10;
		}
		strRet[LSize - idx + 1] = temp + '0';
	}
	strRet[LSize - idx + 1] = temp + '0';
	return strRet;
}

std::string BigData::Mul(std::string left, std::string right)
{
	int LSize = left.size();
	int RSize = right.size();
	char symbol = '+';

	if (left[0] != right[0])
		symbol = '-';

	//�ѽ�С�����ַŵ���� ���ѭ����
	if (LSize > RSize)
	{
		std::swap(left, right);
		std::swap(LSize, RSize);
	}

	size_t resSize = LSize + RSize - 1;
	std::string strRet(resSize, '0');
	strRet[0] = symbol;

	char offset = 0;//ÿһ�γ˶���Ҫ��λ
	char step = 0;//��λ

	//����ѭ�� һ���ǽ�λ һ����ÿһ�����
	for (int i = 1; i < LSize; ++i)
	{
		char cLeft = left[LSize - i] - '0';
		step = 0;
		if (cLeft == 0)
		{
			//�����ߵ�ĳһλΪ�� ��ֱ����λ ����Ҫ�ٰ�����
			offset++;
			continue;
		}

		for (int j = 1; j < RSize; ++j)
		{
			//����cleft�Ѿ������ַ� ���������� ����right�����ַ�
			char temp = cLeft * (right[RSize - j] + step - '0');
			temp = temp + strRet[LSize + RSize - 1 - j - offset] - '0';//tempΪ��˺��������֮�� ��offset��Ϊ�˷�ֹ0��λ��ʱ��û�м�
			step = temp / 10;//��λ��
			strRet[LSize + RSize - 1 - j - offset] = temp % 10 + '0';//���λ��
		}
		//ÿһ��ѭ������ǰ��һλ
		offset++;
	}
	//��ΪLsize Ӧ�û�����Ľ����һλ ����©��һλ
	strRet[1] = step + '0';
	return strRet;
}

bool BigData::IsLeftBig(char*& Pleft, int& LSize, char*&Pright, int & RSize)
{
	if (LSize > RSize || (LSize == RSize && strncmp(Pleft, Pright, LSize) == 0))
		return true;
	return false;
}

char BigData::SubLoop(char*&Pleft, int&Lsize, char*&Pright, int&Rsize)
{
	//����Ҳ�൱�������ļ���
	//countΪ���˼���
	char count = '0';
	while (IsLeftBig(Pleft, Lsize, Pright, Rsize))
	{
		for (int i = 0; i < Lsize; ++i)
		{
			char temp = Pleft[Lsize - 1 - i] - '0';
			if (i < Rsize)
				temp -= Pright[Rsize - 1 - i] - '0';

			//��Ϊ��ߵ����ֽϴ� �����������λ
			if (temp < 0)
			{
				Pleft[Lsize - i - 2] -= 1;
				temp += 10;
			}

			Pleft[Lsize - i - 1] = temp + '0';
		}
		count++;
	}
	//��ǰ��λΪ0��ʱ��
	while ('0' == *Pleft&&Lsize > 0)
	{
		Pleft++;
		Lsize--;
	}
	return count;
}

bool BigData::IsINT64OverFlow() const
{
	std::string strTemp = "+9223372036854775807";
	if (_strData[0] == '-')
		strTemp = "-9223372036854775808";

	if (_strData.size() < strTemp.size())
		return false;
	else if (_strData.size() == strTemp.size() && _strData > strTemp)
		return false;
	return true;
}
