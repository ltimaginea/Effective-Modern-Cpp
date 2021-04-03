# ***条款 31 ：避免默认捕获方式***

一个 `lambda` 表达式具有如下形式：

```cpp
[capture list] (parameter list) -> return type { function body }
```

闭包是 `lambda` 式创建的运行期对象，根据不同的捕获模式，闭包会持有数据的副本或引用，闭包的类型不能被指名，但可用 `auto` 提及。

**显式捕获 `lambda` 表达式所依赖的局部变量或形参是更好的软件工程实践**。

捕获只能针对于在创建 `lambda` 式的作用域内可见的非静态局部变量（包括形参）。

