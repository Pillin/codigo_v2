#include "builderterrain.h"
#include <QProcess>
#include <iostream>
#include "primitives/triangle.h"
#include <unordered_map>

BuilderTerrain::BuilderTerrain()
{
}

void BuilderTerrain::getAllPoints(std::vector<runnel::Point*> &points_terran, std::stringstream& vertexstream){

    std::cout << "Agregando los puntos para enviarlo a Qhull. Cantidad de puntos: " << points_terran.size() << std::endl;
    vertexstream << points_terran.size() << std::endl;
    for (runnel::Point* point : points_terran){
 //       std::cout << point->ident << " " "ptos en Qhull " << point->coord.x << " " << point->coord.y << std::endl;
        vertexstream << point->coord.x << " " << point->coord.y << std::endl;
    }
}

void BuilderTerrain::getTriangle(Terrain* ter){
	std::cout << "Creando estructura con la triangulacion" << std::endl;
	std::unordered_map<int, std::vector< runnel::Triangle*> > neighbours;
	std::unordered_map< int, std::unordered_map<int, runnel::Edge*> > edges_neigh;
	std::vector<runnel::Point*> &points_terran = ter->struct_point; // sino tuviera el & seria una copia
	int triangle_count = 0;
	for(int h = 0; h<ter->height-1;h++){
		for(int w = 0; w<ter->width-1;w++){
			int topleft = h*ter->width+w;
			int p11 = topleft;
			int p12 = topleft+1;
			int p21 = topleft+ter->width;
			int p22 = topleft+ter->width+1;

			runnel::Triangle *trian = new runnel::Triangle(triangle_count);
			this->buildStruct(ter, trian, p21, p12, p11);

			std::vector<int> pto { p21, p12, p11 };
			this->buildNeighbourhood(ter, neighbours, trian, pto );
			this->buildNeighbourhoodByEdges(ter, trian, edges_neigh, p21, p12);
			this->buildNeighbourhoodByEdges(ter, trian, edges_neigh, p12, p11);
			this->buildNeighbourhoodByEdges(ter, trian, edges_neigh, p11, p21);
			ter->addTriangle(trian);

			triangle_count++;
			trian = new runnel::Triangle(triangle_count);
			this->buildStruct(ter, trian, p22, p12, p21);

			std::vector<int> pto2 { p22, p12, p21 };
			this->buildNeighbourhood(ter, neighbours, trian, pto2 );
			this->buildNeighbourhoodByEdges(ter, trian, edges_neigh, p22, p12);
			this->buildNeighbourhoodByEdges(ter, trian, edges_neigh, p12, p21);
			this->buildNeighbourhoodByEdges(ter, trian, edges_neigh, p21, p22);
			ter->addTriangle(trian);
			triangle_count++;
		}
	}
	ter->addNeighbourPoint(neighbours);
}

void BuilderTerrain::getTriangle(Terrain* ter, QByteArray& result){
    std::cout << "Creando estructura con la triangulacion" << std::endl;

    std::stringstream stream;
    stream << std::string(result.data());
    std::unordered_map<int, std::vector< runnel::Triangle*> > neighbours;
    std::unordered_map< int, std::unordered_map<int, runnel::Edge*> > edges_neigh;
   // std::unordered_multimap

    int triangle_count;
    stream >> triangle_count;

    for(int i = 0; i< triangle_count ; ++i){
       int pto1, pto2, pto3;
       stream >> pto1;
       stream >> pto2;
       stream >> pto3;


       runnel::Triangle *trian = new runnel::Triangle(i);
       this->buildStruct(ter, trian, pto1, pto2, pto3);

       std::vector<int> pto { pto1, pto2, pto3 };
       this->buildNeighbourhood(ter, neighbours, trian, pto );
       this->buildNeighbourhoodByEdges(ter, trian, edges_neigh, pto1, pto2);
       this->buildNeighbourhoodByEdges(ter, trian, edges_neigh, pto2, pto3);
       this->buildNeighbourhoodByEdges(ter, trian, edges_neigh, pto3, pto1);
       ter->addTriangle(trian);

       if(!stream)
          break;

    }
    ter->addNeighbourPoint(neighbours);



}

