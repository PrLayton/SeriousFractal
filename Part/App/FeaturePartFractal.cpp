/***************************************************************************
*   Copyright (c) J�rgen Riegel          (juergen.riegel@web.de) 2002     *
*                                                                         *
*   This file is part of the FreeCAD CAx development system.              *
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Library General Public           *
*   License as published by the Free Software Foundation; either          *
*   version 2 of the License, or (at your option) any later version.      *
*                                                                         *
*   This library  is distributed in the hope that it will be useful,      *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU Library General Public License for more details.                  *
*                                                                         *
*   You should have received a copy of the GNU Library General Public     *
*   License along with this library; see the file COPYING.LIB. If not,    *
*   write to the Free Software Foundation, Inc., 59 Temple Place,         *
*   Suite 330, Boston, MA  02111-1307, USA                                *
*                                                                         *
***************************************************************************/


#include "PreCompiled.h"
#ifndef _PreComp_
# include <BRepPrimAPI_MakeBox.hxx>
# include <BRepBuilderAPI_MakeFace.hxx>
# include <Precision.hxx>
#endif


#include <Base/Console.h>
#include <Base/Reader.h>
#include "FeaturePartFractal.h"
#include <Base/Exception.h>


# include <BRepBuilderAPI_MakePolygon.hxx>
# include <BRepBuilderAPI_MakeFace.hxx>
# include <BRepBuilderAPI_MakeShell.hxx>
# include <BRepBuilderAPI_MakeSolid.hxx>
# include <BRepOffsetAPI_MakeOffsetShape.hxx>
# include <Geom_Surface.hxx>
# include <TopoDS_Face.hxx>
#include "TopoShape.h"
//#include "BRepPrimAPI_MakeCylinder.hxx"
#include "BRepAlgoAPI_Cut.hxx"
#include "STEPControl_Writer.hxx"
#include "BRepGProp.hxx"
#include "GProp_GProps.hxx"
#include "Standard_Macro.hxx"


using namespace Part;


PROPERTY_SOURCE(Part::Fractal, Part::Primitive)


Fractal::Fractal()
{
	ADD_PROPERTY_TYPE(Length, (10.0f), "Fractal", App::Prop_None, "The length of the Fractal");
	ADD_PROPERTY_TYPE(Width, (10.0f), "Fractal", App::Prop_None, "The width of the Fractal");
	ADD_PROPERTY_TYPE(Height, (10.0f), "Fractal", App::Prop_None, "The height of the Fractal");
	ADD_PROPERTY_TYPE(Depth, (1), "Fractal", App::Prop_None, "The depth of the Fractal");
}

short Fractal::mustExecute() const
{
	if (Length.isTouched() ||
		Height.isTouched() ||
		Width.isTouched())
		return 1;
	return Primitive::mustExecute();
}

//Tableaux pour les vecteurs sommets et les vecteurs des faces
std::vector<Base::Vector3d>  sommets;
//std::vector<Base::Vector3d>  faces;
//Tableau pour les faces creees avec Part
std::vector<Base::Vector3d>  partFaces;

//Fonction de calcul des coordoneees des milieux
Base::Vector3d  milieu(Base::Vector3d m, Base::Vector3d n){
	double x = (m[0] + n[0]) / 2.0;
	double y = (m[1] + n[1]) / 2.0;
	double z = (m[2] + n[2]) / 2.0;
		return Base::Vector3d(x, y, z);
}


std::vector<Base::Vector3d> _nodes;
TopoDS_Wire wire;
std::vector<TopoDS_Face> faces;

