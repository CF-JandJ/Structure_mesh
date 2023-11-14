#pragma once

//! @brief 
//! @tparam real         t������ 
//! @tparam RhsFunction  h������
//! @param t             ��ǰ����� tn
//! @param h             ����
//! @param yn            ��ǰ���Ľ� y(t)
//! @param f             ΢�ַ����Ҷ���
//! @return              ��һ���Ľ� y(t+h)
template<typename real, typename RhsFunction>
real runge_kutta_4th_rev3(real t, real h, real yn, RhsFunction f)
{
    auto k1 = h * f(t, yn);
    auto k2 = h * f(t + h / 2, yn + k1 / 2);
    auto k3 = h * f(t + h / 2, yn + k2 / 2);
    auto k4 = h * f(t + h, yn + k3);
    return yn + real{ 1 } / real{ 6 } *(k1 + 2 * k2 + 2 * k3 + k4);
}

// ���⣺
//  1) �м���� k1,k2,k3,k4 �����뷽�̸����йأ�����ö�̬������ڼ���Ч������
//  2) ������ù̶��������飬�������������

//! @brief ���һ��Runge-Kutta����
//! @param t     ��ǰ����� tn
//! @param h     ����
//! @param ndof  ���̸���
//! @param yn    ��ǰ���Ľ� y(t)�����鳤��Ϊndof
//! @param f     �Ҷ����
//! @param yn1   ������������飬���鳤��Ϊndof
//! @param k1    ��ʱ�洢�ռ䣬���鳤��Ϊndof
//! @param k2    ��ʱ�洢�ռ䣬���鳤��Ϊndof
//! @param k3    ��ʱ�洢�ռ䣬���鳤��Ϊndof
//! @param k4    ��ʱ�洢�ռ䣬���鳤��Ϊndof
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