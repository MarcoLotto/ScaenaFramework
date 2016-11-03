# Builds all the projects in the solution...
.PHONY: all_projects
all_projects: WindowConnector Graphics ExampleProject ClothDemo

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

# Builds project 'ExampleProject'...
.PHONY: ExampleProject
ExampleProject: Graphics 
	make --directory="ExampleProject/" --file=ExampleProject.makefile

# Builds project 'ClothDemo'...
.PHONY: ClothDemo
ClothDemo: Graphics 
	make --directory="ClothDemo/" --file=ClothDemo.makefile

# Cleans all projects...
.PHONY: clean
clean:
	make --directory="Graphics/" --file=Graphics.makefile clean
	make --directory="ExampleProject/" --file=ExampleProject.makefile clean
	make --directory="ClothDemo/" --file=ClothDemo.makefile clean