//Generation de la fractal "Triangle de Sierpinski
void sierpin(Base::Vector3d a, Base::Vector3d b, Base::Vector3d c, Base::Vector3d d, int n, std::vector<Base::Vector3d> nodes){
//Tant que l'iteration n'est pas arrivee au niveau final
if (n > 0){
	sierpin(a, milieu(a, b), milieu(a, c), milieu(a, d), n - 1, nodes);
	sierpin(milieu(a, b), b, milieu(b, c), milieu(b, d), n - 1, nodes);
	sierpin(milieu(a, c), milieu(b, c), c, milieu(c, d), n - 1, nodes);
	sierpin(milieu(a, d), milieu(b, d), milieu(c, d), d, n - 1, nodes);
}
else{
	//On ajoute les sommets et les faces dans les tableaux
	_nodes.push_back(a);
	_nodes.push_back(b);
	_nodes.push_back(c);
	_nodes.push_back(d);
	//On prend le nombre de sommets du tableau
	int ns = _nodes.size();
	/*partFaces.push_back(Part.Face(Part.makePolygon(Base::Vector3d(sommets[ns - 4], sommets[ns - 3], sommets[ns - 2]))));
	partFaces.push_back(Part.Face(Part.makePolygon(Base::Vector3d(sommets[ns - 4], sommets[ns - 2], sommets[ns - 1]))));
	partFaces.push_back(Part.Face(Part.makePolygon(Base::Vector3d(sommets[ns - 4], sommets[ns - 1], sommets[ns - 3]))));
	partFaces.push_back(Part.Face(Part.makePolygon(Base::Vector3d(sommets[ns - 3], sommets[ns - 2], sommets[ns - 1]))));*/
	//wire
	faces.push_back(BRepBuilderAPI_MakeFace(BRepBuilderAPI_MakePolygon(gp_Pnt(_nodes[ns - 4].x, _nodes[ns - 4].y, _nodes[ns - 4].z), gp_Pnt(_nodes[ns - 3].x, _nodes[ns - 3].y, _nodes[ns - 3].z)
		, gp_Pnt(_nodes[ns - 2].x, _nodes[ns - 2].y, _nodes[ns - 2].z), gp_Pnt(_nodes[ns - 4].x, _nodes[ns - 4].y, _nodes[ns - 4].z), Standard_True), Standard_True));
	faces.push_back(BRepBuilderAPI_MakeFace(BRepBuilderAPI_MakePolygon(gp_Pnt(_nodes[ns - 4].x, _nodes[ns - 4].y, _nodes[ns - 4].z), gp_Pnt(_nodes[ns - 2].x, _nodes[ns - 2].y, _nodes[ns - 2].z)
		, gp_Pnt(_nodes[ns - 1].x, _nodes[ns - 1].y, _nodes[ns - 1].z), gp_Pnt(_nodes[ns - 4].x, _nodes[ns - 4].y, _nodes[ns - 4].z), Standard_True), Standard_True));
	faces.push_back(BRepBuilderAPI_MakeFace(BRepBuilderAPI_MakePolygon(gp_Pnt(_nodes[ns - 4].x, _nodes[ns - 4].y, _nodes[ns - 4].z), gp_Pnt(_nodes[ns - 1].x, _nodes[ns - 1].y, _nodes[ns - 1].z)
		, gp_Pnt(_nodes[ns - 3].x, _nodes[ns - 3].y, _nodes[ns - 3].z), gp_Pnt(_nodes[ns - 4].x, _nodes[ns - 4].y, _nodes[ns - 4].z), Standard_True), Standard_True));
	faces.push_back(BRepBuilderAPI_MakeFace(BRepBuilderAPI_MakePolygon(gp_Pnt(_nodes[ns - 4].x, _nodes[ns - 4].y, _nodes[ns - 4].z), gp_Pnt(_nodes[ns - 2].x, _nodes[ns - 2].y, _nodes[ns - 2].z)
		, gp_Pnt(_nodes[ns - 1].x, _nodes[ns - 1].y, _nodes[ns - 1].z), gp_Pnt(_nodes[ns - 4].x, _nodes[ns - 4].y, _nodes[ns - 4].z), Standard_True), Standard_True));

	/*nodes.push_back(Base::Vector3d(sommets[ns - 4], sommets[ns - 3], sommets[ns - 2]);
	nodes.push_back(Base::Vector3d(sommets[ns - 4], sommets[ns - 2], sommets[ns - 1]);
	nodes.push_back(Base::Vector3d(sommets[ns - 4], sommets[ns - 1], sommets[ns - 3]);
	nodes.push_back((Base::Vector3d(sommets[ns - 3], sommets[ns - 2], sommets[ns - 1]);*/

	//BRepBuilderAPI_MakeFace mkFace(sommets[ns - 4], sommets[ns - 3], sommets[ns - 2], sommets[ns - 4]);
}
}

int level =3;
int width = 3 * level;

float step = 2.0 / width;

float px = -1.0;
float py = -1.0;
float pz = 0;

std::vector<Base::Vector3d>  coords;
//std::vector<Base::Vector3d>  faces;

int idx = 0;

bool t = true;
bool tx = true;
bool ty = true;
bool tz = true;
float w = 0;

