#include <iostream>
#include <vector>
#include <algorithm>

typedef struct {
    double left;
    double right;
    std::vector<double> cover_red;
    std::vector<double> cover_blue;
}interval_t;

void swap(double &a, double &b) {
    double tmp;
    tmp = a;
    a = b;
    b = tmp;
}

bool is_in(double point, interval_t interval) {
    //return true if and only if the point is in intereval
    if(interval.left <= point && point <= interval.right) return true;
    else return false;
}

void print_vector(std::vector<double> vec, double in_color[]) {
    //red : 1
    //blue : -1
    for(int i=0; i<vec.size(); i++) {
        int index = vec[i];
        std::cout << index << " : " << in_color[index] << " ";
    }
    std::cout << std::endl;
}


int main(void) {
    //input
    int size_in_red, size_in_blue, size_in_interval;
    std::cout << "input size of red point set \n red>>";
    std::cin >> size_in_red;
    double in_red[size_in_red+1];
    for(int i=1; i<=size_in_red; i++) {
        std::cout << "p" << i << ">>";
        std::cin >> in_red[i];
    }


    std::cout << "input size of blue point set \n blue>>";
    std::cin >> size_in_blue;
    double in_blue[size_in_blue+1];
    for(int i=1; i<=size_in_blue; i++) {
        std::cout << "p" << i << ">>";
        std::cin >> in_blue[i];
    }

    std::cout << "input size of interval set \n interval>>";
    std::cin >> size_in_interval;
    interval_t in_interval[size_in_interval+1];
    for(int i=1; i<=size_in_interval; i++) {
        double a,b;
        std::cout << "in_interval" << i << "" << std::endl;
        std::cout << "one>>";
        std::cin >> a;
        std::cout << "the other>>";
        std::cin >> b;
        in_interval[i].left = a;
        in_interval[i].right = b;
        if(a<b) {
            //do nothing
        } else {
            swap(in_interval[i].left, in_interval[i].right);
        }
    }
    std::cout << std::endl;

    //precompute & preprocedure
    //give each interval covering point both red and blue
    for(int i=1; i<=size_in_red; i++) {
        for(int j=1; j<=size_in_interval; j++) {
            //push_back each index of point in the interval
            if(is_in(in_red[i],in_interval[j])) in_interval[j].cover_red.push_back(i);
            if(is_in(in_blue[i],in_interval[j])) in_interval[j].cover_blue.push_back(i);
        }
    }

    //CONFIRMATION OF PREPROCEDURE
    // for(int j=1; j<=size_in_interval; j++) {
    //     std::cout << " << interval" << j << " >> : " << in_interval[j].left << " to " << in_interval[j].right << std::endl;
    //     std::cout << "<< RED points >>" << std::endl;
    //     print_vector(in_interval[j].cover_red, in_red);
    //     std::cout << "<< BLUE points >>" << std::endl;
    //     print_vector(in_interval[j].cover_blue, in_blue);
    //     std::cout << std::endl;
    //
    // }

    compute
    std::vector<point> dp[size_in_interval+1][size_in_red+1];

    for(int i=1; i<=size_in_red; i++) {
        for(int j=0; j<=size_in_interval; j++){
            if(j == 0) {
                dp[i][j] = 0;
                continue;
            }
            if((r-b)[i][j] > j) {
                dp[i][j] = dp[i-1][j];
            } else {
                dp[i][j] = max(dp[i-1][j], dp[i-1][ j - (r-b)[i][j] ]);
            }
        }
    }


    //output

    // std::cout << "<< size of point set >> : " << size_in_red << std::endl;
    // for(int i=1; i<=size_in_red; i++) {
    //     std::cout << "red" << i << " : " << in_red[i] << std::endl;
    // }
    // std::cout << std::endl;
    //
    // std::cout << "<< size of blue point set >> : " << size_in_blue << std::endl;
    // for(int i=1; i<=size_in_blue; i++) {
    //     std::cout << "blue" << i << " : " << in_blue[i] << std::endl;
    // }
    // std::cout << std::endl;
    //
    //
    // std::cout << "<< size of interval set >> : " << size_in_interval << std::endl;
    // for(int i=1; i<=size_in_interval; i++) {
    //     std::cout << "interval" << i << " : " << in_interval[i].left << " to " << in_interval[i].right << std::endl;
    // }
    // std::cout << std::endl;

    return 0;
}
