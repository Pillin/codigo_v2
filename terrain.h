#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include "primitives/point.h"
#include "primitives/triangle.h"
#include "lib/glm/glm.hpp"
#include "unordered_map"


class Terrain
{
    public:
        Terrain();

        int width;
        int height;
        float menor_menor;
        float mayor_mayor;
        glm::mat4 matrix_esc;
        glm::vec3 max_bounding;
        glm::vec3 min_bounding;
        glm::vec3 sigma;
        glm::vec3 media;
        std::vector<runnel::Point*> struct_point;
        std::vector<runnel::Triangle*> struct_triangle;
        std::vector<runnel::Edge*> struct_edge;
        std::vector<glm::vec3> points_edge;
        std::vector<glm::vec3> vector_gradient_color;
        std::unordered_map< int, std::vector< runnel::Triangle* > > neigh;
        std::vector<float> count_water;
        float max_value_water;
        std::vector<glm::vec3> position_water_points;

        void addPoint(runnel::Point* p);
        void addTriangle(runnel::Triangle* t);
        void normalize();
        void calculateNeightbour();
        void addEdge(runnel::Edge* ed);
        void setBoundingBox(glm::vec3 coords);
        void addNeighbourPoint( std::unordered_map< int, std::vector< runnel::Triangle* > > n);
        float minumum(std::vector<runnel::Triangle *> list_triangle, runnel::Triangle* trian);

        glm::vec3 getMedia();
        glm::vec3 getSigma();
        std::vector<float> getAngleValuePoints();
        std::vector<runnel::Point*> getPoints();
        std::vector<glm::vec3> getVectorPoints();
        std::vector<glm::vec3> getVectorNormals();
        std::vector<glm::vec3> calculateNeightbourByEdges();
        std::vector<glm::vec3> calculateHeigtArray();
        std::vector<glm::vec3> getDrainageColor();
        std::vector<glm::vec3> getPointsEdgeDrainage();
        std::vector<glm::vec3> getGradientDirectionVector();
        void getMoreWaterPoint();
        void setMapPixel(int ancho_pix, int largo_pix);

};

#endif // TERRAIN_H
