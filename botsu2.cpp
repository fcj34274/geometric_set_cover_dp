#include <iostream>
#include <vector>
#include "point.hpp"
#include "interval.hpp"
#include "io.hpp"

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

    std::cout << "INPUT points>>" << std::endl;
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
    std::cout << "INPUT intervals>>" << std::endl;
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
    std::cout << "INPUT K>>";
    std::cin >> K;
    int f[size_interval][K+1];
    int LA[size_interval][K+1];//for last added interval's index
    int from[size_interval][K+1];
    for(int i=0; i<size_interval; i++) {
      for(int j=0; j<K+1; j++) {
        f[i][j] = 0;
        LA[i][j] = -1;
        //-1 : NULL
      }
    }
    //initialization for array f and LA
    for(int j=0; j<K+1; j++) {
        int score = interval[0].gain();
        if( score <= j && score > 0) {
            f[0][j] = score;
            LA[0][j] = 0;
        }
    }

    for(int i=1; i<size_interval; i++) {
        for(int j=0; j<K+1; j++) {
          int index_LA = LA[i-1][j];
          int gain_interval_i = interval[i].gain();

          if(index_LA == -1) {
            //
            //まだ区間を取っていなかったら
            //i番目の区間を取るかどうかを検討してからcontinue
            //
              int score = interval[i].gain();
              if( score <= j && score > 0) {
                  f[i][j] = score;
                  LA[i][j] = i;
              }
              continue;
            }

            if(!is_in(interval[i].left(), interval[index_LA])) {
                //
                //interval[i]とinterval[LA]が交差しないとき
                //interval[i].gain() > 0なら取る、さもなければ取らない
                //
              int nadd = f[i-1][j];
              f[i][j] = nadd;
              LA[i][j] = LA[i-1][j];

              if( gain_interval_i <= j && gain_interval_i > 0 ) {
                int add = f[i-1][j-gain_interval_i]+gain_interval_i;
                if(add > nadd) {
                  f[i][j] = add;
                  LA[i][j] = i;
                }
              }
            } else {
                //
                //interval[i]とinterval[LA]が交差するとき
                //interval[LA]-interval[i] > 0 なら取る、さもなければ取らない
                //
              int gain_interval_LA = interval[index_LA].gain();
              //TODO
              int gain_union_LA_i = gain_interval(interval[index_LA].left(),interval[i].right(), red, blue);
              // std::cout << "(i,j) = (" << i << "," << j << ")" << std::endl;
              // std::cout << "gain_union of interval " << index_LA <<" and interval " << i << " is " << gain_union_LA_i << std::endl;
              int true_gain_i = gain_union_LA_i - gain_interval_LA;//this means difference interval i\LA
              int nadd = f[i-1][j];
              f[i][j] = f[i-1][j];
              LA[i][j] = LA[i-1][j];
              if( true_gain_i <= j && true_gain_i > 0 ) {
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
    std::cout << "<<ALL POINTS>>" << std::endl;
    for(int i=0; i<size_point; i++) {
        std::cout << "\t<<point " << i << ">> : ";
        std::cout << point[i] << " " << std::endl;
    }std::cout << std::endl << std::endl;

    std::cout << "<<ALL INTERVALS>>" << std::endl;
    for(int i=0; i<size_interval; i++) {
        std::cout << "\t<<interval " << i << ">> : ";
        std::cout << interval[i] << std::endl;
    }std::cout << std::endl << std::endl;


    std::cout << "<<DP MATRIX>>" << std::endl;
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
