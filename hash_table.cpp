#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

template <typename Tobj>
class Table
{
private:
	struct Tipe
	{
		Tipe() {}
		Tipe(Tobj object, int key)
		{
			this->object = object;
			this->key = key;
			is_empty = false;
		}
		Tobj object = Tobj();
		int key = int();
		bool is_empty = true;

		friend ostream & operator<<(ostream & stream, const Tipe & ob)
		{
			stream << ob.object;
			return stream;
		}
	};
	Tipe **table;
	inline int hashing(const Tipe &object);

public:
	const int SIZE;
	Table(const int & size);
	void add(Tipe &&object);
	void remove(const int &key);
	void write(const string &path = "table.dat");
	void read(const string &path);
	void get();
	void get(int key);
	~Table();

};

int main()
{
	Table<int> table(7);

	table.add({ 12, 1 });
	table.add({ 54, 21 });
	table.add({ 525, 12 });
	table.add({ 52, 45 });
	table.add({ 32, 10 });
	table.add({ 26, 35 });
	table.get();

	
	cout << endl;
	table.get(1);
	table.get(10);

	table.remove(21);

	cout << endl;
	table.get();

	return 0;
}

template<typename Tobj>
Table<Tobj>::Table(const int &size) : SIZE(size)
{
	table = new Tipe*[SIZE];
	for (size_t i = 0; i < SIZE; ++i)
	{
		table[i] = new Tipe[SIZE];
	}
}

template<typename Tobj>
void Table<Tobj>::add(Tipe && object)
{
	int hash = hashing(object);
	for (size_t i = 0; i < SIZE; i++)
	{
		if (table[hash][i].is_empty == true)
		{
			table[hash][i] = object;
			break;
		}
	}
}

template<typename Tobj>
void Table<Tobj>::remove(const int & key)
{
	int hash = key % SIZE;
	for (size_t i = 0; i < SIZE; i++)
	{
		if (table[hash][i].key == key)
		{
			table[hash][i].is_empty = true;
			table[hash][i].object = Tobj();
			table[hash][i].key = int();
			break;
		}
	}
}

template<typename Tobj>
void Table<Tobj>::write(const string & path)
{
	ofstream fout(path, ios::binary);
	if (!fout.is_open())
		throw "error of reading file";
	else
		for (size_t i = 0; i < SIZE; ++i)
			for (size_t j = 0; j < SIZE; ++j)
				fout.write((char *)&table[i][j], sizeof(Tipe));
	fout.close();
}

template<typename Tobj>
void Table<Tobj>::read(const string & path)
{
	ifstream fin(path, ios::binary);
	if (!fin.is_open())
		throw "error of reading file";
	else
	{
		Tipe temp;
		while (fin.read((char *)&temp, sizeof(Tipe))) //SIZE
		{
			add(temp.object, temp.key);
		}
	}
	fin.close();
}

template<typename Tobj>
void Table<Tobj>::get()
{
	for (size_t i = 0; i < SIZE; i++)
	{
		for (size_t j = 0; j < SIZE; j++)
		{
			cout << table[i][j] << " ";
		}
		cout << endl;
	}
}

template<typename Tobj>
void Table<Tobj>::get(int key)
{
	int hash = key % SIZE;
	for (size_t i = 0; i < SIZE; i++)
	{
		if (table[hash][i].key == key)
		{
			cout << table[hash][i] << endl;
			break;
		}
	}
}

template<typename Tobj>
int Table<Tobj>::hashing(const Tipe & object)
{
	return object.key % SIZE;
}

template<typename Tobj>
Table<Tobj>::~Table()
{
	write();
	for (size_t i = 0; i < SIZE; ++i)
	{
		delete[] table[i];
	}
}