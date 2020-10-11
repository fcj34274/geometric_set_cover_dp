#pragma once
#include <iostream>
#include <vector>
#include "point.hpp"

class interval_t {
private:
    point_t m_left;
    point_t m_right;
    int m_no;
    std::vector<int> m_covering_red;//vector for index
    std::vector<int> m_covering_blue;//vector for index
public:
    interval_t(){}
    interval_t(int no, point_t left, point_t right): m_no(no), m_left(left), m_right(right)  {
    }
    ~interval_t(){}
    point_t left() const { return m_left;}
    point_t right() const { return m_right;}
    int no() const { return m_no;}
    int cover_red() const { return m_covering_red.size();}
    int cover_blue() const { return m_covering_blue.size();}
    int gain() const { return cover_red()-cover_blue();}
    void print(std::ostream& os) {
        os << m_left << " to " << m_right << std::endl << "\t\t";
        std::cout << "RED: ";
        for(int i=0; i<m_covering_red.size(); i++) {
            os << m_covering_red[i] << " ";
        }std::cout << std::endl << "\t\t";
        std::cout << "BLUE: ";
        for(int i=0; i<m_covering_blue.size(); i++) {
            os << m_covering_blue[i] << " ";
        }std::cout << std::endl;
    }
    void add(int index, int color) {
        if(color == 1) {
            m_covering_red.push_back(index);
        } else if(color == -1) {
            m_covering_blue.push_back(index);
        }
    }
};
std::ostream& operator<<(std::ostream& os, interval_t& obj) {
    obj.print(os);
    return os;
}
