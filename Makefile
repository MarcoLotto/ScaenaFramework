# Builds all the projects in the solution...
.PHONY: all_projects
all_projects: WindowConnector Graphics ModelTransformer TestProject SceneEditor Demo1 

# Builds project 'WindowConnector'...
.PHONY: WindowConnector
WindowConnector: 
	make --directory="Graphics/WindowConnectors/" --file=WindowConnector.makefile

# Builds project 'Graphics'...
.PHONY: Graphics
Graphics: 
	make --directory="Graphics/" --file=Graphics.makefile

# Builds project 'WindowConnector'...
.PHONY: ModelTransformer
ModelTransformer: 
	make --directory="ModelTransformer/" --file=ModelTransformer.makefile

# Builds project 'TestProject'...
.PHONY: TestProject
TestProject: Graphics 
	make --directory="TestProject/" --file=TestProject.makefile
	
# Builds project 'TestProject'...
.PHONY: 22Onfield
TestProject: Graphics 
	make --directory="22Onfield/" --file=22Onfield.makefile

# Builds project 'SceneEditor'...
.PHONY: SceneEditor
SceneEditor: Graphics 
	make --directory="SceneEditor/" --file=SceneEditor.makefile

# Builds project 'Demo1'...
.PHONY: Demo1
Demo1: Graphics 
	make --directory="Demo1/" --file=Demo1.makefile

# Cleans all projects...
.PHONY: clean
clean:
	make --directory="Graphics/" --file=Graphics.makefile clean
	make --directory="TestProject/" --file=TestProject.makefile clean
	make --directory="SceneEditor/" --file=SceneEditor.makefile clean
	make --directory="Demo1/" --file=Demo1.makefile clean

