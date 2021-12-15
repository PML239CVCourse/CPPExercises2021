#include <iostream>
#include <filesystem>
#include <libutils/rasserts.h>

#include "disjoint_set.h"
//#include "morphology.h"

#include <opencv2/highgui.hpp>

// чтобы их потестировать - постарайтесь дописать сюда много разных интересных случаев:
void testingMyDisjointSets() {
    DisjointSet set(10000000);
//    rassert(set.count_differents() == 1000, 2378923791);
    for (int element = 0; element < 5; ++element) {
        rassert(set.get_set(element) == element, 23892803643);
        rassert(set.get_set_size(element) == 1, 238928031);
    }
    set.union_sets(0,1);
    rassert(set.get_set(0)==set.get_set(1),465464)
    set.union_sets(0,999);
    rassert(set.get_set(999)==set.get_set(1),465464)
    for(int i = 0;i<10000000-1000;i+=1)
        set.union_sets(i,i+239);
    for(int i = 0;i<2;i+=1)
        set.union_sets(i,i+1);
//    for(int i = 0;i<10000000-1000;i+=1)
//        set.union_sets(i,i+239+22);
//    for(int i = 0;i<10000000-1000;i+=1)
//        set.union_sets(i,i+239+33);
//    for(int i = 0;i<10000000-1000;i+=1)
//        set.union_sets(i,i+239+445);
//    rassert(set.count_differents() == 1000-2, 2378923791);
}
struct MyVideoContent {
    cv::Mat frame;
    int lastClickX = 0;
    int lastClickY = 0;
    std::vector<int>clickX;
    std::vector<int>clickY;
    std::vector<int>invclickX;
    std::vector<int>invclickY;
    bool invflag = 0;
};

void onMouseClick(int event, int x, int y, int flags, void *pointerToMyVideoContent) {
    MyVideoContent &content = *((MyVideoContent*) pointerToMyVideoContent);
    // не обращайте внимание на предыдущую строку, главное что важно заметить:
    // content.frame - доступ к тому кадру что был только что отображен на экране
    // content.lastClickX - переменная которая вам тоже наверняка пригодится
    // вы можете добавить своих переменных в структурку выше (считайте что это описание объекта из ООП, т.к. почти полноценный класс)

    if (event == cv::EVENT_LBUTTONDOWN) { // если нажата левая кнопка мыши
        std::cout << "Left click at x=" << x << ", y=" << y << std::endl;
        content.lastClickX = x;
        content.lastClickY = y;
        content.clickY.push_back(y);
        content.clickX.push_back(x);
    }
    if (event == cv::EVENT_RBUTTONDOWN) {
        std::cout << "Right click at x=" << x << ", y=" << y << std::endl;
        content.invflag^=1;
    }

}
cv::Mat addBackgroundInsteadOfColorPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground, cv::Vec3b color) {
    // теперь вам гарантируется что largeBackground гораздо больше - добавьте проверок этого инварианта (rassert-ов)
//    int width = largeBackground.cols; // как в ООП - у картинки есть поля доступные через точку, они называются cols и rows - попробуйте их
//    int height = largeBackground.rows;
//    std::cout << "Unicorn image loaded: " << width << "x" << height << std::endl;


    int object_width = object.cols;
    int object_height = object.rows;
    cv::Mat myNewImage(object_height,object_width, CV_8UC3, cv::Scalar(0, 0, 0));
    int largeBackground_width = largeBackground.cols;
    int largeBackground_height = largeBackground.rows;
    for(int i = 0;i<object_height;i++)
        for(int j =0;j<object_width;j++){
            cv::Vec3b object_color = object.at<cv::Vec3b>(i, j);
            cv::Vec3b background_color = largeBackground.at<cv::Vec3b>(i*largeBackground_height/object_height, j*largeBackground_width/object_width);
            unsigned char blue = object_color[0];
            unsigned char green = object_color[1];
            unsigned char red = object_color[2];
            if(abs(blue-color[0])<20 && abs(green-color[1])<20&&abs(red-color[2])<20){
                red = background_color[2];
                green = background_color[1];
                blue = background_color[0];
            }
            myNewImage.at<cv::Vec3b>(i, j) = cv::Vec3b(blue, green, red);
        }

    return myNewImage;
}
// 1) добавьте сохранение на диск визуализации картинок:
// 1.1) картинка эталонного фона
// 1.2) картинка текущего кадра
// 1.3) картинка визуализирующая маску "похож ли пиксель текущего кадра на эталонный фон"
// 1.4) картинка визуализирующая эту маску после применения волшебства морфологии
// 1.5) картинка визуализирующая эту маску после применения волшебства СНМ (системы непересекающихся множеств)
// 2) сохраняйте эти картинки визуализации только для тех кадров, когда пользователем был нажат пробел (код 32)
// 3) попробуйте добавить с помощью нажатия каких то двух кнопок "усиление/ослабление подавления фона"
// 4) попробуйте поменять местами морфологию и СНМ
// 5) попробуйте добавить настройку параметров морфологии и СНМ по нажатию кнопок (и выводите их значения в консоль)
void backgroundMagickStreaming() {
    cv::VideoCapture video("C:\\Users\\ratkovva.22\\CLionProjects\\CPPExercises2021\\lesson04\\data\\Spin_1.mp4");
    //cv::VideoCapture video(0);
    rassert(video.isOpened(), 3423948392481); // проверяем что видео получилось открыть

    MyVideoContent content; // здесь мы будем хранить всякие полезности - например последний видео кадр, координаты последнего клика и т.п.
    // content.frame - доступ к тому кадру что был только что отображен на экране
    // content.lastClickX - переменная которая вам тоже наверняка пригодится
    // вы можете добавить своих переменных в структурку выше (считайте что это описание объекта из ООП, т.к. почти полноценный класс)
    // просто перейдите к ее объявлению - удерживая Ctrl сделайте клик левой кнопкой мыши по MyVideoContent - и вас телепортирует к ее определению
    content.lastClickX = 1;
    content.lastClickY = 1;
    cv::Mat largeCastle = cv::imread("lesson03/data/castle_large.jpg");
    cv::Vec3b color = {0,0,0};
    while (video.isOpened()) { // пока видео не закрылось - бежим по нему
        bool isSuccess = video.read(content.frame); // считываем из видео очередной кадр
        rassert(isSuccess, 348792347819); // проверяем что считывание прошло успешно
        rassert(!content.frame.empty(), 3452314124643); // проверяем что кадр не пустой
        if(content.lastClickX!=1&&content.lastClickY!=1)
        {
            int y = content.lastClickX+1-1;
            int x = content.lastClickY+1-1;
            cv::Vec3b color = content.frame.at<cv::Vec3b>(x, y);
            content.frame = addBackgroundInsteadOfColorPixelsLargeBackground(content.frame,largeCastle,color);
        }


        cv::imshow("video", content.frame); // покаызваем очередной кадр в окошке
        cv::setMouseCallback("video", onMouseClick, &content); // делаем так чтобы функция выше (onMouseClick) получала оповещение при каждом клике мышкой

        int key = cv::waitKey(10);
        if(key == 32 || key == 27)
            return ;
    }
}

int main() {
    try {
//        testingMyDisjointSets();
        backgroundMagickStreaming();
        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}
