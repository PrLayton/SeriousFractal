
#include "PreCompiled.h"

#ifndef _PreComp_
#endif

#include <App/DocumentObjectPy.h>
#include <Base/Placement.h>

#include "Fractal.h"

using namespace App;
using namespace Fract;

PROPERTY_SOURCE(Fract::Fractal, App::GeoFeature)

Fractal::Fractal()
{
	ADD_PROPERTY_TYPE(Name, ("MengerSponge"), "Fractal", Prop_None, "Nom de la fractale");
	ADD_PROPERTY_TYPE(Type, (0), "Fractal", Prop_None, "Categorie");
	ADD_PROPERTY_TYPE(Iteration, (3), "Fractal", Prop_None, "Nombre d'iterations");
	ADD_PROPERTY_TYPE(Length, (5), "Fractal", Prop_None, "Profondeur");
	ADD_PROPERTY_TYPE(Width, (5), "Fractal", Prop_None, "Largeur");
	ADD_PROPERTY_TYPE(Height, (5), "Fractal", Prop_None, "Hauteur");
	ADD_PROPERTY_TYPE(CenterPosition, (0,0,0), "Fractal", Prop_None, "<Position du centre de la figure");
	ADD_PROPERTY_TYPE(Orientation, (0), "Fractal", Prop_None, "Orientation dans l'espace");
	ADD_PROPERTY_TYPE(Color, (0,0,0), "Fractal", Prop_None, "Couleur unique");

	//Name.setStatus(App::Property::ReadOnly, true);
}


Fractal::~Fractal()
{
}