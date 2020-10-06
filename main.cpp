#include <iostream>
#include <vector>
#include <stack>

class point_t {
private:
    double m_x;
    int m_color;
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

class interval_t {
private:
    point_t m_left;
    point_t m_right;
    std::vector<int> m_covering_red;//vector for index
    std::vector<int> m_covering_blue;//vector for index
public:
    interval_t(){}
    interval_t(point_t left, point_t right) {
        m_left = left;
        m_right = right;
    }
    ~interval_t(){}
    point_t left() const { return m_left;}
    point_t right() const { return m_right;}
    int cover_red() const { return m_covering_red.size();}
    int cover_blue() const { return m_covering_blue.size();}
    int gain() const { return cover_red()-cover_blue();}
    void print(std::ostream& os) {
        os << m_left << " to " << m_right << std::endl << "\t\t";
        for(int i=0; i<m_covering_red.size(); i++) {
            os << m_covering_red[i] << " ";
        }
        for(int i=0; i<m_covering_blue.size(); i++) {
            os << m_covering_blue[i] << " ";
        }
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

template<typename T>
void swap(T a, T b) {
    T tmp;
    tmp = a;
    a = b;
    b = tmp;
}

bool is_in(point_t p, interval_t I) {
    if(I.left() <= p && p <= I.right() ) return true;
    return false;
}
int gain_interval(point_t left, point_t right, std::vector<point_t> red, std::vector<point_t> blue) {
  int count_red = 0;
  int count_blue = 0;
  for(int i=0; i<red.size(); i++) {
    if(left <= red[i] && red[i] <= right) {
      count_red += 1;
    }
  }
  for(int i=0; i<blue.size(); i++) {
    if(left <= blue[i] && blue[i] <= right) {
      count_blue += 1;
    }
  }

  return count_red - count_blue;
}

int main(void) {




    //INPUT
    int size_point,size_interval;
    std::vector<point_t> point;
    std::vector<point_t> red;
    std::vector<point_t> blue;

    std::cout << "INPUT points" << std::endl;
    std::cin >> size_point;
    for(int i=0; i<size_point; i++) {
        double tmp_x;
        int tmp_color;
        std::cin >> tmp_x >> tmp_color;
        point_t tmp_point(tmp_x,tmp_color);
        point.push_back(tmp_point);
        if(tmp_color == 1) red.push_back(tmp_point);
        else if(tmp_color == -1) blue.push_back(tmp_point);
    }
    std::cout << "INPUT intervals" << std::endl;
    std::cin >> size_interval;
    std::vector<interval_t> interval;
    for(int j=0; j<size_interval; j++) {
        double a,b;
        std::cin >> a >> b;
        if(a<b) {
            //do nothing
        } else {
            double tmp = a;
            a = b;
            b = tmp;

        }
        point_t tmp_left(a,0), tmp_right(b,0);
        interval_t tmp_interval(tmp_left,tmp_right);
        interval.push_back(tmp_interval);
    }

    //PREPROCEDURE && PRECOMPUTE
    for(int j=0; j<interval.size(); j++) {
        for(int i=0; i<point.size(); i++) {
            if(is_in(point[i],interval[j])) {
                interval[j].add(i,point[i].color());
            }
        }
    }

    //COMPUTE
    int K = 10;
    int f[size_interval][K+1];
    int LA[size_interval][K+1];//for last added interval's index
    for(int i=0; i<size_interval; i++) {
      for(int j=0; j<K+1; j++) {
        f[i][j] = 0;
        LA[i][j] = -1;
        //-1 : NULL
      }
    }
    //initialization for array f and LA
    for(int j=0; j<K+1; j++) {
        int score = interval[0].cover_red() - interval[0].cover_blue();
        if( score < j && score > 0) {
            f[0][j] = score;
            LA[0][j] = 0;
        } else {
            f[0][j] = 0;
        }
    }

    for(int i=1; i<size_interval; i++) {
        for(int j=0; j<K+1; j++) {
          std::cout << "hooray!\t"<< "(" << i << "," << j << ")" << std::endl;
          int index_LA = LA[i-1][j];
          int gain_interval_i = interval[i].gain();
            //TODO
            if(index_LA == -1) {
              int score = interval[i].cover_red() - interval[i].cover_blue();
              if( score <= j && score > f[i-1][j]) {
                  f[i][j] = score;
                  LA[i][j] = i;
              }
              continue;
            }

            if(!is_in(interval[i].left(), interval[index_LA])) {
              int nadd = f[i-1][j];
              f[i][j] = nadd;
              LA[i][j] = LA[i-1][j];

              if( gain_interval_i <= j ) {
                int add = f[i-1][j-gain_interval_i]+gain_interval_i;
                if(add > nadd) {
                  f[i][j] = add;
                  LA[i][j] = i;
                }
              }
            } else {
              int gain_interval_LA = interval[index_LA].gain();
              //TODO
              int gain_union_LA_i = gain_interval(interval[index_LA].left(),interval[i].right(), red, blue);
              int true_gain_i = gain_union_LA_i - gain_interval_LA;//this means difference interval i\LA
              int nadd = f[i-1][j];
              f[i][j] = f[i-1][j];
              LA[i][j] = LA[i-1][j];
              if( true_gain_i <= j ) {
                int add = f[i-1][j-true_gain_i] + true_gain_i;
                if(add > nadd) {
                  f[i][j] = add;
                  LA[i][j] = i;
                }
              }
            }
        }
    }


    //OUTPUT
    std::cout << "<<all points>>" << std::endl;
    for(int i=0; i<size_point; i++) {
        std::cout << point[i] << " ";
    }std::cout << std::endl << std::endl;

    std::cout << "<<all intervals>>" << std::endl;
    for(int i=0; i<size_interval; i++) {
        std::cout << "\t<<interval " << i << ">> : ";
        std::cout << interval[i] << std::endl;
    }std::cout << std::endl << std::endl;

    std::cout << "K\t";
    for(int j=0; j<K+1; j++) std::cout << j << "\t";
    std::cout << std::endl;
    std::cout << "i" << std::endl;
    for(int i=0; i<size_interval; i++) {
      std::cout << i << "\t";
      for(int j=0; j<K+1; j++) {
        std::cout << f[i][j] << "\t";
      }
      std::cout << std::endl;
    }



    return 0;
}
