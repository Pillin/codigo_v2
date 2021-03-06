#ifndef PAINTERTERRAIN_H
#define PAINTERTERRAIN_H

#include "terrain.h"
#include "shaderutils.h"
#include "arbol.h"

#include "painters/glwidget.h"

class PainterTerrain : public GLWidget {
        Q_OBJECT

    public:
        static const int MALLA = 0;
        static const int POLIGONO = 1;
        static const int POINTS = 2;
        PainterTerrain();
        Terrain* ter;
        float min_angle;
        float max_angle;
        float width_line;
        float linewater;
        float exag_z;
        float delta_water;
        int size_points_drainage_peucker;
        int size_points_gradient;
        std::vector<glm::vec3> new_gota;
        std::unordered_map<std::string, glm::vec3> color_conf;

        ShaderUtils shader_terrain;
        ShaderUtils shader_drainage;
        ShaderUtils shader_gradient;
        ShaderUtils shader_angle;
        ShaderUtils shader_angle_edge;
        ShaderUtils shader_callaghan;
        ShaderUtils shader_arbolitos;
        glm::mat4 ortho_matrix;
        glm::mat4 matrix_final;
        std::vector<GLuint> buffer_position_arbolitos;
        std::vector<GLuint> buffer_color_arbolitos;
        std::vector<int> tamano_arbol;
        std::vector<GLuint> buffer_position_arbolitos_type;
        std::vector<GLuint> buffer_color_arbolitos_type;
        std::vector<int> tamano_arbol_type;
        std::unordered_map<std::string, GLuint> attr_buffer;
        void changeAttrConf(std::string name, glm::vec3 value);
        std::unordered_map<std::string, glm::vec3> confColor();
        void changeConf();
        bool checkExists(std::string file);
        void initColorBuffer();
        void drawPointsTerrain();
        void drawNormalTerrain();
        void drawAnglePoint();
        void drawAngleEdge();
        void drawGradientVector();
        void drawGradientPoint();
        void drawArbolitos();
        void drawDrainagePeucker();
        void InitializeVertexBuffer();
        void drawDrainageCallaghan();
        void drawArbolitosType();
        void setTerrain(Terrain* t);
        void paintGL();
        void initGL();
        void calculateHorton(arbol* arb);
        void obtainCaidaWater(glm::vec3 coords);
        void getPath(runnel::Point* pto, std::unordered_map<int, int>& id_used);
        void mouseDoubleClickEvent(QMouseEvent * event);
        void obtainTriangle(glm::vec3 coords);
        void ObtainPositionFromView(int x, int y, glm::mat4 view, glm::mat4 projection);
        void buildBufferArbolitosType(std::vector<arbol*> arbolines, std::vector<std::string> names_type,std::vector<int>& tamano,ShaderUtils shader_name, std::vector<GLuint>& buffer_position, std::vector<GLuint>& buffer_color );
    public slots:
        void changeDeltaWater(double number);
        void changeModelTerrain(int index);
        void changeToRiver();
        void changeLineWater(QString number);
        void changeExag(int number);
        void changeValueLineWidth(int number);
        void changeValueMaxAngle(QString string_number);
        void changeValueMinAngle(QString string_number);

};

#endif // PAINTERTERRAIN_H
