#ifndef IMAGE_ViewProviderFractal_H
#define IMAGE_ViewProviderFractal_H

#include <Gui/ViewProviderGeometryObject.h>
#include <Gui/SoFCSelection.h>

class SoCoordinate3;
class SoDrawStyle;
class SoTexture2;
class QImage;

namespace FractalGui
{

	class MyModuleGuiExport ViewProviderFractal : public Gui::ViewProviderGeometryObject
	{
		PROPERTY_HEADER(RobotGui::ViewProviderFractal);

	public:
		/// constructor.
		ViewProviderFractal();

		/// destructor.
		~ViewProviderFractal();

		void attach(App::DocumentObject *pcObject);
		void setDisplayMode(const char* ModeName);
		std::vector<std::string> getDisplayModes() const;
		void updateData(const App::Property*);

	private:
		bool loadSvg(const char*, float x, float y, QImage& img);

	protected:
		SoCoordinate3         * pcCoords;
		SoTexture2            * texture;
	};

} 


#endif
