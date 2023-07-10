#include <eigen3/Eigen/Core>
#include <iostream>

int main()
{
    Eigen::Vector3f p(2.0f, 1.0f, 1.0f);
    Eigen::Matrix3f m;
    m << cos(M_PI / 4), -sin(M_PI / 4), 1,
        sin(M_PI / 4), cos(M_PI / 4), 2,
        0, 0, 1;
    auto res = m * p;
    std::cout << res << std::endl;
}