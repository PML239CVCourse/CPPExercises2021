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

#include <opencv2/highgui.hpp> // подключили часть библиотеки OpenCV, теперь мы можем читать и сохранять картинки
using namespace std;
using namespace cv;
void task1() {
    cv::Mat imgUnicorn = cv::imread("lesson03/data/unicorn.png").clone();  // загружаем картинку с единорогом
    rassert(!imgUnicorn.empty(), 3428374817241); // проверяем что картинка загрузилась (что она не пустая)

    //  выведите в консоль разрешение картинки (ширина x высота)
    int width = imgUnicorn.cols; // как в ООП - у картинки есть поля доступные через точку, они называются cols и rows - попробуйте их
    int height = imgUnicorn.rows;
    std::cout << "Unicorn image loaded: " << width << "x" << height << std::endl;

    // создаем папку в которую будем сохранять результаты - lesson03/resultsData/
    std::string resultsDir = "lesson03/resultsData/";
    if (!std::filesystem::exists(resultsDir)) { // если папка еще не создана
        std::filesystem::create_directory(resultsDir); // то создаем ее
    }
    cv::Mat blueUnicorn = makeAllBlackPixelsBlue(imgUnicorn); //  реализуйте функцию которая каждый пиксель картинки который близок к белому - делает синим
    std::string filename = resultsDir + "01_blue_unicorn.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename, blueUnicorn);

    cv::Mat imgUnicorn2 = cv::imread("lesson03/data/unicorn.png").clone();
    cv::Mat invertedUnicorn = invertImageColors(imgUnicorn2); //  реализуйте функцию которая каждый цвет картинки инвертирует
    //  сохраните резльутат в ту же папку, но файл назовите "02_inv_unicorn.jpg"
    filename = resultsDir + "02_inv_unicorn.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename, invertedUnicorn);

    cv::Mat imgUnicorn3 = cv::imread("lesson03/data/unicorn.png").clone();
    cv::Mat castle= cv::imread("lesson03/data/castle.png").clone(); //  считайте с диска картинку с замком - castle.png
    cv::Mat unicornInCastle = addBackgroundInsteadOfBlackPixels(imgUnicorn3, castle); // реализуйте функцию которая все черные пиксели картинки-объекта заменяет на пиксели с картинки-фона
    // сохраните результат в ту же папку, назовите "03_unicorn_castle.jpg"
    filename = resultsDir + "03_unicorn_castle.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename, unicornInCastle);

    cv::Mat imgUnicorn4 = cv::imread("lesson03/data/unicorn.png").clone();
    cv::Mat largeCastle = cv::imread("lesson03/data/castle_large.jpg").clone();// считайте с диска картинку с большим замком - castle_large.png
    cv::Mat unicornInLargeCastle = addBackgroundInsteadOfBlackPixelsLargeBackground(imgUnicorn4, largeCastle); // TODO реализуйте функцию так, чтобы нарисовался объект ровно по центру на данном фоне, при этом черные пиксели объекта не должны быть нарисованы
    //  сохраните результат - "04_unicorn_large_castle.jpg"
    filename = resultsDir + "04_unicorn_large_castle.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename, unicornInLargeCastle);

    // сделайте то же самое, но теперь пусть единорог рисуется N раз (случайно выбранная переменная от 0 до 100)
    cv::Mat imgUnicorn5 = cv::imread("lesson03/data/unicorn.png").clone();
    cv::Mat largeCastle2 = cv::imread("lesson03/data/castle_large.jpg").clone();//
    srand((int) time(NULL));
    int N=rand()%((int) 101);
    cv::Mat azazimage = getanime(imgUnicorn5, largeCastle2, N);
    // функцию вам придется объявить самостоятельно, включая:
    // 1) придумывание ее имени
    // 2) добавление декларации в helper_functions.h (три аргумента - объект, фон, число рисований объекта)
    // 3) добавление реализации в helper_functions.cpp (список аргументов должен совпадать со списком в декларации)
    // 4) как генерировать случайные числа - найдите самостоятельно через гугл, например "c++ how to random int"
    // 5) при этом каждый единорог рисуется по случайным координатам
    // 6) результат сохраните - "05_unicorns_otake.jpg"
    filename = resultsDir + "05_unicorns_otake.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename, azazimage);
    // растяните картинку единорога так, чтобы она заполнила полностью большую картинку с замком "06_unicorn_upscale.jpg"
}

