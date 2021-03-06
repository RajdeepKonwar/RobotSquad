CXX=g++
CXXFLAGS=-std=c++11 -O3 -Wall
LDFLAGS=-lGL -lGLU -lGLEW -lglfw3 -L/usr/local/lib -lXi -lGLEW -lGLU -lm -lGL -lm -lpthread -pthread -ldl -ldrm -lXdamage -lX11-xcb -lxcb-glx -lxcb-dri2 -lglfw -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp

OBJECTS=main.o Curve.o SceneGraph.o Shader.o SkyBox.o Window.o

main: $(OBJECTS)
		$(CXX) $(CXXFLAGS) $(OBJECTS) -o main $(LDFLAGS)

main.o: main.cpp

Curve.o: Curve.cpp

SceneGraph.o: SceneGraph.cpp

Shader.o: Shader.cpp

SkyBox.o: SkyBox.cpp

Window.o: Window.cpp

.PHONY: clean
clean:
		rm -f *.o main
