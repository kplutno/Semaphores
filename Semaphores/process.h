#include <vector>
#include <iostream>

using namespace std;

class Buffer
{
	vector<int> bufor;
public:
	int get();
	void put(int a);
	int peek();
	int count();
	int countEN();
	int countON();

	friend ostream& operator << (ostream& os, const Buffer& buf);
};

int Buffer::get()
{
	int buf = bufor[0];
	bufor.erase(bufor.begin());
	return buf;
}

void Buffer::put(int a)
{
	bufor.push_back(a);
}

int Buffer::peek()
{
	return bufor[0];
}

int Buffer::countEN()
{
	int cnt = 0;
	for (int x : bufor)
	{
		if (x % 2 == 0)
			++cnt;
	}
	return cnt;
}

int Buffer::countON()
{
	int cnt = 0;
	for (int x : bufor)
	{
		if (x % 2 == 1)
			++cnt;
	}
	return cnt;
}

int Buffer::count()
{
	return bufor.size();
}

ostream& operator << (ostream& os, const Buffer& buf)
{
	for (int x : buf.bufor)
	{
		os << x << ' ';
	}
	os << endl;
	return os;
}