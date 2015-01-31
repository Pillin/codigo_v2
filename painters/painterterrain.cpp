#include "painterterrain.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "shaderutils.h"
#include "lib/glm/gtc/matrix_transform.hpp"
#include "openglutils.h"

#include "algorithmsDrainage/peuckerdrainagenetwork.h"
#include "algorithmsDrainage/gradientdrainagehalfpointnetwork.h"
#include "algorithmsDrainage/gradientdrainagecallaghanmark.h"
#include "buildtreecallaghan.h"
#include "buildtreepeucker.h"
#include "algorithmpatron.h"
#include "zhangguilbertalgorithm.h"

#define BLA_LINE 1

PainterTerrain::PainterTerrain():
    shader_terrain(),
    shader_drainage(),
    shader_gradient(),
    shader_angle(),
    shader_angle_edge(),
    shader_callaghan(),
    shader_arbolitos()
{

    shader_terrain.readFilesShaders(":shader_file/shaders/terrain.vert", ":shader_file/shaders/terrain.frag");
    shader_drainage.readFilesShaders(":shader_file/shaders/drainage.vert", ":shader_file/shaders/drainage.frag");
    shader_gradient.readFilesShaders(":shader_file/shaders/gradient.vert", ":shader_file/shaders/gradient.frag");
    shader_angle.readFilesShaders(":shader_file/shaders/angle.vert", ":shader_file/shaders/angle.frag");
    shader_angle_edge.readFilesShaders(":shader_file/shaders/angle_edge.vert", ":shader_file/shaders/angle_edge.frag");
    shader_callaghan.readFilesShaders(":shader_file/shaders/callaghan.vert", ":shader_file/shaders/callaghan.frag");
    shader_arbolitos.readFilesShaders(":shader_file/shaders/arbol_drenaje.vert", ":shader_file/shaders/arbol_drenaje.frag");
    ter = (Terrain*)0;
    min_angle = -10.0f;
    max_angle = 10.0f;
    width_line = 0.05f;
    linewater = 0.003f;
	delta_water = 100.0f; //0.07
    exag_z = 1.0f;
    size_points_gradient = 0;
    size_points_drainage_peucker = 0;
    this->initColorBuffer();
}

void PainterTerrain::changeAttrConf(std::string name, glm::vec3 value){
    color_conf[name] = value;
    this->changeConf();
}
bool PainterTerrain::checkExists(std::string file)
{
    std::ifstream file_to_check (file.c_str());
    if(file_to_check.is_open())
      return true;
    return false;
    file_to_check.close();
}
//TODO, Change to when change attr only this is overwritten
void PainterTerrain::changeConf(){
    std::ofstream conf_file;
    conf_file.open ("conf_runnel.txt");
    if (conf_file.is_open()){
        for ( auto it = color_conf.begin(); it != color_conf.end(); ++it )
           conf_file << it->first << " " << it->second.x << " " << it->second.y << " " << it->second.z << "\n";
    }

    conf_file.close();
}

void PainterTerrain::initColorBuffer(){
    if(checkExists("conf_runnel.txt")){
        std::ifstream conf_file;
        std::string line;
        std::string name;
        float x; float y; float z;
        conf_file.open ("conf_runnel.txt");
        while ( std::getline (conf_file, line) ){
            std::istringstream  iss(line);
            iss >> name;
            iss >> x;
            iss >> y;
            iss >> z;
            color_conf[name] = glm::vec3(x,y,z);
   //         std::cout << name << " " << x << " " << y << " " << z << std::endl;
        }
        conf_file.close();
    }else{
        color_conf["shader_edge_color_min"] = glm::vec3(0,1,0);
        color_conf["shader_edge_color_max"] = glm::vec3(1,0,0);
        color_conf["shader_callaghan_color"] = glm::vec3(0,0.5,1);
        color_conf["shader_point_color"] = glm::vec3(0,0,0.8);
        color_conf["normal_color"] = glm::vec3(0.0f, 0.0f, 0.0f);
        color_conf["shader_terrain_color"] = glm::vec3(1.0f,1.0f,1.0f);
        color_conf["color_gradient_path"] = glm::vec3(0.0f,0.3f,0.0f);
        color_conf["shader_drainage_color"] = glm::vec3(0.0f,0.3f,0.0f);
        color_conf["shader_drainage_color_peucker"] = glm::vec3(0.0f,0.3f,0.0f);
    }
}

