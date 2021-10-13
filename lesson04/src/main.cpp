#include <iostream>
#include <filesystem>
#include <libutils/rasserts.h>

#include "disjoint_set.h"
#include "morphology.h"

#include <opencv2/highgui.hpp>

// TODO 100 реализуйте систему непересекающихся множеств - см. файлы disjoint_set.h и disjoint_set.cpp
// чтобы их потестировать - постарайтесь дописать сюда много разных интересных случаев:
void testingMyDisjointSets() {
    DisjointSet set(5);
    rassert(set.count_differents() == 5, 2378923791);
    for (int element = 0; element < 5; ++element) {
        rassert(set.get_set(element) == element, 23892803643);
        rassert(set.get_set_size(element) == 1, 238928031);
    }

    // TODO 100 по мере реализации DisjointSet - добавьте здесь каких-то вызовов операции "объединение двух множеств", сразу после этого проверяя через rassert что после этого результат такой как вы ожидаете
    // TODO 100 затем попробуйте создать СНМ размера 10.000.000 - и пообъединяйте какие-нибудь элементы (в цикле), быстро ли работает? а если при подвешивании одного корня множества к другому мы не будем учитывать ранк (высоту дерева) - как быстро будет работать?
    // TODO 100 попробуйте скомпилировать программу с оптимизациями и посмотреть ускорится ли программа - File->Settings->CMake->Плюсик над Debug->и переключите его в RelWithDebInfo (чтобы были хоть какие-то отладочные символы)
}

// TODO 200 перенесите сюда основную часть кода из прошлого задания про вычитание фона по первому кадру, но:
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

struct MyVideoContent {
    cv::Mat frame;
    int lastClickX;
    int lastClickY;
};

void onMouseClick(int event, int x, int y, int flags, void *pointerToMyVideoContent) {
    MyVideoContent &content = *((MyVideoContent*) pointerToMyVideoContent);
    if (event == cv::EVENT_LBUTTONDOWN) { // если нажата левая кнопка мыши
        std::cout << "Left click at x=" << x << ", y=" << y << std::endl;
    }
}

void backgroundMagickStreaming() {
    cv::VideoCapture video(0);
        MyVideoContent content;
        //MyVideoContent content_bg;
        bool isSuccess_beg = video.read(content.frame); // считываем из видео очередной кадр
        rassert(isSuccess_beg, 348792347819); // проверяем что считывание прошло успешно
        rassert(!content.frame.empty(), 3452314124643); // проверяем что кадр не пусто
        cv::Mat init_pict = content.frame.clone();
        std::string resultsDir = "lesson04/resultsData/";
        std::string picture_before_vid = resultsDir + "init_picture.jpg";
        cv::imwrite(picture_before_vid, init_pict);


        //while (video_bg.isOpened()) { // пока видео не закрылось - бежим по нему
        for(;;){
            bool isSuccess = video.read(content.frame); // считываем из видео очередной кадр
            rassert(isSuccess, 348792347819); // проверяем что считывание прошло успешно
            rassert(!content.frame.empty(), 3452314124643); // проверяем что кадр не пустой

            cv::Mat ready_picture = videoWithBackground(content.frame.clone() , init_pict.clone());

            int updateDelay = 10;
            if(cv::waitKey(updateDelay) == 44){                           // pressing print screen
                ready_picture = invertImageColors(ready_picture);
            }

            cv::imshow("video", ready_picture);
            cv::setMouseCallback("video", onMouseClick, &content); // делаем так чтобы функция выше (onMouseClick) получала оповещение при каждом клике мышкой

            std::string resultsDir = "lesson04/resultsData/";
            std::string picture_before_vid = resultsDir + "picture.jpg";
            cv::imwrite(picture_before_vid, init_pict);


            //int key = cv::waitKey(1);
            //int updateDelay = 10;
            if(cv::waitKey(updateDelay) == 32)
                break;
            if(cv::waitKey(updateDelay) == 27)
                break;

            // TODO сделайте по правому клику мышки переключение в режим "цвета каждого кадра инвертированы" (можете просто воспользоваться функцией invertImageColors)
        }
}

int main() {
    try {
        //testingMyDisjointSets();
      backgroundMagickStreaming();
        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}