void Sponge(){
	for (int x = 0; x < width; x++){
		py = -1.0;
		for (int y = 0; y < width; y++){
			pz = 0.0;
			for (int z = 0; z < width; z++){
				t = true;
				w = width / 3;
				while (w >= 1){
					tx = ((int(x / w) % 3) != 1);
					ty = ((int(y / w) % 3) != 1);
					tz = ((int(z / w) % 3) != 1);
					t = t && ((tx && ty) || (tx && tz) || (ty && tz));
					w = w / 3;
				}
#
				if (t == true){

					coords.push_back(Base::Vector3d(px * 5, py * 5, pz * 5));
					coords.push_back(Base::Vector3d(px * 5 + step * 5, py * 5, pz * 5));
					coords.push_back(Base::Vector3d(px * 5 + step * 5, py * 5 + step * 5, pz * 5));
					coords.push_back(Base::Vector3d(px * 5, py * 5 + step * 5, pz * 5));

					coords.push_back(Base::Vector3d(px, py, pz + step));
					coords.push_back(Base::Vector3d(px + step, py, pz + step));
					coords.push_back(Base::Vector3d(px + step, py + step, pz + step));
					coords.push_back(Base::Vector3d(px, py + step, pz + step));

					faces.push_back(BRepBuilderAPI_MakeFace(BRepBuilderAPI_MakePolygon(gp_Pnt(coords[idx].x, coords[idx].y, coords[idx].z), gp_Pnt(coords[idx + 1].x, coords[idx + 1].y, coords[idx + 1].z)
						, gp_Pnt(coords[idx + 2].x, coords[idx + 2].y, coords[idx + 2].z), gp_Pnt(coords[idx + 3].x, coords[idx + 3].y, coords[idx + 3].z), Standard_True), Standard_True));
					faces.push_back(BRepBuilderAPI_MakeFace(BRepBuilderAPI_MakePolygon(gp_Pnt(coords[idx].x, coords[idx].y, coords[idx].z), gp_Pnt(coords[idx + 1].x, coords[idx + 1].y, coords[idx + 1].z)
						, gp_Pnt(coords[idx + 5].x, coords[idx + 5].y, coords[idx + 5].z), gp_Pnt(coords[idx + 4].x, coords[idx + 4].y, coords[idx + 4].z), Standard_True), Standard_True));
					faces.push_back(BRepBuilderAPI_MakeFace(BRepBuilderAPI_MakePolygon(gp_Pnt(coords[idx + 1].x, coords[idx + 1].y, coords[idx + 1].z), gp_Pnt(coords[idx + 2].x, coords[idx + 2].y, coords[idx + 2].z)
						, gp_Pnt(coords[idx + 6].x, coords[idx + 6].y, coords[idx + 6].z), gp_Pnt(coords[idx + 5].x, coords[idx + 5].y, coords[idx + 5].z), Standard_True), Standard_True));
					faces.push_back(BRepBuilderAPI_MakeFace(BRepBuilderAPI_MakePolygon(gp_Pnt(coords[idx + 1].x, coords[idx + 1].y, coords[idx + 1].z), gp_Pnt(coords[idx + 2].x, coords[idx + 2].y, coords[idx + 2].z)
						, gp_Pnt(coords[idx + 6].x, coords[idx + 6].y, coords[idx + 6].z), gp_Pnt(coords[idx + 5].x, coords[idx + 5].y, coords[idx + 5].z), Standard_True), Standard_True));
					//faces.append((idx, idx + 1, idx + 2, idx + 3))
					//faces.append((idx, idx + 1, idx + 5, idx + 4))
					//faces.append((idx + 1, idx + 2, idx + 6, idx + 5))
					//faces.append((idx + 2, idx + 3, idx + 7, idx + 6))
					//faces.append((idx + 3, idx + 0, idx + 4, idx + 7))
					//faces.append((idx + 4, idx + 5, idx + 6, idx + 7))
					idx += 8;
				}
				pz += step;
			}
			py += step;
		}
		px += step;
	}
	width = 3 * level;
	step = 2.0 / width;

}

