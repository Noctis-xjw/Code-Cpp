#pragma once

#define _CRT_SECURE_NO_WARNINGS 1

typedef int DateType;

struct Node
{
	Node(const DateType &data = 0)
		:_data(data)
		,_pNext(NULL)
		,_pPre(NULL)
	{}
	

	DateType _data;
	Node *_pNext;
	Node *_pPre;
};

class List
{
public:
	List()
		:_pHead(NULL)
		,_pTail(NULL)
		,_size(0)
	{}

	
	List(DateType arr[],size_t size)
		:_size(size)//���ﻻ��sizeҲ�ǶԵ�
	{
		for (size_t idx = 0; idx<size ; ++idx)
		{
			PushBack(arr[idx]);
		}
	}
	
	List(const List& l)
		:_pHead(NULL)
		,_pTail(NULL)
		,_size(0)
	{	
		Node *pCurNode = l._pHead;
		while (pCurNode)
		{
			PushBack(pCurNode->_data);
			pCurNode = pCurNode->_pNext;
		}
	}

	List& operator=(const List &l)
	{
		if (&l != this)
		{
			Clear();
			Node *pCurNode = l._pHead;
			while (pCurNode)
			{
				PushBack(pCurNode->_data);
				pCurNode = pCurNode->_pNext;
			}
		}
		return *this;
	}

	~List()
	{
		Clear();
	}


	Node* BuyNode(const DateType& data)
	{
		return new Node(data);
	}

	//β�� 
	//������� ���Ϊ�� �����Ϊ��
	void PushBack(const DateType& data)
	{
		if (NULL == _pHead)
		{
			_pHead = _pTail = BuyNode(data);
		}
		else 
		{
			Node* pNewNode = BuyNode(data);
			_pTail->_pNext = pNewNode;
			pNewNode->_pPre = _pTail;
			_pTail = pNewNode;
		}
		_size++;
	}

	//βɾ
	//������� һ��Ϊ�� һ��ֻ��һ����� һ���д��������Ľ��
	void PopBack()
	{
		if (NULL == _pHead)
		{
			return;
		}
		else if (_pHead == _pTail)
		{
			delete _pHead;
			_pHead = _pTail = NULL;
		}
		else
		{
			_pTail = _pTail->_pPre;
			delete _pTail->_pNext;
			_pTail->_pNext = NULL;
		}
		--_size;
	}
	
	//ͷ��
	//������� һ��Ϊ�� һ�ֲ�Ϊ��
	void PushFront(const DateType &data)
	{
		if(_pHead == NULL)
		{
			_pHead = _pTail = BuyNode(data);
		}
		else
		{
			Node *pNewNode = BuyNode(data);
			pNewNode->_pNext = _pHead;
			_pHead->_pPre = pNewNode;
			_pHead = pNewNode;
		}
		_size++;
	}

	//ͷɾ
	//������� ��һ���������Ϊ�� �ڶ������ֻ��һ����� ���������
	void PopFront()
	{
		if (_pHead == NULL)
		{
			return ;
		}
		else if (_pHead == _pTail)
		{
			delete _pHead;
			_pHead = _pTail = NULL;
		}
		else
		{
			_pHead = _pHead->_pNext;
			delete _pHead->_pPre;
			_pHead->_pPre = NULL;
		}
		--_size;
	}
	
	//����
	void Clear()
	{
		Node *pCurNode = _pHead;
		Node *pCurNext = _pHead;
		while (pCurNode)
		{
			pCurNext = pCurNode->_pNext;
			delete pCurNode;
			pCurNode = pCurNext;
		}
		_size = 0;
		_pHead = _pTail = NULL;
	}

	//����һ�����
	Node* Find(const DateType &data)const
	{
		Node *pCurNode = _pHead;
		while (pCurNode)
		{
			if (pCurNode->_data == data)
			{
				return pCurNode;
			}
			else
				pCurNode = pCurNode->_pNext;
		}
		return NULL;
	}

	//����
	void Insert(Node *pos,DateType &data)
	{
		if (pos == NULL)
		{
			return;
		}
		if(pos == _pTail)
		{
			PushBack(data);	
			++_size;
		}
		else
		{
			Node *pNewNode = BuyNode(data);
			pNewNode->_pNext = pos->_pNext;
			pos->_pNext->_pPre = pNewNode;
			pos->_pNext = pNewNode;
			pNewNode->_pPre = pos;
			++_size;
		}
	}


	//ɾ��ĳһ�����
	void Erase(Node *pos)
	{
		if (NULL == pos)
		{
			return;
		}
		if (pos == _pTail)
		{
			PopBack();
		}
		else if (pos == _pHead)
		{
			PopFront();
		}
		else
		{
			pos->_pPre->_pNext = pos->_pNext;
			pos->_pNext->_pPre = pos->_pPre;
			delete pos;
		}
		--_size;
	}

	bool Empty()const
	{
		return 0==_size;
	}

	size_t size()const
	{
		return _size;
	}

	Node &Front()
	{
		return *_pHead;
	}

	const Node &Front() const
	{
		return *_pHead;
	}

	Node &Back()
	{
		return *_pTail;
	}

	const Node &Back() const
	{
		return *_pTail;
	}

private:
	Node *_pHead;
	Node *_pTail;
	size_t _size;
};