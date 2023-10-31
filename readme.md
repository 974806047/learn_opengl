### 实验环境
* 系统： window10
* 编辑器： vscode
* 编译器： [mingw 13.1.0](https://github.com/niXman/mingw-builds-binaries/releases)
* 构建：[cmake 3.27.3](https://cmake.org/download/) 
* [glm 0.9.9.8](http://glm.g-truc.net/0.9.5/index.html)
* [glad](https://glad.dav1d.de/)
* [glfw](https://www.glfw.org/download.html)
* [glew 2.1.0](https://glew.sourceforge.net/index.html)
* [stb_image 2.28](https://github.com/nothings/stb/blob/master/stb_image.h)
* [assimp](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
  * [assimp编译出现问题，可以参照这个](https://blog.csdn.net/zionFisher/article/details/120337198)

### 开始
* 安装cmake和mingw
* `mkdir build`
* `cd build`
* `cmake -G "MinGW Makefiles" ..`
* `cmake --build .`
* 进入build目录对应目录下查看并运行exe


### 效果
[第一章](doc/1.md)
[第二章](doc/2.md)
[第三章](doc/3.md)