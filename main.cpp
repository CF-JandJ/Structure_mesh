// example1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "MDArray.h"
#include "read_mesh.h"

int main()
{
    MDArray<double, 2> x, y;
    
    readGrid("naca0012.x", x, y);

    MDArray<double, 3> x2(3, 5, 4);

    return 0;
}

    