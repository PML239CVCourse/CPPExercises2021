#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <chrono>
#include <iostream>
#include <filesystem>
#include <memory>

#include <libutils/rasserts.h>


void drawText(cv::Mat img, std::string text, double fontScale, int &yOffset) {
    cv::Scalar white(255, 255, 255);
    // рассчитываем высоту текста в пикселях:
    float textHeight = cv::getTextSize(text, cv::FONT_HERSHEY_DUPLEX, fontScale, 1, nullptr).height;
    yOffset += textHeight; // увеличиваем сдвиг на высоту текста в пикселях
    cv::putText(img, text, cv::Point(0, yOffset), cv::FONT_HERSHEY_DUPLEX, fontScale, white);
}


void run() {
    const bool useWebcam = true; // TODO попробуйте выставить в true, если у вас работает вебкамера - то и здорово! иначе - работайте хотя бы со статичными картинками

    bool drawOver = true; // рисовать ли поверх наложенную картинку (можно включить-включить чтобы мигнуть картинкой и проверить качество выравнивания)
    bool drawDebug = true; // рисовать ли поверх отладочную информацию (например красный кант вокруг нарисованной поверх картинки)
    bool useSIFTDescriptor = true; // SIFT работает довольно медленно, попробуйте использовать ORB + не забудьте что тогда вам нужен другой DescriptorMatcher

    cv::Mat imgFrame, imgForDetection, imgToDraw;
    // если у вас не работает через веб. камеру - будут использоваться заготовленные картинки
    imgFrame = cv::imread("lesson14/data/1_box2/box0.png"); // пример кадра с вебкамеры, на нем мы хотим найти объект и вместо него нарисовать другую картинку
    imgForDetection = cv::imread("lesson14/data/1_box2/box1.png"); // пример картинки которую мы хотим найти на видеокадре
    imgToDraw = cv::imread("lesson14/data/1_box2/box1_nesquik.png"); // пример картинки которую мы хотим нарисовать заместо искомой
    rassert(!imgFrame.empty(), 324789374290023);
    rassert(!imgForDetection.empty(), 3789572984290019);
    rassert(!imgToDraw.empty(), 3789572984290021);

    std::shared_ptr<cv::VideoCapture> video;
    if (useWebcam) {
        std::cout << "Trying to open web camera..." << std::endl;
        video = std::make_shared<cv::VideoCapture>(0);
        rassert(video->isOpened(), 3482789346289027);
        std::cout << "Web camera video stream opened." << std::endl;
    }

    while (true) {
        cv::Mat currentFrame; // текущий кадр с веб. камеры
        if (useWebcam) {
            bool isSuccess = video->read(currentFrame);
            rassert(isSuccess, 347283947290039);
            rassert(!currentFrame.empty(), 347283947290040);
        } else {
            currentFrame = imgFrame; // или если у вас не работает OpenCV с веб. камерой - то пусть хотя бы картинка-пример используется
        }

        auto frameProcessingStartTime = std::chrono::steady_clock::now(); // замеряем сколько сейчас времени чтобы оценить FPS

        cv::Mat mainWindowImage = currentFrame.clone(); // делаем копию чтобы на ней рисовать любую отладочную информацию не портя оригинальную картинку

        {
            // TODO сопоставьте две картинки: currentFrame и imgForDetection, затем нарисуйте imgToDraw в соответствии с матрицей Гомографии
            cv::Ptr<cv::FeatureDetector> detector;
            cv::Ptr<cv::DescriptorMatcher> matcher;
            if (useSIFTDescriptor) {
                detector = cv::SIFT::create();
                matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);
            } else {
                // TODO SIFT работает довольно медленно, попробуйте использовать ORB + не забудьте что тогда вам нужен другой DescriptorMatcher

                // TODO кроме того будет быстрее работать если вы будете использовать релизную сборку вместо Debug:
                // см. "Как ускорить программу" - https://www.polarnick.com/blogs/239/2021/school239_11_2021_2022/2021/10/05/lesson5-disjoint-set.html
            }

            // TODO детектируйте и постройте дескрипторы у ключевых точек
            // std::cout << "Keypoints initially: " << keypoints0.size() << ", " << keypoints1.size() << "..." << std::endl;

            // TODO сопоставьте ключевые точки

            // TODO пофильтруйте сопоставления, как минимум через K-ratio test, но лучше на ваш выбор
//            std::vector<cv::Point2f> points0;
//            std::vector<cv::Point2f> points1;
//            for (int i = 0; i < keypoints0.size(); ++i) {
//                int fromKeyPoint0 = ....queryIdx;
//                int toKeyPoint1Best = ....trainIdx;
//                float distanceBest = ....distance;
//                rassert(fromKeyPoint0 == i, 348723974920074);
//                rassert(toKeyPoint1Best < keypoints1.size(), 347832974820076);

//                int toKeyPoint1SecondBest = ....trainIdx;
//                float distanceSecondBest = ....distance;
//                rassert(toKeyPoint1SecondBest < keypoints1.size(), 3482047920081);
//                rassert(distanceBest <= distanceSecondBest, 34782374920082);

//                if (TODO) {
//                    points0.push_back(keypoints0[i].pt);
//                    points1.push_back(keypoints1[toKeyPoint1Best].pt);
//                }
//            }
//            rassert(points0.size() == points1.size(), 234723947289089);
            // TODO добавьте вывод в лог - сколько ключевых точек было изначально, и сколько осталось сопоставлений после фильтрации

            // TODO findHomography(...) + рисование поверх:
//            cv::Mat H01 = cv::findHomography(TODO);
//            if (H01.empty()) {
//                // см. документацию https://docs.opencv.org/4.5.1/d9/d0c/group__calib3d.html#ga4abc2ece9fab9398f2e560d53c8c9780
//                // "Note that whenever an H matrix cannot be estimated, an empty one will be returned."
//                std::cout << "FAIL 24123422!" << std::endl;
//            } else {
//                cv::Mat overlapImg = imgToDraw.clone();
//                if (drawDebug) {
//                    // рисуем красный край у накладываемой картинки
//                    cv::Scalar red(0, 0, 255);
//                    cv::rectangle(overlapImg, cv::Point(0, 0), cv::Point(overlapImg.cols-1, overlapImg.rows-1), red, 2);
//                }
//                if (drawOver) {
//                    // cv::warpPerspective(TODO);
//                }
//            }
        }

        if (drawDebug) {
            int textYOffset = 0;

            auto frameProcessingEndTime = std::chrono::steady_clock::now();
            int timeForFrame = std::chrono::duration_cast<std::chrono::milliseconds>(frameProcessingEndTime - frameProcessingStartTime).count();
            int fps;
            if (timeForFrame == 0) {
                fps = 99999;
            } else {
                fps = (int) std::round(1000.0 / timeForFrame);
            }
            drawText(mainWindowImage, std::to_string(fps) + " FPS", 0.5, textYOffset);

            // TODO добавьте короткую справку про кнопки управления
            drawText(mainWindowImage, "Controls: ", 0.5, textYOffset);

            // TODO добавьте разбивку сколько времени занимает детектирование, сколько матчинг, сколько фильтрация (по аналогии с тем как выше замерялось время на обработку для рассчета FPS):
//            drawText(mainWindowImage, "Timings: " + std::to_string(timeForFrame) + " ms = "
//                    + std::to_string(detect_ms) + " ms detect + " + TODO,
//                    0.5, textYOffset);
        }

        // Рисуем все три окошка:
        cv::imshow("Frame", mainWindowImage);
        cv::imshow("Image for detection", imgForDetection);
        cv::imshow("Image to draw", imgToDraw);

        // Смотрим нажал ли пользователь какую-нибудь кнопку
        int key = cv::waitKey(10); // число в скобочках - максимальное число миллисекунд которые мы ждем кнопки от пользователя, а иначе - считаем что ничего не нажато
        if (key == -1) {
            // прошло 5 миллисекунд но ничего не было нажато - значит идем обрабатывать следующий кадр с веб. камеры
        } else if (key == 27) { // Esc - выключаем программу
            break;
//        } else if (useWebcam && key == TODO) {
//            // TODO если пользователь нажал кнопку 1 (как исследовать какой код у такой кнопки? опытным путем!) - текущий кадр надо использовать как imgForDetection
//            // ... = currentFrame.clone();
//        } else if (useWebcam && key == TODO) {
//            // TODO если пользователь нажал кнопку 2 - текущий кадр надо использовать как imgToDraw
//        } else if (key == TODO) {
//            // TODO если пользователь нажал кнопку H (Hide) - выключите/включите рисовать картинку поверх (т.е. drawOver)
//        } else if (key == TODO) {
//            // TODO если пользователь нажал кнопку 3 - включить/выключить отрисовку разной отладочной информации (т.е. drawDebug)
//        } else if (key == TODO) {
//            // TODO если пользователь нажал кнопку S (SIFT) - включить/выключить использование SIFT/ORB
        } else {
            std::cerr << "UKNOWN KEY " << key << " WAS PRESSED" << std::endl;
        }
    }
}


int main() {
    try {
        run();

        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}
