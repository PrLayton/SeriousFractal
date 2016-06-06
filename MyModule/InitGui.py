# MyModule gui init module
# (c) 2001 Juergen Riegel LGPL

class MyModuleWorkbench ( Workbench ):
	"MyModule workbench object"
	MenuText = "MyModule"
	ToolTip = "MyModule workbench"
	def Initialize(self):
		# load the module
		import MyModuleGui
	def GetClassName(self):
		return "MyModuleGui::Workbench"

Gui.addWorkbench(MyModuleWorkbench())
