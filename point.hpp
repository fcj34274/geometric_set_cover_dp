#pragma once
#include <iostream>

class point_t {
private:
    double m_x;
    int m_color;
    bool m_is_left;//for interval's point
public:
    point_t(){}
    point_t(double x, int color) {
        m_x = x;
        m_color = color;
    }
    ~point_t(){}
    point_t& operator=(const point_t& obj) {
        m_x = obj.x();
        m_color = obj.color();
        return *this;
    }
    double x() const {return m_x;}
    int color() const { return m_color;}
    void is_left(bool x) { m_is_left = x;}
    bool is_left() const { return m_is_left;}
    bool operator<(const point_t &r) {
        return m_x < r.x();
    }
    bool operator<=(const point_t &r) {
        return m_x < r.x();
    }
    void print(std::ostream& os) {
        os << m_x;
        if(m_color == 1) std::cout << "(red)";
        if(m_color == -1) std::cout << "(blue)";
    }

};

std::ostream& operator<<(std::ostream& os, point_t& obj) {
    obj.print(os);
    return os;
}
