# HelloCV：TrafficLight_Detect - 基于OpenCV的红绿灯识别

基于C++和OpenCV，用CMake构建的红绿灯识别工具。

## 更新内容

### 10.26日更新
1. 去除运行程序时窗口实时显示（imshow）
2. 增加运行程序时参数，制定文件进行操作
3. 增加程序运行提示、运行结束提示

### 10.25日更新
1. 修改部分错别字
2. 修改程序逻辑，将计算最小外界圆放置在判断是否为圆形之后
3. 完善 `README.md` 

## 项目结构

```
HelloCV/
├── CMakeLists.txt          # CMake 配置文件
├── main.cpp                # 主程序源代码
├── links.md                # 语雀笔记链接汇总文件
├── README.md               # 项目说明文档（本文件）
└── build/                  # CMake 构建目录（生成）
```

## 实现方法

使用OpenCV中相关算法进行 **颜色检测** 和 **图形检测** ，对检测到的相关信息进行输出。

### 颜色检测

将Mat对象转化为HSV色彩空间，再进行红色和绿色的提取。

相关参数如下：

```c++
    // 定义红色的HSV范围（两个区间）
    Scalar lRed1(0, 100, 100);   // 范围1的下界
    Scalar uRed1(10, 255, 255);  // 范围1的上界
    Scalar lRed2(156, 100, 100); // 范围2的下界
    Scalar uRed2(180, 255, 255); // 范围2的上界
    // 定义绿色的HSV范围
    Scalar lGreen(34, 50, 87);
    Scalar uGreen(104, 255, 255);
```

### 形状检测

通过轮廓+最小外接圆+圆形性过滤实现圆形检测。

具体步骤：

1. 使用 `findContours` 进行轮廓检测。  
2. 计算所得轮廓面积，筛选小于一定值的轮廓，去除Voice。
3. 计算圆形度，判断是否为真正的圆形。
4. 对合适的圆形进行求最小外接圆操作，并将圆形标在img中。

### 视频保存
使用 `VideoWriter` 对象进行视频保存。

## 环境配置

### 必需工具安装

1. **安装 C++ 编译器**


2. **安装 CMake（版本 3.28 或更高）**


### 验证安装
```bash
cmake --version
g++ --version
```

## 构建项目

### 1. 克隆项目
```bash
git clone https://github.com/ZHFlame/HelloCV.git
cd HelloCV
```

### 2. 使用 CMake 构建
```bash
# 创建并进入 build 目录（CMake 推荐使用外部构建）
mkdir build
cd build

# 生成构建文件
cmake ..

# 编译项目（生成 TrafficLight_Detect 可执行文件）
make
```

### 构建选项
- 默认生成的可执行文件名为 `TrafficLight_Detect`
- 可执行文件输出到 `build` 目录中
- 如需更改安装路径，可使用 `-DCMAKE_INSTALL_PREFIX` 参数

## 使用方法

### 运行程序
1. **[方式1]** 默认将同目录下 `TrafficLight.mp4` 作为操作对象：
将待检测视频更名为 `TrafficLight.mp4` 放到执行目录（build目录）中。
在 build 目录中执行：
```bash
./TrafficLight_Detect
```
最终输出为同目录下的 `result.avi` 文件

2. **[方式2]** 将待操作视频放到执行目录（build目录）中。
   在 build 目录中执行：
```bash
./TrafficLight_Detect [待处理文件名]
```
最终输出为同目录下的 `result.avi` 文件


## 故障排除

### 常见问题

1. **CMake 报错版本过低**
   ```
   CMake 3.28 or higher is required
   ```
   解决方案：升级 CMake 到 3.28 或更高版本

2. **编译错误**
    - 确保已安装完整的 C++ 编译环境
    - 检查 CMakeLists.txt 文件路径是否正确

3. **文件找不到错误**
    - 确保输入的文件路径正确
    - 使用相对路径时，路径相对于程序运行目录

4. **输出文件内容不全**
   - 确保没有提前关闭程序

5. **显示“提供过多参数”**
   - 确保运行程序时正确提供参数！
```bash
./TrafficLight_Detect # 默认处理当前目录下“TrafficLight.mp4”文件
```
```bash
./TrafficLight_Detect [待处理文件名] # 处理指定文件
```

## 许可证

本项目采用 Apache-2.0 许可证。

## 贡献

欢迎提交 Issue 和 Pull Request 来改进本项目。

---