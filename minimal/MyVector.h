#pragma once
#include <stdexcept>

namespace MyVector {

class Vector3 {
public:
  // 构造函数
  Vector3() : _x(0.0f), _y(0.0f), _z(0.0f) {}
  Vector3(float x, float y, float z) : _x(x), _y(y), _z(z) {}
  // 数据成员访问器
  float x() const { return _x; }
  float y() const { return _y; }
  float z() const { return _z; }
  float &x() { return _x; }
  float &y() { return _y; }
  float &z() { return _z; }
  // 算数运算符重载, 利用 += 和取负实现所有加减运算符
  // +=
  Vector3 &operator+=(const Vector3 &v) {
    _x += v._x;
    _y += v._y;
    _z += v._z;
    return *this;
  }
  // 一元负号运算符
  Vector3 operator-() const {
    return Vector3(-_x, -_y, -_z);
  }

  // 加法, 减法, -=
  Vector3 operator+(const Vector3& v) const{
    return Vector3{*this} += v;
  }
  Vector3 operator-(const Vector3& v) const {
    return Vector3{*this} += -v;
  }
  Vector3 &operator-=(const Vector3 &v) {
    *this += -v;
    return *this;
  }
  
  // 浮点数乘、数除法
  friend Vector3 operator*(const Vector3 &v, float scalar) {
    return Vector3(v._x * scalar, v._y * scalar, v._z * scalar);
  }
  friend Vector3 operator*(float scalar, const Vector3 &v) {
    return v * scalar;
  }
  friend Vector3 operator/(const Vector3 &v, float scalar) {
    if (scalar == 0.0f) {
      throw std::runtime_error("Division by zero");
    }
    return Vector3(v._x / scalar, v._y / scalar, v._z / scalar);
  }
  
private:
  float _x, _y, _z;
};

} // namespace MyVector