void task2() {
    cv::Mat imgUnicorn = cv::imread("lesson03/data/unicorn.png").clone();
    rassert(!imgUnicorn.empty(), 3428374817241);

    // cv::waitKey - функция некоторое время ждет не будет ли нажата кнопка клавиатуры, если да - то возвращает ее код
    int updateDelay = 10; // указываем сколько времени ждать нажатия кнопки клавиатуры - в миллисекундах
    cv::Mat imgUnicorn2=imgUnicorn.clone();
    while (cv::waitKey(updateDelay) != 32) {
        // поэтому если мы выполняемся до тех пор пока эта функция не вернет код 32 (а это код кнопки "пробел"), то достаточно будет нажать на пробел чтобы закончить работу программы

        // кроме сохранения картинок на диск (что часто гораздо удобнее конечно, т.к. между ними легко переключаться)
        // иногда удобно рисовать картинку в окне:
        cv::imshow("lesson03 window", imgUnicorn2);
        // сделайте функцию которая будет все черные пиксели (фон) заменять на случайный цвет (аккуратно, будет хаотично и ярко мигать, не делайте если вам это противопоказано)
        imgUnicorn2=blackrandom(imgUnicorn);
    }
}
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
    // не обращайте внимание на предыдущую строку, главное что важно заметить:
    // content.frame - доступ к тому кадру что был только что отображен на экране
    // content.lastClickX - переменная которая вам тоже наверняка пригодится
    // вы можете добавить своих переменных в структурку выше (считайте что это описание объекта из ООП, т.к. почти полноценный класс)

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

void task3() {
    // давайте теперь вместо картинок подключим видеопоток с веб камеры:
    cv::VideoCapture video(0);
    // если у вас нет вебкамеры - подключите ваш телефон к компьютеру как вебкамеру - это должно быть не сложно (загуглите)
    // альтернативно если у вас совсем нет вебки - то попробуйте запустить с видеофайла, но у меня не заработало - из-за "there is API version mismath: plugin API level (0) != OpenCV API level (1)"
    // скачайте какое-нибудь видео с https://www.videezy.com/free-video/chroma-key
    // например https://www.videezy.com/elements-and-effects/5594-interactive-hand-gesture-sliding-finger-studio-green-screen
    // если вы увидите кучу ошибок в консоли навроде "DynamicLib::libraryLoad load opencv_videoio_ffmpeg451_64.dll => FAILED", то скопируйте файл C:\...\opencv\build\x64\vc14\bin\opencv_videoio_ffmpeg451_64.dll в папку с проектом
    // и укажите путь к этому видео тут:
//    cv::VideoCapture video("lesson03/data/Spin_1.mp4");

    rassert(video.isOpened(), 3423948392481); // проверяем что видео получилось открыть
    MyVideoContent content;
    // здесь мы будем хранить всякие полезности - например последний видео кадр, координаты последнего клика и т.п.
    // content.frame - доступ к тому кадру что был только что отображен на экране
    // content.lastClickX - переменная которая вам тоже наверняка пригодится
    // вы можете добавить своих переменных в структурку выше (считайте что это описание объекта из ООП, т.к. почти полноценный класс)
    // просто перейдите к ее объявлению - удерживая Ctrl сделайте клик левой кнопкой мыши по MyVideoContent - и вас телепортирует к ее определению
    while (video.isOpened()) { // пока видео не закрылось - бежим по нему
        bool isSuccess = video.read(content.frame); // считываем из видео очередной кадр
        rassert(isSuccess, 348792347819); // проверяем что считывание прошло успешно
        rassert(!content.frame.empty(), 3452314124643); // проверяем что кадр не пустой
        image1=content.frame.clone();
        //cout<<" new image"<<endl;
        for(auto h:allmouseclicks)
        {
            //cout<<h.first<<' '<<h.second<<endl;
            cv::Vec3b &color = image1.at<cv::Vec3b>(h.second,h.first);
            color[0]=0;color[1]=0;color[2]=255;
        }
        if(isinverted)
        {
            image1=invertImageColors(image1);
        }
        cv::imshow("video", image1); // покаызваем очередной кадр в окошке
        cv::setMouseCallback("video", onMouseClick, &content); // делаем так чтобы функция выше (onMouseClick) получала оповещение при каждом клике мышкой

        int key = cv::waitKey(10);
        if(key==32) exit(0);
        if(key==27) exit(0);

        // сохраняйте в вектор (std::vector<int>) координаты всех кликов мышки
        //  и перед отрисовкой очередного кадра - заполняйте все уже прокликанные пиксели красным цветом
        // сделайте по правому клику мышки переключение в режим "цвета каждого кадра инвертированы" (можете просто воспользоваться функцией invertImageColors)
    }
}

