# Builds all the projects in the solution...
.PHONY: all_projects
all_projects: WindowConnector Graphics TestProject 

# Builds project 'WindowConnector'...
.PHONY: WindowConnector
WindowConnector: 
	make --directory="Graphics/WindowConnectors/" --file=WindowConnector.makefile

# Builds project 'Graphics'...
.PHONY: Graphics
Graphics: 
	make --directory="Graphics/" --file=Graphics.makefile

# Builds project 'ModelTransformer'...
.PHONY: ModelTransformer
ModelTransformer: 
	make --directory="ModelTransformer/" --file=ModelTransformer.makefile

# Builds project 'TestProject'...
.PHONY: TestProject
TestProject: Graphics 
	make --directory="TestProject/" --file=TestProject.makefile

# Builds project 'SceneEditor'...
.PHONY: SceneEditor
SceneEditor: Graphics 
	make --directory="SceneEditor/" --file=SceneEditor.makefile

# Cleans all projects...
.PHONY: clean
clean:
	make --directory="Graphics/" --file=Graphics.makefile clean
	make --directory="TestProject/" --file=TestProject.makefile clean
	make --directory="SceneEditor/" --file=SceneEditor.makefile clean

