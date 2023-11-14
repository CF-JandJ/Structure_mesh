#pragma once

//! @brief 
//! @tparam real         t的类型 
//! @tparam RhsFunction  h的类型
//! @param t             当前因变量 tn
//! @param h             步长
//! @param yn            当前步的解 y(t)
//! @param f             微分方程右端项
//! @return              下一步的解 y(t+h)
template<typename real, typename RhsFunction>
real runge_kutta_4th_rev3(real t, real h, real yn, RhsFunction f)
{
    auto k1 = h * f(t, yn);
    auto k2 = h * f(t + h / 2, yn + k1 / 2);
    auto k3 = h * f(t + h / 2, yn + k2 / 2);
    auto k4 = h * f(t + h, yn + k3);
    return yn + real{ 1 } / real{ 6 } *(k1 + 2 * k2 + 2 * k3 + k4);
}

// 问题：
//  1) 中间变量 k1,k2,k3,k4 个数与方程个数有关，如果用动态数组存在计算效率问题
//  2) 如果采用固定长度数组，求解问题受限制

//! @brief 完成一步Runge-Kutta迭代
//! @param t     当前因变量 tn
//! @param h     步长
//! @param ndof  方程个数
//! @param yn    当前步的解 y(t)，数组长度为ndof
//! @param f     右端项函数
//! @param yn1   输出计算结果数组，数组长度为ndof
//! @param k1    临时存储空间，数组长度为ndof
//! @param k2    临时存储空间，数组长度为ndof
//! @param k3    临时存储空间，数组长度为ndof
//! @param k4    临时存储空间，数组长度为ndof
template<typename Vector, typename RhsFunction>
void runge_kutta_4th_rev6(
    double t, double h,
    int ndof,
    const Vector& yn,
    RhsFunction f,
    Vector& yn1,
    Vector& k1,
    Vector& k2,
    Vector& k3,
    Vector& k4
)
{
    // k1 = h * f(t, yn)
    //    f(t,yn) --> k1
    //    k1 = k1 * h
    f(t, ndof, yn1, k1);
    for (int i = 0; i < ndof; ++i)k1[i] *= h;

    // k2 = h * f(t + h / 2, yn + k1 / 2)
    //    yn+k1/2       --> yn1
    //    f(t+h/2, yn1) --> k2
    //    k2 * h        --> k2
    for (int i = 0; i < ndof; ++i)yn1[i] = yn[i] + k1[i] / 2;
    f(t + h / 2, ndof, yn1, k2);
    for (int i = 0; i < ndof; ++i)k2[i] *= h;

    // k3 = h * f(t + h / 2, yn + k2 / 2)
    //    yn + k2 / 2  --> yn1
    //    f(t+h/2,yn1) --> k3
    //    k3 * h       --> k3
    for (int i = 0; i < ndof; ++i)yn1[i] = yn[i] + k2[i] / 2;
    f(t + h / 2, ndof, yn1, k3);
    for (int i = 0; i < ndof; ++i)k3[i] *= h;

    // k4 = h * f(t + h, yn + k3)
    //    yn + k3    --> yn1
    //    f(t+h,yn1) --> k4
    //    k4 * h     --> k4
    for (int i = 0; i < ndof; ++i)yn1[i] = yn[i] + k3[i];
    f(t + h, ndof, yn1, k4);
    for (int i = 0; i < ndof; ++i)k4[i] *= h;

    // yn + 1.0 / 6.0 * (k1 + 2 * k2 + 2 * k3 + k4)
    for (int i = 0; i < ndof; ++i)
        yn1[i] = yn[i] + 1.0 / 6.0 * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);
}