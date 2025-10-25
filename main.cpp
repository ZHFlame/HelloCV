#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

/*vector<Vec3f> detCircle(Mat &img,Mat &model) {


    vector<Vec3f> circles; // 输出向量，每个元素为一个圆，包含(x, y, radius)三个值。
    //使用霍夫圆函数进行圆形的检测
    HoughCircles(img,circles,HOUGH_GRADIENT,1,150,100,33,70,0);
    //将检测出来的函数描绘出来
    for (int i =0;i<circles.size();i++) {
        circle(model,{static_cast<int>(circles[i][0]),static_cast<int>(circles[i][1])},circles[i][2],Scalar(0,0,255),8);
    }
    cout<<circles.size()<<endl;
    return circles;
}*/
    /**以上为HoughCircles函数法检测圆，已弃用！**/
int detCircle(Mat img,Mat imgModel) {
    //使用轮廓检测方法进行圆形的检测
    vector<std::vector<cv::Point>> contours;
    vector<cv::Vec4i> hierarchy;
    vector<Vec3f> Circles;//存储检测出来的圆形，同样是包含(x, y, radius)三个值。
    findContours(img,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    //drawContours(imgModel,contours,-1,Scalar(255,0,255),10);

    for (int i = 0; i < contours.size(); i++) {


        //过滤面积去除Voice
        double area = contourArea(contours[i]);
        if (area < 300) {
            continue ;//面积小则直接跳过
        }

        //计算圆形度，判断到底是不是真的圆形！！The MOST Important！！！
        double perimeter = arcLength(contours[i], true);
        if (perimeter == 0) continue; // 避免除零错误



        //将合适的圆形加入到vector中
        double circularity = (4 * CV_PI * area) / (perimeter * perimeter);
        if (circularity > 0.82) {

            //计算最小外接圆
            Point2f center;
            float radius;
            minEnclosingCircle(contours[i], center, radius);

            //错误写法：Circles.push_back((center.x,center.y,radius));
            Circles.emplace_back(center.x, center.y, radius);
        }
    }
    //画圆

    for (int i =0;i<Circles.size();i++) {
        circle(imgModel,{static_cast<int>(Circles[i][0]),static_cast<int>(Circles[i][1])},Circles[i][2],Scalar(255,255,255),8);
    }
    return Circles.size();
}

Mat preProcess(Mat imgRaw) {//图像预处理函数
    Mat imgProcessed;
    GaussianBlur(imgRaw,imgProcessed,Size(5,5),3,0);
    //Canny(imgProcessed,imgProcessed,50,100);
    Mat kernel = getStructuringElement(MORPH_RECT,Size(3,3));
    erode(imgProcessed,imgProcessed,kernel);
    dilate(imgProcessed,imgProcessed,kernel);
    return imgProcessed;
}

int main() {
    Mat img;//视频的每一帧转换为的原图存储
    VideoCapture cap;//视频捕获器
    Mat imgHSV;//存储HSV色彩空间图样
    //使用视频捕获器读取视频
    string path = "TrafficLight.mp4";
    if (!cap.open(path)) {
        cout << "无法打开视频文件: " << path << endl;
        return -1;
    }
    //cap.open(0);

    //*******以下为视频写入设置
    //获取视频帧的基本属性（宽度、高度、帧率）
    int frame_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(CAP_PROP_FPS);
    //创建frame_size变量
    Size frame_size(frame_width, frame_height);
    //创建VideoWriter对象，定义编码器和输出文件
    VideoWriter writer;
    //选择AVI视频常用的编码器
    int fourcc_code = VideoWriter::fourcc('M', 'J', 'P', 'G');
    std::string output_filename = "result.avi";
    //打开视频写入流
    writer.open(output_filename, fourcc_code, 24, frame_size, true);


    //*********定义红色和绿色HSV的阈值*********//
    // 定义红色的HSV范围（两个区间）
    Scalar lRed1(0, 100, 100);   // 范围1的下界
    Scalar uRed1(10, 255, 255);  // 范围1的上界
    Scalar lRed2(156, 100, 100); // 范围2的下界
    Scalar uRed2(180, 255, 255); // 范围2的上界
    // 定义绿色的HSV范围
    Scalar lGreen(34, 50, 87);
    Scalar uGreen(104, 255, 255);


    //逐帧写入到img中，并进行处理
    while (true) {
        if (!cap.read(img) || img.empty()) break;// 读取失败或到达文件末尾，退出循环

        cvtColor(img,imgHSV,COLOR_BGR2HSV);


        Mat redMask1,redMask2,redMask,greenMask;//创建Mask，其中redMask由1和2合并得到
        inRange(imgHSV,lGreen,uGreen,greenMask);//得到绿色Mask
        inRange(imgHSV,lRed1,uRed1,redMask1);
        inRange(imgHSV,lRed2,uRed2,redMask2);
        redMask = redMask1 | redMask2; //合并两个Mask得到redMask

        //预处理
        redMask = preProcess(redMask);
        greenMask = preProcess(greenMask);

        //圆形检测+文字输出
        if (detCircle(redMask,img)) {
            putText(img,"Red",Point(150,350),FONT_HERSHEY_SIMPLEX,5,Scalar(0,0,255),5);
        };
        if (detCircle(greenMask,img)) {
            putText(img,"Green",Point(150,350),FONT_HERSHEY_SIMPLEX,5,Scalar(0,255,0),5);
        }

        //在这里将img插入到writer中
        writer.write(img);

        //下面是显示区
        //imshow("redMask",redMask);
        //imshow("greenMask",greenMask);
        imshow("OutputVideo_img",img);
        //imshow("OutputVideo_HSV",imgHSV);

        // 延时+ESC 键退出
        int key = waitKey(1);
        if (key == 27) {
            break;
        }

    }

    //后续处理
    cap.release();
    destroyAllWindows();
    writer.release();
    return 0;
}
