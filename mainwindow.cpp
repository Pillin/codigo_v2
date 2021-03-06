#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <sstream>

#include <QFileDialog>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    runnel_controller(),
    conf(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
    std::cout << "Start Aplication..." << std::endl;
    QWebSettings *settings = QWebSettings::globalSettings();
    settings->setAttribute(QWebSettings::JavascriptEnabled,true);
    settings->setAttribute(QWebSettings::PluginsEnabled, true);
    this->connectSignalForRunnel();
    ui->mapa_google->layout()->addWidget( &runnel_controller.getGoogleMap());
    ui->malla_see->layout()->addWidget(&runnel_controller.getPainter());
    conf = runnel_controller.getColorConf();
    this->confColorRunnel();

}

MainWindow::~MainWindow()
{
    QWebSettings::clearMemoryCaches();
    delete ui;
}

void MainWindow::connectSignalForRunnel(){
//    QObject::connect(ui->actionExageration_x1, SIGNAL(triggered()), &runnel_controller.getPainter(), SLOT(changeExagZ1()));
//    QObject::connect(ui->actionExageration_x3, SIGNAL(triggered()), &runnel_controller.getPainter(), SLOT(changeExagZ3()));
    QObject::connect(ui->actionTIFF, SIGNAL(triggered()),this , SLOT(getTypeTIFFForObtainNameFile()));
    QObject::connect(ui->actionRunnel, SIGNAL(triggered()),this , SLOT(getTypeRunnelForObtainNameFile()));
//    QObject::connect(ui->button_malla, SIGNAL(clicked()), &runnel_controller.getPainter(), SLOT(changeToMalla()) );
//    QObject::connect(ui->button_nube, SIGNAL(clicked()), &runnel_controller.getPainter(), SLOT(changeToNube()) );
//    QObject::connect(ui->button_poligono, SIGNAL(clicked()), &runnel_controller.getPainter(), SLOT(changeToPoligono()) );
//    QObject::connect(ui->button_river, SIGNAL(clicked()), &runnel_controller.getPainter(), SLOT(changeToRiver()) );

    QObject::connect(ui->modelform, SIGNAL(currentIndexChanged(int)), &runnel_controller.getPainter(), SLOT(changeModelTerrain(int)) );
    QObject::connect(ui->exag, SIGNAL(valueChanged(int)),&runnel_controller.getPainter(), SLOT(changeExag(int)));
    QObject::connect(ui->line_width, SIGNAL(valueChanged(int)), &runnel_controller.getPainter(), SLOT(changeValueLineWidth(int)));
    QObject::connect(ui->max_angle, SIGNAL(textChanged(QString)), &runnel_controller.getPainter(), SLOT(changeValueMaxAngle(QString)));
    QObject::connect(ui->min_angle, SIGNAL(textChanged(QString)), &runnel_controller.getPainter(), SLOT(changeValueMinAngle(QString)));
    QObject::connect(ui->linewater, SIGNAL(textChanged(QString)), &runnel_controller.getPainter(), SLOT(changeLineWater(QString)));
    QObject::connect(ui->deltawater, SIGNAL(valueChanged(double)), &runnel_controller.getPainter(), SLOT(changeDeltaWater(double)));
    QObject::connect(ui->color_peucker_boton, SIGNAL(clicked()), this, SLOT(changeColor1()));
    QObject::connect(ui->color_angle_minimum_edge_button, SIGNAL(clicked()), this, SLOT(changeColor2()));
    QObject::connect(ui->color_angle_maximo_edge_button, SIGNAL(clicked()), this, SLOT(changeColor3()));
    QObject::connect(ui->color_angle_point_button, SIGNAL(clicked()), this, SLOT(changeColor4()));
    QObject::connect(ui->color_callaghan_button, SIGNAL(clicked()), this, SLOT(changeColor5()));
    QObject::connect(ui->color_drainage_button, SIGNAL(clicked()), this, SLOT(changeColor6()));
    QObject::connect(ui->color_normal_button, SIGNAL(clicked()), this, SLOT(changeColor7()));
    QObject::connect(ui->color_path_button, SIGNAL(clicked()), this, SLOT(changeColor8()));
    QObject::connect(ui->color_terrain_button, SIGNAL(clicked()), this, SLOT(changeColor9()));
}