void task4() {
    cv::VideoCapture video(0);

    rassert(video.isOpened(), 3423948392481); // проверяем что видео получилось открыть

    MyVideoContent content;
    cv::Mat largeCastle = cv::imread("lesson03/data/castle_large.jpg").clone();
    while (video.isOpened()) { // пока видео не закрылось - бежим по нему
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
        for(int i=0;i<image1.rows;++i) {
            for(int j=0;j<image1.cols;++j) {
                cv::Vec3b &color = image1.at<cv::Vec3b>(i,j);
                bool isclose=ban[color[0]][color[1]][color[2]];
                /*for(auto h:allmouseclicks) {
                    //cout<<i<<" i "<<j<<" j "<<endl;
                    //cout<<h.first<<' '<<h.second<<endl;
                    cv::Vec3b color2 = image1all[h.second][h.first];
                    if(abs(color[0]-color2[0])>40) continue;
                    int val=abs(color[0]-color2[0])+abs(color[1]-color2[1])+abs(color[2]-color2[2]);
                    if(val<40)
                    {
                        isclose=true;
                    }
                }*/
                if(isclose)
                {
                    cv::Vec3b colornew=largeCastle.at<cv::Vec3b>(i*largeCastle.rows/image1.rows,j*largeCastle.cols/image1.cols);
                    //if(isinverted) {colornew[0]=255-colornew[0];colornew[1]=255-colornew[1];colornew[2]=255-colornew[2];}
                    color=colornew;
                }
            }
        }
        if(isinverted)
        {
            image1=invertImageColors(image1);
        }
        cv::imshow("video", image1); // покаызваем очередной кадр в окошке
        cv::setMouseCallback("video", onMouseClick, &content); // делаем так чтобы функция выше (onMouseClick) получала оповещение при каждом клике мышкой

        int key = cv::waitKey(10);
        if(key==32) exit(0);
        if(key==27) exit(0);

        // сохраняйте в вектор (std::vector<int>) координаты всех кликов мышки
        //  и перед отрисовкой очередного кадра - заполняйте все уже прокликанные пиксели красным цветом
        // сделайте по правому клику мышки переключение в режим "цвета каждого кадра инвертированы" (можете просто воспользоваться функцией invertImageColors)
    }
    // при клике мышки - определяется цвет пикселя в который пользователь кликнул, теперь этот цвет считается прозрачным (как было с черным цветом у единорога)
    // и теперь перед отрисовкой очередного кадра надо подложить вместо прозрачных пикселей - пиксель из отмасштабированной картинки замка (castle_large.jpg)

    // попробуйте сделать так чтобы цвет не обязательно совпадал абсолютно для прозрачности (чтобы все пиксели похожие на тот что был кликнут - стали прозрачными, а не только идеально совпадающие)

    // TODO подумайте, а как бы отмаскировать фон целиком несмотря на то что он разноцветный?
    // а как бы вы справились с тем чтобы из фотографии с единорогом и фоном удалить фон зная как выглядит фон?
    // а может сделать тот же трюк с вебкой - выйти из вебки в момент запуска программы, и первый кадр использовать как кадр-эталон с фоном который надо удалять (делать прозрачным)
}
void task4maskfon() {
    cv::VideoCapture video(0);

    rassert(video.isOpened(), 3423948392481); // проверяем что видео получилось открыть
    MyVideoContent content;
    bool isSuccess = video.read(content.frame); // считываем из видео очередной кадр
    rassert(isSuccess, 6579849545); // проверяем что считывание прошло успешно
    cv::Mat image0=content.frame.clone();
    cv::Mat largeCastle = cv::imread("lesson03/data/castle_large.jpg").clone();
    while (video.isOpened()) { // пока видео не закрылось - бежим по нему
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
        for(int i=0;i<image1.rows;++i) {
            for(int j=0;j<image1.cols;++j) {
                cv::Vec3b &color = image1.at<cv::Vec3b>(i,j);
                cv::Vec3b color0=image0.at<cv::Vec3b>(i,j);
                int val=abs(color[0]-color0[0])+abs(color[1]-color0[1])+abs(color[2]-color0[2]);
                //if(i==10 && j==10) cout<<(int) color[0]<<' '<<(int) color[1]<<' '<<(int) color[2]<<' '<<(int) color0[0]<<' '<<(int) color0[1]<<' '<<(int) color0[2]<<endl;
                bool isclose=(val<=100);
                /*for(auto h:allmouseclicks) {
                    //cout<<i<<" i "<<j<<" j "<<endl;
                    //cout<<h.first<<' '<<h.second<<endl;
                    cv::Vec3b color2 = image1all[h.second][h.first];
                    if(abs(color[0]-color2[0])>40) continue;
                    int val=abs(color[0]-color2[0])+abs(color[1]-color2[1])+abs(color[2]-color2[2]);
                    if(val<40)
                    {
                        isclose=true;
                    }
                }*/
                if(isclose)
                {
                    cv::Vec3b colornew=largeCastle.at<cv::Vec3b>(i*largeCastle.rows/image1.rows,j*largeCastle.cols/image1.cols);
                    //if(isinverted) {colornew[0]=255-colornew[0];colornew[1]=255-colornew[1];colornew[2]=255-colornew[2];}
                    color=colornew;
                }
            }
        }
        if(isinverted)
        {
            image1=invertImageColors(image1);
        }
        cv::imshow("video", image1); // покаызваем очередной кадр в окошке
        cv::setMouseCallback("video", onMouseClick, &content); // делаем так чтобы функция выше (onMouseClick) получала оповещение при каждом клике мышкой

        int key = cv::waitKey(10);
        if(key==32) exit(0);
        if(key==27) exit(0);

        // сохраняйте в вектор (std::vector<int>) координаты всех кликов мышки
        //  и перед отрисовкой очередного кадра - заполняйте все уже прокликанные пиксели красным цветом
        // сделайте по правому клику мышки переключение в режим "цвета каждого кадра инвертированы" (можете просто воспользоваться функцией invertImageColors)
    }
    // при клике мышки - определяется цвет пикселя в который пользователь кликнул, теперь этот цвет считается прозрачным (как было с черным цветом у единорога)
    // и теперь перед отрисовкой очередного кадра надо подложить вместо прозрачных пикселей - пиксель из отмасштабированной картинки замка (castle_large.jpg)

    // попробуйте сделать так чтобы цвет не обязательно совпадал абсолютно для прозрачности (чтобы все пиксели похожие на тот что был кликнут - стали прозрачными, а не только идеально совпадающие)

    // подумайте, а как бы отмаскировать фон целиком несмотря на то что он разноцветный?
    // а как бы вы справились с тем чтобы из фотографии с единорогом и фоном удалить фон зная как выглядит фон?
    // а может сделать тот же трюк с вебкой - выйти из вебки в момент запуска программы, и первый кадр использовать как кадр-эталон с фоном который надо удалять (делать прозрачным)
}
void lesson4tasks()
{
    cv::Mat image=cv::imread("lesson03/data/castle_large.jpg").clone();
    cv::Mat image1=tuda(image,500,239);
    std::string resultsDir = "lesson03/resultsData/";
    if (!std::filesystem::exists(resultsDir)) { // если папка еще не создана
        std::filesystem::create_directory(resultsDir); // то создаем ее
    }
    string filename = resultsDir + "aazazazz.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename, image1);
}
void task4maskfonk() {
    cv::VideoCapture video(0);

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
    int eps=50;
    int cyc=0;
    while (video.isOpened()) { // пока видео не закрылось - бежим по нему
        ++cyc;
        string h=to_string(cyc);
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
                int val = abs(color[0] - color0[0]) + abs(color[1] - color0[1]) + abs(color[2] - color0[2]);
                //if(i==10 && j==10) cout<<(int) color[0]<<' '<<(int) color[1]<<' '<<(int) color[2]<<' '<<(int) color0[0]<<' '<<(int) color0[1]<<' '<<(int) color0[2]<<endl;
                bool isclose = (val <= eps);
                v[i][j] = isclose;
                /*for(auto h:allmouseclicks) {
                    //cout<<i<<" i "<<j<<" j "<<endl;
                    //cout<<h.first<<' '<<h.second<<endl;
                    cv::Vec3b color2 = image1all[h.second][h.first];
                    if(abs(color[0]-color2[0])>40) continue;
                    int val=abs(color[0]-color2[0])+abs(color[1]-color2[1])+abs(color[2]-color2[2]);
                    if(val<40)
                    {
                        isclose=true;
                    }
                }*/
                /*if (isclose) {
                    cv::Vec3b colornew = largeCastle.at<cv::Vec3b>(i * largeCastle.rows / image1.rows,
                                                                   j * largeCastle.cols / image1.cols);
                    //if(isinverted) {colornew[0]=255-colornew[0];colornew[1]=255-colornew[1];colornew[2]=255-colornew[2];}
                    color = colornew;
                }*/
            }
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
        cv::imshow("video", image1); // покаызваем очередной кадр в окошке
        cv::setMouseCallback("video", onMouseClick, &content); // делаем так чтобы функция выше (onMouseClick) получала оповещение при каждом клике мышкой

        int key = cv::waitKey(10);
        if(key==32) exit(0);
        if(key==27) exit(0);

        // сохраняйте в вектор (std::vector<int>) координаты всех кликов мышки
        //  и перед отрисовкой очередного кадра - заполняйте все уже прокликанные пиксели красным цветом
        // сделайте по правому клику мышки переключение в режим "цвета каждого кадра инвертированы" (можете просто воспользоваться функцией invertImageColors)
    }
    // при клике мышки - определяется цвет пикселя в который пользователь кликнул, теперь этот цвет считается прозрачным (как было с черным цветом у единорога)
    // и теперь перед отрисовкой очередного кадра надо подложить вместо прозрачных пикселей - пиксель из отмасштабированной картинки замка (castle_large.jpg)

    // попробуйте сделать так чтобы цвет не обязательно совпадал абсолютно для прозрачности (чтобы все пиксели похожие на тот что был кликнут - стали прозрачными, а не только идеально совпадающие)

    // подумайте, а как бы отмаскировать фон целиком несмотря на то что он разноцветный?
    // а как бы вы справились с тем чтобы из фотографии с единорогом и фоном удалить фон зная как выглядит фон?
    // а может сделать тот же трюк с вебкой - выйти из вебки в момент запуска программы, и первый кадр использовать как кадр-эталон с фоном который надо удалять (делать прозрачным)
}
int main() {
    try {
        // task1();
        // task2();
        // task3();
        //task4();
        // task4maskfon();
        //lesson4tasks();
        task4maskfonk();
        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}