std::unordered_map<std::string, glm::vec3> PainterTerrain::confColor(){
    return color_conf;
}

void PainterTerrain::initGL()
{
    GLenum err = glewInit();
    if (GLEW_OK != err){
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    shader_terrain.InitializeProgram();
    shader_drainage.InitializeProgram();
    shader_gradient.InitializeProgram();
    shader_angle.InitializeProgram();
    shader_angle_edge.InitializeProgram();
    shader_callaghan.InitializeProgram();
    shader_arbolitos.InitializeProgram();

}

void PainterTerrain::setTerrain(Terrain* t)
{
    ter = t;
    PeuckerDrainageNetwork::calculateGrid(ter);
    PainterTerrain::InitializeVertexBuffer();

    ortho_matrix = glm::ortho<GLfloat>(-1.0f, 1.0f, -1.0f, 1.0f, -10.f, 10.f);
    glm::mat4 matrix = glm::mat4();
    float max_sigma = std::max(ter->sigma.x, ter->sigma.y);
    max_sigma = std::max(max_sigma, ter->sigma.z);
    matrix = glm::scale(matrix, glm::vec3(1,1,1)/max_sigma);
    matrix = t->matrix_esc * matrix;
    matrix = glm::translate(matrix, -ter->media);
    this->GLWidget::model_matrix = matrix;
}

void PainterTerrain::InitializeVertexBuffer()
{
    shader_terrain.saveAttribute("position");

    shader_gradient.saveAttribute("position");

    shader_angle.saveAttribute("position");
    shader_angle.saveAttribute("valuecolor");

    shader_angle_edge.saveAttribute("position");
    shader_angle_edge.saveAttribute("height_angle");
    shader_angle_edge.saveAttribute("angle_edge");

    shader_drainage.saveAttribute("position");
    shader_drainage.saveAttribute("valuecolor");

    shader_callaghan.saveAttribute("position");
    shader_callaghan.saveAttribute("watercount");
    shader_arbolitos.saveAttribute("position");
    shader_arbolitos.saveAttribute("valuecolor");

    std::vector<glm::vec3> vertex_position_terrain = ter->getVectorPoints();
    std::vector<glm::vec3> normal_triangle = ter->getVectorNormals();
    std::vector<float> angle_value_point = ter->getAngleValuePoints();
    std::vector<glm::vec3> angle_value_edge = ter->calculateNeightbourByEdges();
    std::vector<glm::vec3> vertex_height = ter->calculateHeigtArray();

    std::vector<glm::vec3> vertex_position_gradient = ter->getGradientDirectionVector();
    std::vector<glm::vec3> vertex_position_gradient_color = ter->vector_gradient_color;
  //  std::cout << "vertex_position_gradient_color " << vertex_position_gradient_color.size() << std::endl;



    std::vector<glm::vec3> vertex_color_drainage_peucker = ter->getDrainageColor();
    std::vector<glm::vec3> vertex_position_drainage_peucker = ter->getPointsEdgeDrainage();
    size_points_drainage_peucker  = vertex_position_drainage_peucker.size();



 //   std::cout << "vertex_position_drainage_peucker " << vertex_position_drainage_peucker.size() << std::endl;
    GradientDrainageCallaghanMark gdcm =  GradientDrainageCallaghanMark();
	std::cout << "delta_water runAlgorithm "<<delta_water<<std::endl;
    gdcm.runAlgorithm(ter, delta_water );
    ter->getMoreWaterPoint();
    std::vector<float> count_water = ter->count_water;
 //   std::cout << "count water" << count_water.size() << std::endl;
    std::vector<glm::vec3> water_position = ter->position_water_points;
  //  std::cout << "water position " << water_position.size() << std::endl;



    shader_callaghan.bufferCreate(attr_buffer["buffer_color_drainage_callaghan"], count_water );
    shader_callaghan.bufferCreate(attr_buffer["buffer_position_drainage_callaghan"], water_position);

	shader_drainage.bufferCreate(attr_buffer["buffer_color_drainage_peucker"], vertex_color_drainage_peucker);
    shader_drainage.bufferCreate(attr_buffer["buffer_position_drainage_peucker"], vertex_position_drainage_peucker);

    shader_drainage.bufferCreate(attr_buffer["buffer_color_gradient_triangle"], vertex_position_gradient_color);
    shader_drainage.bufferCreate(attr_buffer["buffer_gradient_triangle"], vertex_position_gradient);



    shader_terrain.bufferCreate(attr_buffer["buffer_position_points_terrain"], vertex_position_terrain);
    shader_gradient.bufferCreate(attr_buffer["buffer_position_normal_triangle"], normal_triangle);

    shader_angle.bufferCreate(attr_buffer["buffer_position_points_terrain_angle"], vertex_position_terrain);
    shader_angle.bufferCreate(attr_buffer["buffer_color_angle_point"], angle_value_point);

    shader_angle_edge.bufferCreate(attr_buffer["buffer_color_angle_edge"], angle_value_edge);
    shader_angle_edge.bufferCreate(attr_buffer["buffer_vertex_height"], vertex_height);
    shader_angle_edge.bufferCreate(attr_buffer["buffer_position_angle_edge"], vertex_position_terrain);



	buildTreeCallaghan ip (ter->struct_point, 0.01f/10000000.0f);
	//BuildTreePeucker ip(ter->struct_point, ter);
    std::vector<arbol*> arbolines = ip.reviewPoints();
    if(arbolines.size() > 0){
     //   std::cout << "arbolitps " << std::endl;
        for(arbol* aa : arbolines){
            GLuint buffer_arbolito;
            std::vector<glm::vec3> ed_ar;
            aa->getArbolEdges(ed_ar);
            shader_arbolitos.bufferCreate(buffer_arbolito, ed_ar);
            tamano_arbol.push_back(ed_ar.size());
            buffer_position_arbolitos.push_back(buffer_arbolito);

            GLuint buffer_arbolito_color;
            std::vector<glm::vec3> ed_ar_col;
            aa->getColorEdges(ed_ar_col);
            shader_arbolitos.bufferCreate(buffer_arbolito_color, ed_ar_col);
            buffer_color_arbolitos.push_back(buffer_arbolito_color);
        }

    }

    ZhangGuilbertAlgorithm la;
    la.setData(ter, arbolines);
    std::vector<std::string> names_type = la.runAlgoritm();
    this->buildBufferArbolitosType(arbolines,names_type,tamano_arbol_type,shader_arbolitos,buffer_position_arbolitos_type,buffer_color_arbolitos_type );



}

void PainterTerrain::buildBufferArbolitosType(std::vector<arbol*> arbolines, std::vector<std::string> names_type,std::vector<int>& tamano,ShaderUtils shader_name, std::vector<GLuint> &buffer_position, std::vector<GLuint>& buffer_color ){
   // std::cout << "entro aki " << std::endl;
    if(arbolines.size() > 0){
     //   std::cout << "arbolitps " << std::endl;
        int num = 0;
        for(arbol* aa : arbolines){
            GLuint buffer_arbolito;
            std::vector<glm::vec3> ed_ar;
            aa->getArbolEdges(ed_ar);
            shader_name.bufferCreate(buffer_arbolito, ed_ar);
            tamano.push_back(ed_ar.size());
            buffer_position.push_back(buffer_arbolito);

            GLuint buffer_arbolito_color;
            std::vector<glm::vec3> ed_ar_col;
            aa->getColorEdgesType(ed_ar_col, names_type[num]);
            shader_name.bufferCreate(buffer_arbolito_color, ed_ar_col);
            buffer_color.push_back(buffer_arbolito_color);
            num++;
        }
    }
}



void PainterTerrain::paintGL(){
    this->GLWidget::paintGL();
    if(!ter){
        std::cout << "No hay terreno para pintar"<<std::endl;
        return;
    }
    this->GLWidget::model_view_matrix = glm::translate(glm::mat4(),glm::vec3(0.0f,0.0f, -5.0f))*this->GLWidget::matrix_glwidget*this->GLWidget::model_matrix;
    matrix_final = ortho_matrix*this->GLWidget::model_view_matrix;

    this->drawPointsTerrain();
    if( this->GLWidget::normal_view ){//N
        this->drawNormalTerrain();
    }
    if (this->GLWidget::angle_edge){ //A
        this->drawAngleEdge();
    }
    if (this->GLWidget::angle_point){//S
        this->drawAnglePoint();
    }



    if(this->GLWidget::drainage_callaghan){//C
        this->drawDrainageCallaghan();
    }
    if(this->GLWidget::drainage_view){//D
        this->drawDrainagePeucker();
    }
    if(this->GLWidget::gradient_view){//G
        this->drawGradientVector();
    }

    if(this->GLWidget::arbolito_mode){
        this->drawArbolitos();
    }
    if(this->GLWidget::arbolito_mode_type){
        this->drawArbolitosType();
    }

    this->drawGradientPoint();


    OpenGLUtils::printOpenGLError();
}

void PainterTerrain::drawArbolitos(){
    int i = 0;
    for( GLuint buffer_id: buffer_position_arbolitos){
        if(tamano_arbol[i] > 4){
            glDisable(GL_DEPTH_TEST);
			glLineWidth(BLA_LINE);
            glUseProgram(shader_arbolitos.theProgram);
            shader_arbolitos.setUniform("mvp", matrix_final);

            shader_arbolitos.setUniform("exag", exag_z);
            shader_arbolitos.linkBufferWithAttr(buffer_position_arbolitos[i], "position", 3);
            shader_arbolitos.linkBufferWithAttr(buffer_color_arbolitos[i], "valuecolor", 3);
            glDrawArrays(GL_LINES, 0, tamano_arbol[i]);
          //  std::cout<< "Tamano arbolito("<<buffer_id<<"): "<<tamano_arbol[i]<<std::endl;
            glLineWidth(1);
            glEnable(GL_DEPTH_TEST);
        }
      //  break;
        ++i;
    }

}


void PainterTerrain::drawArbolitosType(){
    int i = 0;
   // std::cout << "entro a dibujar" << std::endl;
    for( GLuint buffer_id: buffer_position_arbolitos_type){
        if(tamano_arbol_type[i] > 4){
            glDisable(GL_DEPTH_TEST);
			glLineWidth(BLA_LINE);
            glUseProgram(shader_arbolitos.theProgram);
            shader_arbolitos.setUniform("mvp", matrix_final);

            shader_arbolitos.setUniform("exag", exag_z);
            shader_arbolitos.linkBufferWithAttr(buffer_position_arbolitos_type[i], "position", 3);
            shader_arbolitos.linkBufferWithAttr(buffer_color_arbolitos_type[i], "valuecolor", 3);
            glDrawArrays(GL_LINES, 0, tamano_arbol_type[i]);
          //  std::cout<< "Tamano arbolito("<<buffer_id<<"): "<<tamano_arbol[i]<<std::endl;
            glLineWidth(1);
            glEnable(GL_DEPTH_TEST);
        }
      //  break;
        ++i;
    }

}



void PainterTerrain::drawDrainagePeucker(){

    if (size_points_drainage_peucker>0){
        glDisable(GL_DEPTH_TEST);
		glLineWidth(BLA_LINE);
        glUseProgram(shader_drainage.theProgram);
        shader_drainage.setUniform("exag", exag_z);
        shader_drainage.setUniform("mvp", matrix_final);
        shader_drainage.setUniform("color_drainage", color_conf["shader_drainage_color_peucker"]);
        shader_drainage.linkBufferWithAttr(attr_buffer["buffer_position_drainage_peucker"], "position", 3);
        shader_drainage.linkBufferWithAttr(attr_buffer["buffer_color_drainage_peucker"], "valuecolor", 3);
        glDrawArrays(GL_LINES, 0, size_points_drainage_peucker);
        glLineWidth(1);
        glEnable(GL_DEPTH_TEST);
    }

}



void PainterTerrain::drawDrainageCallaghan(){
	glLineWidth(BLA_LINE);
    glUseProgram(shader_callaghan.theProgram);
    shader_callaghan.setUniform("exag", exag_z);
    shader_callaghan.setUniform("mvp", matrix_final);
    shader_callaghan.setUniform("color_callaghan", color_conf["shader_callaghan_color"]);

    shader_callaghan.setUniform("linewater", linewater);
//    std::cout << "max value water " << ter->max_value_water << std::endl;
    shader_callaghan.setUniform("maxwater", ter->max_value_water);
    shader_callaghan.linkBufferWithAttr(attr_buffer["buffer_position_drainage_callaghan"], "position", 3);
    shader_callaghan.linkBufferWithAttr(attr_buffer["buffer_color_drainage_callaghan"], "watercount", 1);
    glDrawArrays(GL_LINES, 0, ter->struct_edge.size()*2);
    glLineWidth(1);
}


void PainterTerrain::drawGradientVector(){
    glDisable(GL_DEPTH_TEST);
	glLineWidth(BLA_LINE);
    glUseProgram(shader_drainage.theProgram);
    shader_drainage.setUniform("exag", exag_z);
    shader_drainage.setUniform("mvp", matrix_final);
    shader_drainage.setUniform("color_drainage", color_conf["shader_drainage_color"]);
    shader_drainage.linkBufferWithAttr(attr_buffer["buffer_gradient_triangle"], "position", 3);
    shader_drainage.linkBufferWithAttr(attr_buffer["buffer_color_gradient_triangle"], "valuecolor", 3);
    glDrawArrays(GL_LINES, 0, ter->struct_triangle.size()*2);
    glLineWidth(1);
    glEnable(GL_DEPTH_TEST);
}


void PainterTerrain::drawGradientPoint(){
    if (size_points_gradient > 0){
        glDisable(GL_DEPTH_TEST);
		glLineWidth(BLA_LINE);
        glUseProgram(shader_gradient.theProgram);
        shader_gradient.setUniform("exag", exag_z);
        shader_gradient.setUniform("mvp", matrix_final);
        shader_gradient.setUniform("valuecolor", color_conf["color_gradient_path"]);
        shader_gradient.linkBufferWithAttr(attr_buffer["buffer_position_gradient_drainage"], "position", 3);
        glDrawArrays(GL_LINES, 0, size_points_gradient);
        glLineWidth(1);
        glEnable(GL_DEPTH_TEST);
    }
}


void PainterTerrain::drawAnglePoint(){
    glDisable(GL_DEPTH_TEST);
	glLineWidth(BLA_LINE);
    glUseProgram(shader_angle.theProgram);
    shader_angle.setUniform("mvp", matrix_final);
    shader_angle.setUniform("exag", exag_z);
    shader_angle.setUniform("color_angle_point", color_conf["shader_point_color"]);

    shader_angle.linkBufferWithAttr(attr_buffer["buffer_position_points_terrain_angle"], "position", 3);
    shader_angle.linkBufferWithAttr(attr_buffer["buffer_color_angle_point"], "valuecolor", 1);
    glDrawArrays(GL_TRIANGLES, 0, ter->struct_triangle.size()*6);
    glLineWidth(1);
    glEnable(GL_DEPTH_TEST);
}


void PainterTerrain::drawAngleEdge(){
    glDisable(GL_DEPTH_TEST);
	glLineWidth(BLA_LINE);
    glUseProgram(shader_angle_edge.theProgram);
    shader_angle_edge.setUniform("mvp", matrix_final);
    shader_angle_edge.setUniform("exag", exag_z);
    shader_angle_edge.setUniform("linewidth", width_line);
    shader_angle_edge.setUniform("cotamaxangle", max_angle);
    shader_angle_edge.setUniform("cotaminangle", min_angle);
    shader_angle_edge.setUniform("color_min", color_conf["shader_edge_color_min"]);
    shader_angle_edge.setUniform("color_max", color_conf["shader_edge_color_max"]);
    shader_angle_edge.linkBufferWithAttr(attr_buffer["buffer_position_angle_edge"], "position", 3);
    shader_angle_edge.linkBufferWithAttr(attr_buffer["buffer_color_angle_edge"], "angle_edge", 3);
    shader_angle_edge.linkBufferWithAttr(attr_buffer["buffer_vertex_height"], "height_angle", 3);
    glDrawArrays(GL_TRIANGLES, 0, ter->struct_triangle.size()*6);
    glLineWidth(1);
    glEnable(GL_DEPTH_TEST);
}


void PainterTerrain::drawPointsTerrain(){
    glUseProgram(shader_terrain.theProgram);
    shader_terrain.setUniform("mvp", matrix_final);
    shader_terrain.setUniform("promz",ter->max_bounding.z);
    shader_terrain.setUniform("exag",exag_z);
    shader_terrain.setUniform("color_terrain",color_conf["shader_terrain_color"] );
    shader_terrain.linkBufferWithAttr(attr_buffer["buffer_position_points_terrain"], "position", 3);
    glDrawArrays(GL_TRIANGLES, 0, ter->struct_triangle.size()*3);
}


void PainterTerrain::drawNormalTerrain(){
    glUseProgram(shader_gradient.theProgram);
    shader_gradient.setUniform("mvp", matrix_final);
    shader_gradient.setUniform("exag", exag_z);
    shader_gradient.setUniform("valuecolor", color_conf["normal_color"]);
    shader_gradient.linkBufferWithAttr(attr_buffer["buffer_position_normal_triangle"], "position", 3);
    glDrawArrays(GL_LINES, 0, ter->struct_triangle.size()*3);
}


void PainterTerrain::mouseDoubleClickEvent(QMouseEvent * event){
    this->ObtainPositionFromView(event->x(), event->y(),this->GLWidget::model_view_matrix, ortho_matrix);
}


void PainterTerrain::ObtainPositionFromView(int x, int y, glm::mat4 view, glm::mat4 projection){
    int window_width = this->width();
    int window_height = this->height();
  //  std::cout << "Windows size obtain: "<<window_height<<", "<<window_width<<std::endl;

    GLfloat depth;
    glReadPixels(x, window_height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
 //   std::cout << "Clicked on pixel "<< x << " " << y << " depth " << depth << std::endl;

    glm::vec4 viewport = glm::vec4(0, 0, window_width, window_height);
    glm::vec3 wincoord = glm::vec3(x, window_height - y - 1, depth);
    glm::vec3 objcoord = glm::unProject(wincoord, view, projection, viewport);

  //  std::cout << "Doble click en (" << x << ", " << y <<", " << depth << ") -> (" << objcoord.x << ", " << objcoord.y << ", " << objcoord.z << ")" << std::endl;
   // this->obtainTriangle(objcoord);
    this->obtainCaidaWater(objcoord);
}



void PainterTerrain::obtainTriangle(glm::vec3 coords){
    float minimum_distance = glm::distance(ter->struct_triangle[0]->incentro,coords);
    runnel::Triangle* triangle_minimum_distance = ter->struct_triangle[0];
    for(runnel::Triangle* trian : ter->struct_triangle){
        float distance = glm::distance(trian->incentro, coords);
        if( distance < minimum_distance){
            triangle_minimum_distance = trian;
            minimum_distance = distance;
        }

    }
    std::vector<glm::vec3> points_gradient;
    GradientDrainageHalfPointNetwork::calculateNextTriangle(triangle_minimum_distance, ter->struct_point, points_gradient);

    if( attr_buffer["buffer_position_gradient_drainage"] != 0){
        glDeleteBuffers(1, &attr_buffer["buffer_position_gradient_drainage"] );
        size_points_gradient = 0;
        attr_buffer["buffer_position_gradient_drainage"] = 0;
    }
    if(points_gradient.size() == 0){
        std::cout << "Salieron 0"<<std::endl;
        return;
    }

    shader_terrain.bufferCreate( attr_buffer["buffer_position_gradient_drainage"], points_gradient);
    size_points_gradient = points_gradient.size();
    this->GLWidget::updateGL();
}

void PainterTerrain::changeValueLineWidth(int number){
    width_line = number/1000.0f;
    this->GLWidget::updateGL();
}

void PainterTerrain::changeValueMaxAngle(QString string_number){
    max_angle = string_number.toFloat();
    this->GLWidget::updateGL();
}

void PainterTerrain::changeValueMinAngle(QString string_number){
    min_angle = string_number.toFloat();
    this->GLWidget::updateGL();
}


void PainterTerrain::obtainCaidaWater(glm::vec3 coords){
    float minimum_distance = glm::distance(glm::vec2(ter->struct_point[0]->coord),glm::vec2(coords));
    runnel::Point* pto_minimum = ter->struct_point[0];
    for(runnel::Point*pto : ter->struct_point){
        float dist = glm::distance(glm::vec2(pto->coord), glm::vec2(coords));
        if(dist < minimum_distance){
            pto_minimum = pto;
            minimum_distance = dist;

        }
    }

 //   std::cout << "obtain caida de agua count water " << std::endl;
    std::vector<glm::vec3> points_gradient;
    new_gota = {};
    std::unordered_map<int, int> id_used;
    this->getPath(pto_minimum, id_used);
    points_gradient = new_gota;

    if(attr_buffer["buffer_position_gradient_drainage"] != 0){
        glDeleteBuffers(1, &attr_buffer["buffer_position_gradient_drainage"] );
        size_points_gradient = 0;
        attr_buffer["buffer_position_gradient_drainage"] = 0;
    }
    if(points_gradient.size() == 0){
        std::cout << "Salieron 0"<<std::endl;
        return;
    }

    shader_terrain.bufferCreate( attr_buffer["buffer_position_gradient_drainage"], points_gradient);
    size_points_gradient = points_gradient.size();

    this->GLWidget::updateGL();
}

void PainterTerrain::getPath(runnel::Point* pto, std::unordered_map<int, int>& id_used){

    std::vector<int> position_neightbour;
    position_neightbour.push_back(pto->ident - ter->width);
    position_neightbour.push_back(pto->ident + ter->width);
    if(pto->ident%ter->width > 0){
        position_neightbour.push_back(pto->ident + ter->width - 1);
        position_neightbour.push_back(pto->ident - ter->width - 1);
        position_neightbour.push_back(pto->ident - 1);
    }
    if(pto->ident%ter->width < ter->width-1){
        position_neightbour.push_back(pto->ident + 1);
        position_neightbour.push_back(pto->ident - ter->width + 1);
        position_neightbour.push_back(pto->ident + ter->width + 1);
    }
  //  std::cout << "getPath " << std::endl;
    float max_z = -1;
    int id_max = -1;
    for (int id : position_neightbour){
        if(id >= 0 && id < ter->struct_point.size()){
 //           std::cout << "id " << ter->struct_point[id]->ident << " " << id << " valor del punto " << ter->struct_point[id]->coord.z << " valor de la otra parte "<< pto->coord.z  << " del delta " << delta_water << std::endl;
            if( ter->struct_point[id]->coord.z <= (pto->coord.z + delta_water )){
                if(id_used[id] == 0){

                    float dist_z = (pto->coord.z + delta_water ) - ter->struct_point[id]->coord.z;
                    if( max_z < dist_z){
                        max_z = dist_z;
                        id_max = id;
                    }
                }else{
                 //   std::cout << "fue usado " << std::endl;
                }
            }
        }
    }
    if(id_max != -1 && id_used[id_max] == 0){
        id_used[id_max] = 1;
        new_gota.push_back(pto->coord);
        new_gota.push_back(ter->struct_point[id_max]->coord);
        this->getPath(ter->struct_point[id_max], id_used);
    }else{
        std::cout << "valor del pto " << id_used[id_max] << " id del punto " << id_max << std::endl;
    }

}

void PainterTerrain::changeModelTerrain(int index){
    switch (index) {
        case MALLA:
            this->GLWidget::drawing_mode = this->GLWidget::WIREFRAME;
            break;
        case POLIGONO:
            this->GLWidget::drawing_mode = this->GLWidget::FILL_POLYGONS;
            break;
        case POINTS:
            this->GLWidget::drawing_mode = this->GLWidget::NUBE_PUNTOS;
            break;
        default:
            break;
    }
    this->GLWidget::updateGL();

}

void PainterTerrain::changeExag(int number){
//    std::cout << "exageracion en " << number << std::endl;
    exag_z = number*1.0f;
}

void PainterTerrain::changeToRiver(){
    this->GLWidget::angle_edge = !this->GLWidget::angle_edge;
    this->GLWidget::updateGL();
}

void PainterTerrain::changeLineWater(QString number){
    linewater = number.toFloat();
    this->GLWidget::updateGL();
}

void PainterTerrain::changeDeltaWater(double number){
    delta_water = (float)number/100.0f;
    std::cout << "numero de conf para el delta de agua " << number << std::endl;
}

