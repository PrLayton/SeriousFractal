#pragma once
#include <App/GeoFeature.h>
#include <App/PropertyFile.h>
#include <App/PropertyUnits.h>

namespace Fract
{
	class MyModuleAppExport Fractal : public App::GeoFeature
	{
		PROPERTY_HEADER(Fract::Fractal);

	public:
		/// Constructor
		Fractal(void);
		virtual ~Fractal();

		App::PropertyString       Name;
		App::PropertyInteger      Type;
		App::PropertyInteger      Iteration;
		App::PropertyLength       Length;
		App::PropertyLength       Width;
		App::PropertyLength       Height;
		App::PropertyVector       CenterPosition;
		App::PropertyAngle        Orientation;
		App::PropertyColor        Color;

		/// returns the type name of the ViewProvider
		virtual const char* getViewProviderName(void) const {
			return "FractalGui::ViewProviderFractal";
			}
	};
}