App::DocumentObjectExecReturn *Fractal::execute(void)
{
	faces.clear();
	//Appel de la fonction de generation avec des valeurs prereglees(taille) et la valeur de l'utilisateur
	//sierpin([-7, -4, 0], [0, 8, 0], [7, -4, 0], [0, 0, 11], 6)
	//sierpin(Base::Vector3d(-5, -5, -5), Base::Vector3d(5, 5, -5), Base::Vector3d(-5, 5, 5), Base::Vector3d(5, -5, 5), 1, Nodes.getValues());
	
	sierpin(Base::Vector3d(-Length.getValue(), -Width.getValue(), -Height.getValue()), Base::Vector3d(Length.getValue(), Width.getValue(), -Height.getValue()), Base::Vector3d(-Length.getValue(), Width.getValue(), Height.getValue()), Base::Vector3d(Length.getValue(), -Width.getValue(), Height.getValue()), Depth.getValue(), Nodes.getValues());
	//Sponge();

		/*//Creation du shell avec la liste des faces
		myShell = Part.makeShell(partFaces);
		// Creation du polygon avec le shell
		mySolid = Part.makeSolid(myShell);
		// Mise a jour de l'orientation des faces
		mySolidRev = mySolid.copy();
		mySolidRev.reverse();

		//Affichage du solide
		Part.show(mySolidRev);*/

	BRepBuilderAPI_MakePolygon poly;
	//const std::vector<Base::Vector3d> nodes = Nodes;
	/*_nodes.push_back(Base::Vector3d(0, 0, 0));
	_nodes.push_back(Base::Vector3d(0, 0, 1));
	_nodes.push_back(Base::Vector3d(0, 0, 3));
	_nodes.push_back(Base::Vector3d(0, 0, 5));
	_nodes.push_back(Base::Vector3d(0, 5, 0));
	_nodes.push_back(Base::Vector3d(0, 0, 8));*/

	/*std::string s = std::to_string(_nodes.size());
	char const *pchar = s.c_str();
	Base::Console().Message(pchar);
	Base::Console().Log(pchar);*/

	for (std::vector<Base::Vector3d>::const_iterator it = _nodes.begin(); it != _nodes.end(); ++it) {
		gp_Pnt pnt(it->x, it->y, it->z);
		poly.Add(pnt);
	}

	if (!poly.IsDone())
		throw Base::Exception("Cannot create polygon because less than two vetices are given");
	//TopoDS_Wire wire = poly.Wire();
	//BRepBuilderAPI_MakeFace makeFace(poly.Wire());

	/*gp_Pnt point1 = gp_Pnt(1,0,0);
	gp_Pnt point2 = gp_Pnt(1,1,0);
	gp_Pnt point3 = gp_Pnt(1,0,1);*/
	//TopoDS_Wire wire = BRepBuilderAPI_MakePolygon(, Standard_True);
	//TopoDS_Face face = BRepBuilderAPI_MakeFace(wire, Standard_True);
	//TopoDS_Shape s;

	/*if (makeFace.Error() == BRepBuilderAPI_FaceDone)
	{
		TopoDS_Face faceCurrent = makeFace.Face();
	}
	BRepBuilderAPI_MakeFace makeFace(faceCurrent);
	makeFace.Add(MP.Wire());
	if (makeFace.Error() == BRepBuilderAPI_FaceDone)
	{
		faceCurrent = makeFace.Face();
	}*/

	//this->Shape.setValue();



	//TopoDS_Face tf;

	//BRepBuilderAPI_MakeFace mkFace(TopoDS::Wire(sh));

	//Handle_Geom_Surface gs;
	//Handle<Geom_Surface> dd;
	//gs.Access = 0;

	/*RepOffsetAPI_MakeOffsetShape mkOffset(this->_Shape, offset, tol, BRepOffset_Mode(offsetMode),
        intersection ? Standard_True : Standard_False,
        selfInter ? Standard_True : Standard_False,
        GeomAbs_JoinType(join));
   
    if (!fill)
        return res;
#if 1
    //s=Part.makePlane(10,10)
    //s.makeOffsetShape(1.0,0.01,False,False,0,0,True)*/

	//BRepOffsetAPI_MakeOffsetShape myOffsetShape;
	//TopoShape ts;
	//myOffsetShape = ts.makeOffsetShape(1.0, 0.01, False, False, 0, 0, True);
	//const TopoDS_Shape& res = myOffsetShape.Shape();

	BRep_Builder builder;
	TopoDS_Shell shell;
	builder.MakeShell(shell);
	/*s = std::to_string(faces.size());
	pchar = s.c_str();
	Base::Console().Message(pchar);
	Base::Console().Log(pchar);*/
	for (int i = 0; i < faces.size(); i++) {
		builder.Add(shell, faces[i]);
		//this->Shape.setValue(shell);
	}
	//this->Shape.setValue(builder.sh);

	//BRepBuilderAPI_MakeShell myShell(gs);
	//const TopoDS_Shell& td = gs.sh;

	/*TopoDS_Solid mysolid;
	builder.MakeSolid(mysolid);
	builder.Add(mysolid, shell);*/

	BRepBuilderAPI_MakeSolid mySolid(shell);
	this->Shape.setValue(mySolid.Shape());


	return App::DocumentObject::StdReturn;
	}

