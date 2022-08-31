#include<iostream>
#include<vector>
#include<string>
using std::cout;
using std::endl;
using std::vector;
using std::string;

#include"linq_to_objects/linq_to_objects.hpp"

struct Person
{
	int age;
	string name;
	string address;
};

template<typename T>
class A
{
public:
	A(T value):val(value){}
	
	void test()
	{
		cout<<val<<endl;	
	}
private:
	T val;
};

void TestLinqCpp()
{
	vector<Person>v={{21,"wjh","shanghai"},{22,"bb","wuhan"},{20,"bqf","zhuhai"}};
	auto count=from(v).where([](const Person& p){return p.age>20;}).count();
	cout<<count<<endl;
}

int main()
{
	TestLinqCpp();
    return 0;
}
