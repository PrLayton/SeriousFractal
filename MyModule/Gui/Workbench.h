/***************************************************************************
 *   Copyright (c) YEAR YOUR NAME         <Your e-mail address>            *
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


#ifndef MyModule_WORKBENCH_H
#define MyModule_WORKBENCH_H

#include <Gui/Workbench.h>

namespace MyModuleGui {

class Workbench : public Gui::StdWorkbench
{
    TYPESYSTEM_HEADER();

public:
    Workbench();
    virtual ~Workbench();

protected:
    Gui::MenuItem* setupMenuBar() const;
    Gui::ToolBarItem* setupToolBars() const;
};

} // namespace MyModuleGui


#endif // MyModule_WORKBENCH_H 