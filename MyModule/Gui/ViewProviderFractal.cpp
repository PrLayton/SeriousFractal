#include "PreCompiled.h"

#ifndef _PreComp_
# include <Inventor/SbVec3f.h>
# include <Inventor/nodes/SoSeparator.h>
# include <Inventor/nodes/SoCoordinate3.h>
# include <Inventor/nodes/SoDrawStyle.h>
# include <Inventor/nodes/SoFaceSet.h>
# include <Inventor/nodes/SoShapeHints.h>
# include <Inventor/nodes/SoImage.h>
# include <Inventor/nodes/SoTextureCoordinate2.h>
# include <Inventor/nodes/SoTexture2.h>
# include <QFile>
# include <QFileInfo>
# include <QImage>
# include <QString>
#endif

#include "ViewProviderFractal.h"

#include <Mod/MyModule/App/Fractal.h>
#include <App/Document.h>
#include <Gui/BitmapFactory.h>
#include <Base/FileInfo.h>
#include <Base/Stream.h>
#include <Base/Console.h>
#include <sstream>

using namespace Gui;
using namespace FractalGui;
using namespace Fract;


PROPERTY_SOURCE(FractalGui::ViewProviderFractal, Gui::ViewProviderGeometryObject)

ViewProviderFractal::ViewProviderFractal()
{
	texture = new SoTexture2;
	texture->ref();

	pcCoords = new SoCoordinate3();
	pcCoords->ref();
}

ViewProviderFractal::~ViewProviderFractal()
{
	pcCoords->unref();
	texture->unref();
}

void ViewProviderFractal::attach(App::DocumentObject *pcObj)
{
	ViewProviderDocumentObject::attach(pcObj);

	// NOTE: SoFCSelection node has beem removed because it led to
	// problems using the image as a construction plan with the
	// draft commands
	SoSeparator* planesep = new SoSeparator;
	planesep->addChild(pcCoords);

	SoTextureCoordinate2 *textCoord = new SoTextureCoordinate2;
	textCoord->point.set1Value(0, 0, 0);
	textCoord->point.set1Value(1, 1, 0);
	textCoord->point.set1Value(2, 1, 1);
	textCoord->point.set1Value(3, 0, 1);
	planesep->addChild(textCoord);

	// texture
	texture->model = SoTexture2::MODULATE;
	planesep->addChild(texture);

	// plane
	pcCoords->point.set1Value(0, 0, 0, 0);
	pcCoords->point.set1Value(1, 1, 0, 0);
	pcCoords->point.set1Value(2, 1, 1, 0);
	pcCoords->point.set1Value(3, 0, 1, 0);
	SoFaceSet *faceset = new SoFaceSet;
	faceset->numVertices.set1Value(0, 4);
	planesep->addChild(faceset);

	addDisplayMaskMode(planesep, "Fractal");
}

void ViewProviderFractal::setDisplayMode(const char* ModeName)
{
	if (strcmp("Fractal", ModeName) == 0)
		setDisplayMaskMode("Fractal");
	ViewProviderGeometryObject::setDisplayMode(ModeName);
}

std::vector<std::string> ViewProviderFractal::getDisplayModes(void) const
{
	std::vector<std::string> StrList;
	StrList.push_back("Fractal");
	return StrList;
}

bool ViewProviderFractal::loadSvg(const char* filename, float x, float y, QImage& img)
{
	QFileInfo fi(QString::fromUtf8(filename));
	if (fi.suffix().toLower() == QLatin1String("svg")) {
		QPixmap px = BitmapFactory().pixmapFromSvg(filename, QSize((int)x, (int)y));
		img = px.toImage();
		return true;
	}

	return false;
}

void ViewProviderFractal::updateData(const App::Property* prop)
{
	/*Fract::Fractal* pcPlaneObj = static_cast<Fract::Fractal*>(pcObject);
	if (prop == &pcPlaneObj->XSize || prop == &pcPlaneObj->YSize) {
		float x = pcPlaneObj->XSize.getValue();
		float y = pcPlaneObj->YSize.getValue();

		//pcCoords->point.setNum(4);
		pcCoords->point.set1Value(0, -(x / 2), -(y / 2), 0.0);
		pcCoords->point.set1Value(1, +(x / 2), -(y / 2), 0.0);
		pcCoords->point.set1Value(2, +(x / 2), +(y / 2), 0.0);
		pcCoords->point.set1Value(3, -(x / 2), +(y / 2), 0.0);

		QImage impQ;
		loadSvg(pcPlaneObj->ImageFile.getValue(), x, y, impQ);
		if (!impQ.isNull()) {
			SoSFImage img;
			// convert to Coin bitmap
			BitmapFactory().convert(impQ, img);
			texture->image = img;
		}
	}
	else if (prop == &pcPlaneObj->ImageFile) {
		float x = pcPlaneObj->XSize.getValue();
		float y = pcPlaneObj->YSize.getValue();
		QImage impQ;
		if (!loadSvg(pcPlaneObj->ImageFile.getValue(), x, y, impQ))
			impQ.load(QString::fromUtf8(pcPlaneObj->ImageFile.getValue()));
		if (!impQ.isNull()) {
			SoSFImage img;
			// convert to Coin bitmap
			BitmapFactory().convert(impQ, img);
			texture->image = img;
		}
	}
	else {
		Gui::ViewProviderGeometryObject::updateData(prop);
	}*/
}
