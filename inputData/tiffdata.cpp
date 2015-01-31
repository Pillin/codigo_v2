#include "tiffdata.h"
#include <iostream>
#include <QImage>
#define	N(a)	(sizeof (a) / sizeof (a[0]))
TiffData::TiffData()
{

}

bool TiffData::openFile(QString name){
	image = QImage(name);

	if (!image.isNull()){
		return true;
	}
	std::cout << "Failed to open image " << name.toStdString() << std::endl;
	return false;

}

void TiffData::getDataTerrain(Terrain* ter){
	int width = image.width()+1;
	int height = image.height();
	ter->width = width;
	ter->height = height;
	const uchar* raster = image.constBits ();
	ter->setMapPixel(1,1);
	if (raster != NULL) {
		if (raster) {
			int counter = 0;
			int muestras = 500;
			ter->width = muestras-1;
			ter->height = muestras-1;
			for (int h = 0; h < muestras-1; h++){
				float final_h = h*(1.0f*height/muestras);
				float h_floor = std::floor(final_h);
				for (int w = 0; w < muestras-1; w++){
					float final_w = w*(1.0f*width/muestras);
					float w_floor = std::floor(final_w);
					float w_ceil = w_floor+1;
					float h_ceil = h_floor+1;

					float z11 = 1.0*(raster[(int)(w_floor +width*h_floor)]);
					float z12 = 1.0*(raster[(int)(w_ceil +width*h_floor)]);
					float z21 = 1.0*(raster[(int)(w_floor +width*h_ceil)]);
					float z22 = 1.0*(raster[(int)(w_ceil +width*h_ceil)]);
					float zw1 = glm::mix(z11, z12, final_w-w_floor);
					float zw2 = glm::mix(z21, z22, final_w-w_floor);
					float z = glm::mix(zw1, zw2, final_h - h_floor);
					glm::vec3 coords = glm::vec3(final_w,final_h,z);
					ter->setBoundingBox(coords);
					runnel::Point *point_new = new runnel::Point(coords, counter);
					ter->addPoint(point_new);
					++counter;
				}

			}
		}else{
			std::cout << "Error abriendo imagen :(" << std::endl;
		}
	}
	image = QImage();
}
