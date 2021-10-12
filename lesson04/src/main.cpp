#include <iostream>
#include <filesystem> // это нам понадобится чтобы создать папку для результатов
#include <libutils/rasserts.h>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include "helper_functions.h"
#include <chrono>
#include <thread>
#include <opencv2/highgui.hpp>

#include "disjoint_set.h"
//#include "morphology.h"
#include "helper_functions.h"
//#include "disjoint_set.h"
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;
// 100 реализуйте систему непересекающихся множеств - см. файлы disjoint_set.h и disjoint_set.cpp
// чтобы их потестировать - постарайтесь дописать сюда много разных интересных случаев:
void testingMyDisjointSets() {
    int time1=clock();
    DisjointSet set(5);
    rassert(set.count_differents() == 5, 2378923791);
    for (int element = 0; element < 5; ++element) {
        rassert(set.get_set(element) == element, 23892803643);
        rassert(set.get_set_size(element) == 1, 238928031);
    }
    DisjointSet set2(5);
    cout<<set2.get_set(0)<<endl;
    set2.union_sets(0,1);
    cout<<set2.get_set(0)<<endl;
    cout<<set2.get_set(1)<<endl;
    cout<<set2.count_differents()<<endl;
    DisjointSet set3((int) 1e7);
    for(int i=0;i<(1e7)-1;++i) set3.union_sets(i,i+1);
    cout<<set3.count_differents()<<endl;
    int time2=clock();
    cout<<setprecision(6)<<(time2-time1+0.0)/CLOCKS_PER_SEC<<endl;
    //  100 по мере реализации DisjointSet - добавьте здесь каких-то вызовов операции "объединение двух множеств", сразу после этого проверяя через rassert что после этого результат такой как вы ожидаете
    //  100 затем попробуйте создать СНМ размера 10.000.000 - и пообъединяйте какие-нибудь элементы (в цикле), быстро ли работает? а если при подвешивании одного корня множества к другому мы не будем учитывать ранк (высоту дерева) - как быстро будет работать?
    //  100 попробуйте скомпилировать программу с оптимизациями и посмотреть ускорится ли программа - File->Settings->CMake->Плюсик над Debug->и переключите его в RelWithDebInfo (чтобы были хоть какие-то отладочные символы)
}

// TODO 200 перенесите сюда основную часть кода из прошлого задания про вычитание фона по первому кадру, но:
//for TODO watch lesson 03!!!!
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
vector <pair<int,int> > allmouseclicks;
bool isinverted=false;
bool ban[256][256][256];
cv::Mat image1;
struct MyVideoContent {
    cv::Mat frame;
    int lastClickX;
    int lastClickY;
};

