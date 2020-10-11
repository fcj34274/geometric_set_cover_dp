#include <iostream>
#include <vector>
#include <climits>
#include <tuple>
#include <algorithm>
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

int gain_interval(double left, double right, std::vector<point_t> red, std::vector<point_t> blue) {
    int count_red = 0;
    int count_blue = 0;
    for(int i=0; i<red.size(); i++) {
      if(left <= red[i].x() && red[i].x() <= right) {
        count_red += 1;
      }
    }
    for(int i=0; i<blue.size(); i++) {
      if(left <= blue[i].x() && blue[i].x() <= right) {
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
        interval_t tmp_interval(j,tmp_left,tmp_right);
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
    std::vector<int> set_cover[size_interval][K+1];
    for(int i=0; i<size_interval; i++) {
      for(int j=0; j<K+1; j++) {
        f[i][j] = INT_MAX;
      }
    }
    //initialization for array f
    for(int j=0; j<K+1; j++) {



        if(j == 0) {
            f[0][j] = 0;
            continue;
        }
        int gain_interval_0 = interval[0].gain();
        if( gain_interval_0 == j && gain_interval_0 > 0) {

            f[0][j] = gain_interval_0;
            set_cover[0][j].push_back(0);
        }
    }

    for(int i=1; i<size_interval; i++) {
        int gain_interval_i = interval[i].gain();
        for(int j=0; j<K+1; j++) {


            if(j == 0) { f[i][j] = 0; continue; }




            //-----------------------------------------------
            //f(i,j)を更新する
            // *f(i-1,j),
            // *f(i-1,j-k)+1 (k=1 to gain_interval_i)
            //-----------------------------------------------
            f[i][j] = f[i-1][j];
            set_cover[i][j].clear();
            set_cover[i][j].resize(set_cover[i-1][j].size());
            std::copy(set_cover[i-1][j].begin(), set_cover[i-1][j].end(), set_cover[i][j].begin());

            for(int k=1; k<=gain_interval_i; k++) {
                //-----------------------------------------------
                //j-k<0 なら配列からはみ出すので break
                //-----------------------------------------------
                if(j-k < 0) break;

                //-----------------------------------------------
                //各 k (k=1 to gain_interval_i) に対して セットカバーとそのサイズを計算
                //-----------------------------------------------
                int score_k;
                std::vector<int> tmp_set_cover;
                tmp_set_cover.clear();
                tmp_set_cover.resize(set_cover[i-1][j-k].size());
                std::copy(set_cover[i-1][j-k].begin(), set_cover[i-1][j-k].end(), tmp_set_cover.begin());
                tmp_set_cover.push_back(i);

                //カバーセットの区間集合と区間iのすべての端点をx_RL_noに保存
                std::vector<std::tuple<double, int, int>> x_RL_no;
                for(int l=0; l<tmp_set_cover.size(); l++) {
                    int idx = tmp_set_cover[l];
                    double tmp_left = interval[idx].left().x();
                    double tmp_right = interval[idx].right().x();
                    int no = idx;
                    x_RL_no.push_back(std::make_tuple(tmp_left,0,no));
                    x_RL_no.push_back(std::make_tuple(tmp_right,1,no));
                    //0: left
                    //1: right
                }


                std::sort(x_RL_no.begin(), x_RL_no.end());
                bool is_open = false;
                int step = 0;
                double tmp_hole_left;
                double tmp_hole_right;
                int sum_hole_gain = 0;
                for(int l=0; l<x_RL_no.size(); l++) {
                    double tmp_x;
                    int tmp_RL;
                    int tmp_no;
                    std::tie(tmp_x,tmp_RL,tmp_no) = x_RL_no[l];
                    if(tmp_no!=i) {
                        if(tmp_RL == 0) {
                            step += 1;
                            if(step == 1 && is_open) {
                                tmp_hole_right = tmp_x;
                                if(tmp_hole_left != tmp_hole_right) {
                                    sum_hole_gain += gain_interval(tmp_hole_left, tmp_hole_right, red, blue);
                                }
                            }
                        } else {
                            step -= 1;
                            if(step == 0 && is_open) {
                                tmp_hole_left = tmp_hole_right = tmp_x;
                            }
                        }
                    } else {
                        if(tmp_RL == 0) {
                            is_open = true;
                            if(step == 0) {
                                tmp_hole_left = tmp_hole_right = tmp_x;
                            }

                        } else {
                            is_open = false;
                            if(step == 0) {
                                tmp_hole_right = tmp_x;
                                if(tmp_hole_left != tmp_hole_right) {
                                    sum_hole_gain += gain_interval(tmp_hole_left, tmp_hole_right, red, blue);
                                }
                            }
                        }
                    }
                }

                score_k = sum_hole_gain + j-k;
                if(score_k == j && f[i-1][j-k]+1 < f[i][j] && f[i-1][j-k] != INT_MAX) {
                    f[i][j] = f[i-1][j-k] + 1;
                    set_cover[i][j].clear();
                    set_cover[i][j].resize(tmp_set_cover.size());
                    std::copy(tmp_set_cover.begin(), tmp_set_cover.end(), set_cover[i][j].begin());
                }
            }
        }
    }
    int idx_max = 0;
    for(int j=0; j<K+1; j++) {
        if(f[size_interval-1][j] != INT_MAX && f[size_interval-1][j] >= f[size_interval-1][idx_max]) {
            idx_max = j;
        }
    }


    //OUTPUT
    // std::cout << "<<ALL POINTS>>" << std::endl;
    // for(int i=0; i<size_point; i++) {
    //     std::cout << "\t<<point " << i << ">> : ";
    //     std::cout << point[i] << " " << std::endl;
    // }std::cout << std::endl << std::endl;

    // std::cout << "<<ALL INTERVALS>>" << std::endl;
    // for(int i=0; i<size_interval; i++) {
    //     std::cout << "\t<<interval " << i << ">> : ";
    //     std::cout << interval[i] << std::endl;
    // }std::cout << std::endl << std::endl;


    std::cout << "<<DP MATRIX>>" << std::endl;
    std::cout << "K\t";
    for(int j=0; j<K+1; j++) std::cout << j << "\t";
    std::cout << std::endl;
    std::cout << "i" << std::endl;
    for(int i=0; i<size_interval; i++) {
      std::cout << i << "\t";
      for(int j=0; j<K+1; j++) {
          if(f[i][j] == INT_MAX) {
              std::cout << "INF";
          } else {
              std::cout << f[i][j];
          }std::cout << "\t";
      }
      std::cout << std::endl;
    }

    std::cout << "<<ANSER>>" << std::endl;
    int tmp_i = size_interval-1, tmp_j = idx_max;
    std::cout << "i,j = " << tmp_i << "," << tmp_j <<std::endl;
    for(int i=0; i<set_cover[tmp_i][tmp_j].size(); i++) {
        std::cout << set_cover[tmp_i][tmp_j][i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
