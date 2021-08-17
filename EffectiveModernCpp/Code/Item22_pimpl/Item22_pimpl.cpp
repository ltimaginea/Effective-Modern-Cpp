#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>

#include "pimpl.h"

class Widget::Impl
{
private:
	std::string name;
	std::vector<double> data;
	int i;
public:
	// ...
};

Widget::Widget() :pimpl_(std::make_unique<Impl>())
{

}

Widget::Widget(const Widget& rhs) : pimpl_(std::make_unique<Impl>(*rhs.pimpl_))
{

}

Widget::Widget(Widget&& rhs) noexcept :pimpl_(std::move(rhs.pimpl_))
{

}

Widget& Widget::operator=(const Widget& rhs)
{
	pimpl_ = std::make_unique<Impl>(*rhs.pimpl_);
	return *this;
}

Widget& Widget::operator=(Widget&& rhs) noexcept
{
	pimpl_ = std::move(rhs.pimpl_);
	return *this;
}

Widget::~Widget() = default;

int main()
{
	Widget w;
	return 0;
}

//	Tips:
//	1. 对于 std::unique_ptr 而言，删除器的类型是智能指针类型的一部分
//	2. 对于 std::shared_ptr 而言，删除器的类型并非智能指针类型的一部分