void onMouseClick(int event, int x, int y, int flags, void *pointerToMyVideoContent) {
    MyVideoContent &content = *((MyVideoContent*) pointerToMyVideoContent);
    if (event == cv::EVENT_LBUTTONDOWN) { // если нажата левая кнопка мыши
        std::cout << "Left click at x=" << x << ", y=" << y << std::endl;
        allmouseclicks.push_back({x,y});
        cv::Vec3b color=image1.at<cv::Vec3b>(y,x);
        int blue=color[0];int green=color[1];int red=color[2];
        cout<<blue<<' '<<green<<' '<<red<<endl;
        for(int i=0;i<256;++i)
        {
            int val1=abs(i-blue);
            if(val1>40) continue;
            cout<<val1<<" val1 "<<endl;
            for(int j=0;j<256;++j)
            {
                int val2=val1+abs(j-green);
                if(val2>40) continue;
                for(int k=0;k<256;++k)
                {
                    int val3=val2+abs(k-red);
                    if(val3<=40)
                    {
                        ban[i][j][k]=true;
                    }
                }
            }
        }
    }
    else if (event == cv::EVENT_RBUTTONDOWN) {
        isinverted^=1;
    }
}
void backgroundMagickStreaming() {
    cv::VideoCapture video(0);
    freopen("lesson03/resultsData/mylogfile.txt","w",stdout);
    rassert(video.isOpened(), 3423948392481); // проверяем что видео получилось открыть
    MyVideoContent content;
    bool isSuccess = video.read(content.frame); // считываем из видео очередной кадр
    rassert(isSuccess, 6579849545); // проверяем что считывание прошло успешно
    std::chrono::milliseconds timespan(1000); // or whatever
    std::this_thread::sleep_for(timespan);
    cv::Mat image0=content.frame.clone();
    cv::Mat largeCastle = cv::imread("lesson03/data/dima.jpg").clone();
    std::this_thread::sleep_for(timespan);
    vector <vector <int> > v;
    cv::Mat image2(image0.rows,image0.cols,CV_8UC3,Scalar(0,0,0));
    int eps=150;
    int cyc=0;
    vector <vector <int> > v3;v3.resize(image0.rows);
    for(auto &h:v3) h.resize(image0.cols);
    while (video.isOpened()) { // пока видео не закрылось - бежим по нему
        ++cyc;
        string h=to_string(cyc%300);
        while(h.size()<3) h.insert(h.begin(),'0');
        string filename="lesson03/data/myvideo_"+h+".jpg";
        largeCastle = cv::imread(filename).clone();
        bool isSuccess = video.read(content.frame); // считываем из видео очередной кадр
        rassert(isSuccess, 348792347819); // проверяем что считывание прошло успешно
        rassert(!content.frame.empty(), 3452314124643); // проверяем что кадр не пустой
        image1=content.frame.clone();
        //cout<<" new image"<<endl;
        vector <vector <cv::Vec3b> > image1all(image1.rows);
        for(auto &h:image1all) h.resize(image1.cols);
        for(int i=0;i<image1.rows;++i) {
            for (int j = 0; j < image1.cols; ++j) {
                image1all[i][j] = image1.at<cv::Vec3b>(i, j);
            }
        }
        v.resize(image1.rows);
        for(int i=0;i<image1.rows;++i) {
            v[i].resize(image1.cols);
            for (int j = 0; j < image1.cols; ++j) {
                cv::Vec3b &color = image1.at<cv::Vec3b>(i, j);
                cv::Vec3b color0 = image0.at<cv::Vec3b>(i, j);
                cv::Vec3b &color2= image2.at<cv::Vec3b> (i,j);
                int val = abs(color[0] - color0[0]) + abs(color[1] - color0[1]) + abs(color[2] - color0[2]);
                v3[i][j]=val;
                //if(i==10 && j==10) cout<<(int) color[0]<<' '<<(int) color[1]<<' '<<(int) color[2]<<' '<<(int) color0[0]<<' '<<(int) color0[1]<<' '<<(int) color0[2]<<endl;
                color2[0]=val;color2[1]=0;color2[2]=0;
                bool isclose = (val <= eps);
                v[i][j] = isclose;
                if(cyc%100==0) {
                    cout << isclose << ' ';
                }
            }
            if(cyc%100==0) {
                cout << endl;
            }
        }
        if(cyc%100==0)
        {
            cout<<cyc<<" cyc "<<endl;
        }
        vector <vector <int> > v1=go(v,5,0.5,3);
        for(int i=0;i<image1.rows;++i) {
            v[i].resize(image1.cols);
            for (int j = 0; j < image1.cols; ++j) {
                cv::Vec3b &color = image1.at<cv::Vec3b>(i, j);
                cv::Vec3b color0 = image0.at<cv::Vec3b>(i, j);
                bool isclose=v1[i][j];
                if (isclose) {
                    cv::Vec3b colornew = largeCastle.at<cv::Vec3b>(i * largeCastle.rows / image1.rows,
                                                                   j * largeCastle.cols / image1.cols);
                    //if(isinverted) {colornew[0]=255-colornew[0];colornew[1]=255-colornew[1];colornew[2]=255-colornew[2];}
                    color = colornew;
                }
            }
        }
        if(isinverted)
        {
            image1=invertImageColors(image1);
        }
        cv::imshow("video", image2); // покаызваем очередной кадр в окошке
        cv::setMouseCallback("video", onMouseClick, &content); // делаем так чтобы функция выше (onMouseClick) получала оповещение при каждом клике мышкой

        int key = cv::waitKey(10);
        if(key==32) exit(0);
        if(key==27) exit(0);
    }
}

int main() {
    try {
        //    testingMyDisjointSets();
        backgroundMagickStreaming();
        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}