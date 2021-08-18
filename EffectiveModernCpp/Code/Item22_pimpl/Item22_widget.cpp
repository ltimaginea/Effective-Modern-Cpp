#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include "pimpl.h"
//#include "gadget.h"

class Widget::Impl
{
public:
	Impl(int i = 0) :i_(i) {  }
public:
	int GetInfo() { return i_; }
private:
	std::string name_;
	std::vector<double> data_;
	int i_;
	// ...
};

Widget::Widget(int i) :pimpl_(std::make_unique<Impl>(i))
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

void Widget::PrintInfo()
{
	std::cout << pimpl_->GetInfo() << std::endl;
}

// Tips:
//	1. 对于 std::unique_ptr 而言，删除器的类型是智能指针类型的一部分
//	2. 对于 std::shared_ptr 而言，删除器的类型并非智能指针类型的一部分
