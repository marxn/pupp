Pupp
====

### pupp-lang featuers:

* Support Closures and Lambda expressions natively. 

* Introduced transactions statement. That means you can use commit/rollback to control all the changes of variables in any code block atomically.

* Introduced a universal set whose keys and values can be of different types.

* Introduced a decimal type of unlimited precsion.

* Introduced a total new variable type - 'Message'. It is the essential function of PUPP.


```C++
a := [[1,2,3],4,5,6,(){}];
def b : int[10,10];
a[0][0] = 9;
b[1,1]=1;
print a,NL;
print b,NL;
```