/**
* This method was added for backward-compatibility. In former versions
* of Box we had the properties x,y,z and l,h,w which have changed to
* Location -- as replacement for x,y and z and Length, Height and Width.
*/
void Fractal::Restore(Base::XMLReader &reader)
{
	reader.readElement("Properties");
	int Cnt = reader.getAttributeAsInteger("Count");

	bool location_xyz = false;
	bool location_axis = false;
	bool distance_lhw = false;
	Base::Placement plm;
	App::PropertyDistance x, y, z;
	App::PropertyDistance l, w, h;
	App::PropertyVector Axis, Location;
	Axis.setValue(0.0f, 0.0f, 1.0f);
	for (int i = 0; i<Cnt; i++) {
		reader.readElement("Property");
		const char* PropName = reader.getAttribute("name");
		const char* TypeName = reader.getAttribute("type");
		App::Property* prop = getPropertyByName(PropName);
		if (!prop) {
			// in case this comes from an old document we must use the new properties
			if (strcmp(PropName, "l") == 0) {
				distance_lhw = true;
				prop = &l;
			}
			else if (strcmp(PropName, "w") == 0) {
				distance_lhw = true;
				prop = &h; // by mistake w was considered as height
			}
			else if (strcmp(PropName, "h") == 0) {
				distance_lhw = true;
				prop = &w; // by mistake h was considered as width
			}
			else if (strcmp(PropName, "x") == 0) {
				location_xyz = true;
				prop = &x;
			}
			else if (strcmp(PropName, "y") == 0) {
				location_xyz = true;
				prop = &y;
			}
			else if (strcmp(PropName, "z") == 0) {
				location_xyz = true;
				prop = &z;
			}
			else if (strcmp(PropName, "Axis") == 0) {
				location_axis = true;
				prop = &Axis;
			}
			else if (strcmp(PropName, "Location") == 0) {
				location_axis = true;
				prop = &Location;
			}
		}
		else if (strcmp(PropName, "Length") == 0 && strcmp(TypeName, "PropertyDistance") == 0) {
			distance_lhw = true;
			prop = &l;
		}
		else if (strcmp(PropName, "Height") == 0 && strcmp(TypeName, "PropertyDistance") == 0) {
			distance_lhw = true;
			prop = &h;
		}
		else if (strcmp(PropName, "Width") == 0 && strcmp(TypeName, "PropertyDistance") == 0) {
			distance_lhw = true;
			prop = &w;
		}

		// NOTE: We must also check the type of the current property because a subclass
		// of PropertyContainer might change the type of a property but not its name.
		// In this case we would force to read-in a wrong property type and the behaviour
		// would be undefined.
		std::string tn = TypeName;
		if (strcmp(TypeName, "PropertyDistance") == 0) // missing prefix App::
			tn = std::string("App::") + tn;
		if (prop && strcmp(prop->getTypeId().getName(), tn.c_str()) == 0)
			prop->Restore(reader);

		reader.readEndElement("Property");
	}

	if (distance_lhw) {
		this->Length.setValue(l.getValue());
		this->Height.setValue(h.getValue());
		this->Width.setValue(w.getValue());
	}

	// for 0.7 releases or earlier
	if (location_xyz) {
		plm.setPosition(Base::Vector3d(x.getValue(), y.getValue(), z.getValue()));
		this->Placement.setValue(this->Placement.getValue() * plm);
		this->Shape.StatusBits.set(10); // override the shape's location later on
	}
	// for 0.8 releases
	else if (location_axis) {
		Base::Vector3d d = Axis.getValue();
		Base::Vector3d p = Location.getValue();
		Base::Rotation rot(Base::Vector3d(0.0, 0.0, 1.0),
			Base::Vector3d(d.x, d.y, d.z));
		plm.setRotation(rot);
		plm.setPosition(Base::Vector3d(p.x, p.y, p.z));
		this->Placement.setValue(this->Placement.getValue() * plm);
		this->Shape.StatusBits.set(10); // override the shape's location later on
	}

	reader.readEndElement("Properties");
}

void Fractal::onChanged(const App::Property* prop)
{
	if (prop == &Length || prop == &Width || prop == &Height) {
		if (!isRestoring()) {
			App::DocumentObjectExecReturn *ret = recompute();
			delete ret;
		}
	}
	else if (prop == &this->Shape) {
		// see Box::Restore
		if (this->Shape.StatusBits.test(10)) {
			this->Shape.StatusBits.reset(10);
			App::DocumentObjectExecReturn *ret = recompute();
			delete ret;
			return;
		}
	}
	Part::Primitive::onChanged(prop);
}