void MainWindow::getTypeTIFFForObtainNameFile(){
    QString extension_tiff = "All Files (*);;tiff(*.tif)";
    std::cout << "Obtaining extension file " << std::endl;
    this->getObtainNameFile(extension_tiff, "TIFF");
}
void MainWindow::getTypeRunnelForObtainNameFile(){
    QString extension_runnel = "All Files (*);;Runnel(*.runnel)";
    std::cout << "Obtaining extension file... " << std::endl;
    this->getObtainNameFile(extension_runnel, "RUNNEL");
}

void MainWindow::getObtainNameFile(QString extension_name, std::string type_file){
    std::cout << "Obtaining name of file..." << std::endl;
    QString title_window = "Open File ";
    QString nameFile = QFileDialog::getOpenFileName(this, (title_window), "",extension_name );
    if (nameFile.size() == 0){
        return;
    }
    std::cout << "name of file is " << nameFile.toStdString() << std::endl;
    runnel_controller.obtainFileTerrain(nameFile, type_file);

}

void MainWindow::confColorRunnel(){

    ui->color_peucker->setStyleSheet(getColor(conf["shader_drainage_color_peucker"]));
    ui->color_callaghan->setStyleSheet(getColor(conf["shader_callaghan_color"]));
    ui->color_angle_minimum_edge->setStyleSheet(getColor(conf["shader_edge_color_min"]));
    ui->color_angle_maximo_edge->setStyleSheet(getColor(conf["shader_edge_color_max"]));
    ui->color_angle_point->setStyleSheet(getColor(conf["shader_point_color"]));
    ui->color_normal->setStyleSheet(getColor(conf["normal_color"]));
    ui->color_terrain->setStyleSheet(getColor(conf["shader_terrain_color"]));
    ui->color_drainage->setStyleSheet(getColor(conf["shader_drainage_color"]));
    ui->color_path->setStyleSheet(getColor(conf["color_gradient_path"]));
}


QString MainWindow::getColor(glm::vec3 value){
    std::stringstream color;
    color << "background-color:" << "rgb(" << value.x*255 << ", " << value.y*255 << "," << value.z*255 << ");";
    QString color_qstring =QString::fromUtf8(color.str().c_str());
    return color_qstring;
}
void MainWindow::changeColor(std::string name){
//    QPushButton *action = qobject_cast<QPushButton *>(sender());
//           if (action){
//                   QString data = action->QPushButton.);
//                   std::cout << data.toUtf8().constData() << std::endl;

//    }
    QColor color = QColorDialog::getColor(Qt::white);

    if (color.isValid()){
       std::cout << color.toRgb().red() << " " << color.toRgb().green() << " " << color.toRgb().blue() << std::endl;
       conf[name] = glm::vec3(color.toRgb().red()/255.0f, color.toRgb().green()/255.0f, color.toRgb().blue()/255.0f);
       runnel_controller.changeColors(name, conf[name] );
    }
    this->confColorRunnel();
}
void MainWindow::changeColor1(){
    std::string name = "shader_drainage_color_peucker";
    this->changeColor(name);
}
void MainWindow::changeColor2(){
    std::string name = "shader_edge_color_min";
    this->changeColor(name);
}
void MainWindow::changeColor3(){
    std::string name = "shader_edge_color_max";
    this->changeColor(name);
}
void MainWindow::changeColor4(){
    std::string name = "shader_point_color";
    this->changeColor(name);
}
void MainWindow::changeColor5(){
    std::string name = "shader_callaghan_color";
    this->changeColor(name);
}
void MainWindow::changeColor6(){
    std::string name = "shader_drainage_color";
    this->changeColor(name);
}
void MainWindow::changeColor7(){
    std::string name = "normal_color";
    this->changeColor(name);
}
void MainWindow::changeColor8(){
    std::string name = "color_gradient_path";
    this->changeColor(name);
}
void MainWindow::changeColor9(){
    std::string name = "shader_terrain_color";
    this->changeColor(name);
}
