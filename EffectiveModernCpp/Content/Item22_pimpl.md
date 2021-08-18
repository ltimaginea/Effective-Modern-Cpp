# *条款 22：使用 Pimpl习惯用法时，将特殊成员函数的定义放到实现文件中*

## Pimpl

"Pimpl" 意为 "Pointer to implementation" ，即 “指向实现的指针”。

"Pimpl" 是一种 C++ 编程技巧，它将类的实现细节从对象表示中移除，放到一个分离的类中，并以一个不透明的指针进行访问。此技巧用于构造拥有稳定 ABI 的 C++ 库接口，及减少编译时依赖。



`interface (widget.h)`

```cpp
#pragma once
#include <memory>

class Widget
{
public:
	Widget(int i = 0);
	Widget(const Widget&);
	Widget(Widget&&) noexcept;
	Widget& operator=(const Widget&);
	Widget& operator=(Widget&&) noexcept;
	~Widget();
public:
	void PrintInfo();
	// ...
private:
	// things to be hidden go here
	class Impl;
	// opaque pointer to forward-declared class
	std::unique_ptr<Impl> pimpl_;
};

// Tips:
//	1. 对于 std::unique_ptr 而言，删除器的类型是智能指针类型的一部分
//	2. 对于 std::shared_ptr 而言，删除器的类型并非智能指针类型的一部分
```



`implementation (widget.cpp)`

```cpp
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
```



`user (user.cpp)`

```cpp
#include "pimpl.h"

int main()
{
	Widget w1(1);
	Widget w2(w1);
	Widget w3 = std::move(w1);
	Widget w4(4);
	w1 = w4;
	Widget w5(5);
	Widget w6(6);
	w5 = std::move(w5);
	w6 = w1;

	w1.PrintInfo();
	w2.PrintInfo();
	w3.PrintInfo();
	w4.PrintInfo();
	w5.PrintInfo();
	w6.PrintInfo();

	return 0;
}
```



## 解释

因为类的私有数据成员参与其对象表示，影响大小和布局，也因为类的私有成员函数参与[重载决议](https://zh.cppreference.com/w/cpp/language/overload_resolution)（这发生于成员访问检查之前），故对实现细节的任何更改都要求该类的所有用户重编译。

Pimpl 打破了这种编译依赖；实现的改动不会导致重编译。结果是，若某个库在其 ABI 中使用 Pimpl，则这个库的新版本可以更改实现，并且与旧版本保持 ABI 兼容。所以它通常被称为“编译防火墙”。

在这里我把`#include`命令写出来是为了明确一点，对于`std::string`，`std::vector`和`gadget`的头文件的整体依赖依然存在。 然而，这些依赖从头文件`widget.h`（它被所有`Widget`类的用户(Users)包含，并且对他们可见）移动到了`widget.cpp`（该文件只被`Widget`类的实现者(Implementer)包含，并只对他可见）。

![](https://github.com/ltimaginea/Effective-Modern-Cpp/blob/main/EffectiveModernCpp/Images/Item22_pimpl/gotw.png)

## 实现

由于接口类型的对象控制实现类型对象的生存期，指向实现的指针通常是 [std::unique_ptr](https://zh.cppreference.com/w/cpp/memory/unique_ptr)。

因为 [std::unique_ptr](https://zh.cppreference.com/w/cpp/memory/unique_ptr) 要求被指向类型在任何实例化删除器的语境中均为完整类型，故特殊成员函数必须为用户声明，且在实现类为完整的实现文件中类外定义。

因为当 const 成员通过非 const 成员指针调用函数时会调用实现函数的非 const 重载，故该指针必须包装于 [std::experimental::propagate_const](https://zh.cppreference.com/w/cpp/experimental/propagate_const) 或等价物中。

将所有私有数据成员和所有私有非虚成员函数置于实现类中。将所有公开、受保护和虚成员留于接口类中（对替代方案的讨论见 [GOTW #100](http://herbsutter.com/gotw/_100/)）。

若任何私有成员需要访问公开或受保护成员，则可以将指向接口的引用或指针作为参数传递给私有函数。另外，也可以将回溯引用作为实现类的一部分维持。

## 原因

因为





> ## *References*
>
> [PImpl - cppreference.com](https://en.cppreference.com/w/cpp/language/pimpl)
>
> [GotW #100: Compilation Firewalls (Difficulty: 6/10) – Sutter’s Mill (herbsutter.com)](https://herbsutter.com/gotw/_100/)
>
> [EffectiveModernCppChinese/item22.md at master · kelthuzadx/EffectiveModernCppChinese · GitHub](https://github.com/kelthuzadx/EffectiveModernCppChinese/blob/master/4.SmartPointers/item22.md)
>
> 

