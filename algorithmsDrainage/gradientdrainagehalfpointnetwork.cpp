#include "gradientdrainagehalfpointnetwork.h"
#include "iostream"



void GradientDrainageHalfPointNetwork::calculateDrainage(Terrain* ter, glm::vec3 pto, std::vector<glm::vec3>& points_gradient){
    //buscar el triangulo al cual el pto pertenece

    runnel::Triangle* trian = ter->struct_triangle[41];
    calculateNextTriangle(trian, ter->struct_point, points_gradient);


}


void GradientDrainageHalfPointNetwork::calculateNextTriangle(runnel::Triangle* trian, std::vector<runnel::Point*> points, std::vector<glm::vec3>& points_gradient){
    std::unordered_map<int, int> triangle_path;
    runnel::Triangle* trian_ant = 0;
    while(1){
        glm::vec3 pto_half = trian->incentro;

        glm::vec3 vector_movement_gradient = trian->gradient_vector + pto_half;
   //     std::cout << "triangle " << trian->ident << std::endl;
  //      std::cout << trian->gradient_vector.z << std::endl;
    //    std::cout << "movimento del vector" << vector_movement_gradient.x << " " << vector_movement_gradient.y << " " << vector_movement_gradient.z << std::endl;

        if(triangle_path[trian->ident]){
            return;
        }


        runnel::Edge* interception_edge = 0;
        runnel::Edge* interception_edge_old = 0;
     //   glm::vec3 pto_interception_real = glm::vec3(0,0,0);
        float maximun_angle = -100;
        for(runnel::Edge* ed : trian->edges){
            runnel::Triangle* trian_other = calculateOtherTriangleInEdge(trian, ed);
            if(trian_other == 0){
                continue;
            }

            float value = glm::dot(glm::vec2(glm::normalize(trian_other->incentro - trian->incentro)), glm::vec2( trian->gradient_vector));
           // glm::vec3 pto_interception = calculatePointInterception(points[ed->id1]->coord, points[ed->id2]->coord, pto_half, vector_movement_gradient);
          //  std::cout << "punto interception " << pto_interception.x << " " << pto_interception.y << " " << pto_interception.z << std::endl;
//            std::cout << "valor " << value << std::endl;
//            if (isOnEdge(points[ed->id1], points[ed->id2], pto_interception)){
//                interception_edge = ed;
//                pto_interception_real = pto_interception;
//            }
                if(value > maximun_angle){
                    interception_edge_old = interception_edge;
                    interception_edge = ed;
                    maximun_angle = value;
                }
        }
        if(!interception_edge){
    //        std::cout<<"Interception edge no encontrada"<<std::endl;
            return;
        }
        runnel::Triangle* next_triangle = calculateOtherTriangleInEdge(trian, interception_edge);
        if(!next_triangle)
            return;
        if(trian_ant == next_triangle){

            next_triangle = calculateOtherTriangleInEdge(trian, interception_edge_old);
        }
        points_gradient.push_back(pto_half);
        points_gradient.push_back(next_triangle->incentro);
        triangle_path[trian->ident] = 1;
        trian = next_triangle;
       // calculateNextTriangle(next_triangle, points, points_gradient);
    }

}


bool GradientDrainageHalfPointNetwork::isSamePoint(glm::vec3 pto1, glm::vec3 pto2){
    return glm::all(glm::equal(pto1, pto2));
}

glm::vec3 GradientDrainageHalfPointNetwork::calculatePointInterception(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4){
    glm::vec3 a = p2-p1;
    glm::vec3 b = p4-p3;
    glm::vec3 c = p3-p1;
    float s = glm::dot(glm::cross(c, b), glm::cross(a, b))/glm::dot(glm::cross(a, b),glm::cross(a, b));
    glm::vec3 X = p1 + s*a;
    return X;
}

bool GradientDrainageHalfPointNetwork::isOnEdge(runnel::Point* p1, runnel::Point* p2, glm::vec3 pto){
    bool isOn = false;
    glm::vec3 min_edge = glm::min(p1->coord, p2->coord);
    glm::vec3 max_edge = glm::max(p1->coord, p2->coord);
  //  std::cout << std::endl;
 //   std::cout << " pto menor " << p1->coord.x << " " << p1->coord.y << " " << p1->coord.z << std::endl;
//    std::cout << "min " << min_edge.x << "," << min_edge.y << "," << min_edge.z << " max " << max_edge.x << "," << max_edge.y << "," << max_edge.z << std::endl;
//    std::cout << "pto " << pto.x << "," << pto.y <<"," << pto.z << std::endl;
    if(glm::all(glm::lessThanEqual(min_edge, pto)) && glm::all(glm::greaterThanEqual(max_edge, pto)) ){
    //    std::cout << " pertenece" << std::endl;
     //   std::cout << std::endl;
        return true;
    }
    return isOn;
}

runnel::Triangle* GradientDrainageHalfPointNetwork::calculateOtherTriangleInEdge(runnel::Triangle* trian_initial, runnel::Edge* ed){
    for(runnel::Triangle* trian: ed->neighbour_triangle){
        if(!(trian->ident == trian_initial->ident)){
            return trian;
        }
    }
    return (runnel::Triangle*)0;
}
