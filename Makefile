CXX := g++
CXXFLAGS := -std=c++20 -Wall -Iimgui/imgui -Iimgui/backends `pkg-config --cflags glfw3`

LIBS := -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo `pkg-config --libs glfw3`

SRCS := main.cpp \
        imgui/imgui.cpp \
        imgui/imgui_demo.cpp \
        imgui/imgui_draw.cpp \
        imgui/imgui_tables.cpp \
        imgui/imgui_widgets.cpp \
        imgui/backends/imgui_impl_glfw.cpp \
        imgui/backends/imgui_impl_opengl3.cpp \
		html_parser.cpp \
		parsing_utils.cpp \
		html_renderer.cpp \

OBJS := $(SRCS:.cpp=.o)

TARGET := app

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
