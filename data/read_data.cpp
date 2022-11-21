#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

int main(void){
    std::ifstream fin("/home/perple/lianxu/nerf_demo/data/views.txt", std::ios::in | std::ios::binary);
    std::string temp;
    while(getline(fin, temp, '\n')){
        std::vector<double> waypoint;
        std::istringstream sin(temp);
        for(int i=0; i<=3; i++){
            double data;
            sin >> data;
            waypoint.push_back(data);
        }
        std::cout << waypoint[0]  << " "<< waypoint[1] << " " << waypoint[2] << " "<< waypoint[3] << std::endl;
    }
    return 0;
}