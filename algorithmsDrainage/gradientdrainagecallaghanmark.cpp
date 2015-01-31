#include "gradientdrainagecallaghanmark.h"
#include <iostream>
#include <algorithm>
#include "primitives/point.h"
struct customLess{
    inline bool operator()(runnel::Point* p1, runnel::Point* p2){
        return p1->coord.z > p2->coord.z;
    }
};
GradientDrainageCallaghanMark::GradientDrainageCallaghanMark(){
    w = 0;
    h = 0;
}

void GradientDrainageCallaghanMark::sortElement(std::vector<runnel::Point *> points){
    points_terrain =  points;
    std::sort( points_terrain.begin(), points_terrain.end(), customLess());
}


void GradientDrainageCallaghanMark::runAlgorithm(Terrain* ter, float moreWater){
    w = ter->width;
    h = ter->height;
    GradientDrainageCallaghanMark::sortElement(ter->struct_point);

    for(runnel::Point* pto : points_terrain){
        this->chooseMoreDepthPoint(ter->struct_point, pto, moreWater);
    }

}
//1  2  3  4  5  6  7  8  9  10
//11 12 13 14 15 16 17 18 19 20
//21 22 23 24 25 26 27 28 29 30

void GradientDrainageCallaghanMark::chooseMoreDepthPoint(std::vector<runnel::Point *>& points, runnel::Point *pto, float delta_water){
    std::vector<int> position_neightbour;
    position_neightbour.push_back(pto->ident - this->w);
    position_neightbour.push_back(pto->ident + this->w);
    if(pto->ident%w > 0){
        position_neightbour.push_back(pto->ident + this->w - 1);
        position_neightbour.push_back(pto->ident - this->w - 1);
        position_neightbour.push_back(pto->ident - 1);
    }
    if(pto->ident%w < w-1){
        position_neightbour.push_back(pto->ident + 1);
        position_neightbour.push_back(pto->ident - this->w + 1);
        position_neightbour.push_back(pto->ident + this->w + 1);
    }
    //   std::cout << "Soy el pto " << pto->ident << std::endl;
	float max_z = -10000000;
    int id_max = -1;
    for (int id : position_neightbour){
       // std::cout << "id en los vecinos " << id << " maximo " << points.size() << std::endl;

        if(id >= 0 && id < points.size()){
          //  std::cout << "valores de z " << points[id]->coord.z  << " " << pto->coord.z << std::endl;
			if( points[id]->coord.z <= (pto->coord.z +delta_water)){
				float dist_z = pto->coord.z - points[id]->coord.z;
             //   std::cout << "dist z  " << dist_z << std::endl;
				if( max_z < dist_z){
                    max_z = dist_z;
                    id_max = id;
                }
			}
        }
    }
	if(!(id_max == -1)){
        points[id_max]->water_parent.push_back(points[pto->ident]);
        points[id_max]->water_value = points[id_max]->water_value + points[pto->ident]->water_value ;
		if(points[id_max]->water_value<0)
			std::cout << "AGUA MENOR A 0!!! "<< points[id_max]->water_value<<std::endl;
	}
   // std::cout << " valroes " << id_max << " " << points[id_max]->water_value << std::endl;
}