void BuilderTerrain::runTriangulation(Terrain* ter){
    std::cout << "Enviando los puntos a Qhull" << std::endl;
    std::vector<runnel::Point*> &points_terran = ter->struct_point; // sino tuviera el & seria una copia
    std::stringstream vertexfile;

    vertexfile << "2" << std::endl;
    getAllPoints(points_terran, vertexfile);

    QProcess qhull;
	QString program2="D:/Desarrollo/QtProjects/qhull-2012.1/bin/qhull.exe";

    qhull.start(program2, QStringList() << " d Qt i");

    if (!qhull.waitForStarted())
         std::cout << "Error with wait for started" << std::endl;

    long c = qhull.write(vertexfile.str().c_str());
    qhull.closeWriteChannel();

    if (!qhull.waitForFinished(-1))
        std::cout << "Error with the wait for finished" << std::endl;

    if(qhull.exitCode()!=0)
        std::cout << " Error " << std::endl;

    QByteArray result = qhull.readAllStandardOutput();
    BuilderTerrain::getTriangle(ter, result);
}

glm::vec3 BuilderTerrain::calculateNormal(runnel::Point* p1, runnel::Point* p2, runnel::Point* p3){
    glm::vec3 vector_a = p2->coord - p1->coord;
    glm::vec3 vector_b = p3->coord - p1->coord;
    glm::vec3 value_normal = glm::normalize(glm::cross(vector_a, vector_b));
    return value_normal;
}

void BuilderTerrain::buildStruct(Terrain* ter, runnel::Triangle* trian, int pto1, int pto2, int pto3){
    glm::vec3 normal = this->calculateNormal(ter->struct_point[pto1],ter->struct_point[pto2], ter->struct_point[pto3]);
    if(normal.z < 0){
        normal = -normal;
        int aux = pto3;
        pto3 = pto2;
        pto2 = aux;
    }

    trian->addGroupPoints(ter->struct_point[pto1],ter->struct_point[pto2], ter->struct_point[pto3] );


    trian->normal = normal;
    trian->calculateIncentroPoint();
}


void BuilderTerrain::buildNeighbourhood(Terrain* ter, std::unordered_map<int, std::vector< runnel::Triangle*> >& neigh, runnel::Triangle *trian, std::vector< int> pto ){
    for( int p: pto){
        int id = ter->struct_point[p]->ident;
        if (neigh.find(id) == neigh.end()){
            neigh[id] = std::vector<runnel::Triangle*>();
            neigh[id].push_back(trian);
        }
    }

}
void BuilderTerrain::buildNeighbourhoodByEdges(Terrain* ter, runnel::Triangle* trian, std::unordered_map< int, std::unordered_map<int, runnel::Edge*> > & edges_neigh, int pto0, int pto1){
    int e1_min = std::min(pto0, pto1);
    int e1_max = std::max(pto0, pto1);
    runnel::Edge* ed1 = new runnel::Edge(ter->struct_point[pto0], ter->struct_point[pto1]);
    ed1->addTriangle(trian);
    if (edges_neigh.find(e1_min) == edges_neigh.end()){ //no ta
        std::unordered_map<int,runnel::Edge * > aux_min;
        aux_min[e1_max] = ed1;
        edges_neigh[e1_min] = aux_min;
        ter->addEdge(ed1);
    }else{
        if(edges_neigh[e1_min].find(e1_max) ==  edges_neigh[e1_min].end()){//no ta en el segundo
            edges_neigh[e1_min][e1_max] = ed1;
            ter->addEdge(ed1);
        }else{//si ta
            delete ed1;
            ed1 = edges_neigh[e1_min][e1_max];
            ed1->addTriangle(trian);
            edges_neigh[e1_min][e1_max] = ed1;
        }
    }
    trian->addEdge(ed1);


}


