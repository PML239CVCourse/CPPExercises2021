#include <filesystem>
#include <iostream>
#include <libutils/rasserts.h>

#include "sobel.h"
#include "disjoint_set.h"

#include <opencv2/highgui.hpp>

void testBGRToGray() {
    std::string name = "valve";
    cv::Mat img = cv::imread("lesson05/data/" + name + ".jpg");
    rassert(!img.empty(), 23981920813);

    // TODO реализуйте эту функцию, преобразующую картинку в черно-белую
    cv::Mat gray = convertBGRToGray(img);

    // TODO и удостоверьтесь что результат выглядит так как вы ожидаете, если нет - спросите меня
    std::string resultsDir = "lesson05/resultsData/";
    if (!std::filesystem::exists(resultsDir)) { // если папка еще не создана
        std::filesystem::create_directory(resultsDir); // то создаем ее
    }
    cv::imwrite("lesson05/resultsData/" + name + "_grey.jpg", gray);
}

void testSobel(const std::string &name) {
    cv::Mat img = cv::imread("lesson05/data/" + name + ".jpg");
    rassert(!img.empty(), 23981920813);

    // TODO реализуйте функцию считающую применение оператора Собеля к картинке
    // т.е. посчитайте производную по x и по y (в каждом пикселе хранятся две эти производные)
    cv::Mat dxy = sobelDXY(convertBGRToGray(img)); // обратите внимание что внутри ждут черно-белую картинку, значит нашу картинку надо перед Собелем преобразовать

    cv::Mat dx = convertDXYToDX(dxy); // TODO реализуйте функцию которая вытаскивает силу производной по x (ее абсолютное значение)
    // TODO и удостоверьтесь что результат выглядит так как вы ожидаете, если нет - спросите меня
    cv::imwrite("lesson05/resultsData/" + name + "_dx.jpg", dx);

    cv::Mat dy = convertDXYToDY(dxy); // TODO реализуйте функцию которая вытаскивает силу производной по y (ее абсолютное значение)
    // TODO и удостоверьтесь что результат выглядит так как вы ожидаете, если нет - спросите меня
    cv::imwrite("lesson05/resultsData/" + name + "_dy.jpg", dy);

    cv::Mat gradientStrength = convertDXYToGradientLength(dxy); // TODO реализуйте функцию которая считает силу градиента в каждом пикселе
//    // точнее - его длину, ведь градиент - это вектор (двухмерный, ведь у него две компоненты), а у вектора всегда есть длина - sqrt(x^2+y^2)
//    // TODO и удостоверьтесь что результат выглядит так как вы ожидаете, если нет - спросите меня
    cv::imwrite("lesson05/resultsData/" + name + "_gradientLength.jpg", gradientStrength);
//    // для valve.jpg должно быть похоже на картинку с википедии - https://ru.wikipedia.org/wiki/%D0%9E%D0%BF%D0%B5%D1%80%D0%B0%D1%82%D0%BE%D1%80_%D0%A1%D0%BE%D0%B1%D0%B5%D0%BB%D1%8F
}

struct MyVideoContent {
    //    MyVideoContent() {
    //
    //    }

    //    MyVideoContent(int a) : set(set) {
    //        DisjointSet set(a);
    //    }

    std::vector<std::vector<int>> pix;
    cv::Mat frame;
    cv::Mat ans;
    cv::Mat fon;
    cv::Mat frame1;
    cv::Mat mat;
    cv::Mat samdurak;
    cv::Mat nahnado;
    //    DisjointSet set;
    std::vector<std::vector<int>> mas;
    std::vector<std::vector<int>> vasya;
    std::vector<std::vector<int>> snm;
    int lastClickX = 0;
    int lastClickY = 0;
    int ret = 10, r = 10;
    bool kaka = false;
    bool pack = false, dil = false, er = false, diller = false, elrond = false;

    void Set(int a){
        switch (a) {
            case 3:
                dil = true, er = false, diller = false, elrond = false;
                break;
                case 4:
                    dil = false, er = true, diller = false, elrond = false;
                    break;
                    case 5:
                        dil = false, er = false, diller = true, elrond = false;
                        break;
                        case 6:
                            dil = false, er = false, diller = false, elrond = true;
                            break;
                            case 7:
                                dil = false, er = false, diller = false, elrond = false;
                                break;
                                default:
                                    rassert(true, "ПИЗДА")
        }
    }

    void Click(int x, int y){
        lastClickX = x;
        lastClickY = y;
    }

    void setF(cv::Mat qwerty){
        frame1 = qwerty;
    }

    void SetFon(cv::Mat fon1){
        fon = fon1;
    }

    cv::Mat Paint(std::vector<std::vector<int>> q){
        for (int i = 0; i < q.size(); ++i) {
            //            std::cout << q[i][0] << "   " << q[i][1] << std::endl;
            if (kaka) {
                frame.at<cv::Vec3b>(q[i][1], q[i][0]) = cv::Vec3b(255, 255, 0);
            }
            else{
                frame.at<cv::Vec3b>(q[i][1], q[i][0]) = cv::Vec3b(0, 0, 255);
            }
        }
        if (kaka){
            return invertImageColors(frame);
        }
        else{
            return frame;
        }
    }

    cv::Mat Paint1(){
        rassert(!frame1.empty(), 123);
        //        std::cout << pix.size();
        for (int i = 0; i < pix.size(); ++i) {
            if (kaka) {
                frame = rast1(frame.clone(),frame1.clone(),pix[i]);
            }
        }
        return frame;
    }

    cv::Mat Paint2(){
        rassert(!frame1.empty(), 123);
        rassert(!fon.empty(), 123);
        //        std::cout << pix.size();
        frame = rast2(frame,frame1,fon);
        return frame;
    }

    cv::Mat Paint3(){
        rassert(!frame1.empty(), 123);
        rassert(!fon.empty(), 123);
        rassert(mat.rows == frame.rows, "123431234314")
        rassert(mat.cols == frame.cols, "ПАРАПАПАРА2")
        //        cv::Vec3b color = frame.at<cv::Vec3b>(13, 5);
        //        cv::Vec3b colorf = fon.at<cv::Vec3b>(13, 5);
        //        for (int i = 0; i < mas.size(); ++i) {
        //            for (int j = 0; j < mas[i].size(); j++) {
        //                color = frame.at<cv::Vec3b>(j, i);
        //                colorf = fon.at<cv::Vec3b>(j, i);
        //                if (((int) color[0] < (int)colorf[0]+ret && (int) color[1] < (int)colorf[1]+ret && (int) color[2] < (int)colorf[2]+ret)&&((int) color[0] > (int)colorf[0]-ret && (int) color[1] > (int)colorf[1]-ret && (int) color[2] > (int)colorf[2]-ret)){
        //                    mas[i][j] = 1;
        //                }
        //            }
        //        }
        //        vasya = mas;
        //        ans = frame.clone();
        //        if (dil){
        //            mas = Dilate(mas, r);
        //        }
        //        if (er){
        //            mas = Erode(mas, r);
        //        }
        //        if (diller){
        //            mas = Dilate(mas, r);
        //            mas = Erode(mas, r);
        //        }
        //        if (elrond){
        //            mas = Erode(mas, r);
        //            mas = Dilate(mas, r);
        //        }
        //        for (int i = 0; i < mas.size(); ++i) {
        //            for (int j = 0; j < mas[i].size(); j++) {
        //                if (mas[i][j] == 1){
        //                    ans.at<cv::Vec3b>(j,i) = frame1.at<cv::Vec3b>(j,i);
        //                    mas[i][j] = 0;
        //                }
        //            }
        //        }
        //        std::cout << 1;
        //        std::cout << pix.size();
        //frame = rast2(frame,frame1,fon);
        return ans;
    }

    cv::Mat Paint4(){
        return samdurak;
    }
    void Dudu(DisjointSet set){
        cv::Vec3b color = frame.at<cv::Vec3b>(13, 5);
        cv::Vec3b colorf = fon.at<cv::Vec3b>(13, 5);
        for (int i = 0; i < mas.size(); ++i) {
            for (int j = 0; j < mas[i].size(); j++) {
                color = frame.at<cv::Vec3b>(j, i);
                colorf = fon.at<cv::Vec3b>(j, i);
                if (((int) color[0] < (int)colorf[0]+ret && (int) color[1] < (int)colorf[1]+ret && (int) color[2] < (int)colorf[2]+ret)&&((int) color[0] > (int)colorf[0]-ret && (int) color[1] > (int)colorf[1]-ret && (int) color[2] > (int)colorf[2]-ret)){
                    mas[i][j] = 1;
                }
            }
        }
        int n = 0;
        //        std::cout << (mas.size()-1)*(mas[0].size()-1) << std::endl;
        for (int i = 0; i < mas.size()-1; ++i) {
            for (int j = 0; j < mas[i].size()-1; j++) {
                n++;
                if (mas[i][j] == mas[i+1][j]){
                    set.union_sets(n,n+1);
                }
                if (mas[i][j] == mas[i][j+1]){
                    set.union_sets(n,n+mas[i].size()-1);
                }
            }
        }
        int w = 0, h = 0, o = mas[0][0];
        snm[0][0] = o;
        bool pidor = false;
        for (int i = 0; i < set.s; ++i) {
            h = i%mas[0].size();
            w = i/mas[0].size();
            //            if (set.get_set_size(i) < 20){
            //                int q = i;
            //                while (set.union_sets(i,q) == 1){
            //                    if (q >= set.s-1){
            //                        pidor = true;
            //                    }
            //                    if (q <= 0){
            //                        pidor = false;
            //                    }
            //                    if (!pidor){
            //                        q++;
            //                    }
            //                    else{
            //                        q--;
            //                    }
            //                }
            //            }
            int asdf = set.get_set(0), asnaropi = set.get_set(1);
            std::vector<int> asfdjiasdf = set.GetParents();
            if (i > 0){
                if (set.get_set(i) != set.get_set(i-1)){
                    if (o == 0){
                        o = 1;
                    }
                    else{
                        o = 0;
                    }
                }
                rassert(o == mas[w][h], i)
                //                snm[w][h] = o;
                color = frame.at<cv::Vec3b>(h, w);
                colorf = frame1.at<cv::Vec3b>(h, w);
                if (o == 0){
                    samdurak.at<cv::Vec3b>(h,w) = colorf;
                    nahnado.at<cv::Vec3b>(h,w) = cv::Vec3b(0, 0, 0);
                }
                else{
                    samdurak.at<cv::Vec3b>(h,w) = color;
                    nahnado.at<cv::Vec3b>(h,w) = cv::Vec3b(255, 255, 255);
                }
            }
        }
        vasya = mas;
        ans = frame.clone();
        if (dil){
            mas = Dilate(mas, r);
        }
        if (er){
            mas = Erode(mas, r);
        }
        if (diller){
            mas = Dilate(mas, r);
            mas = Erode(mas, r);
        }
        if (elrond){
            mas = Erode(mas, r);
            mas = Dilate(mas, r);
        }
        //        std::cout << 1;
        for (int i = 0; i < mas.size(); ++i) {
            for (int j = 0; j < mas[i].size(); j++) {
                if (mas[i][j] == 1){
                    ans.at<cv::Vec3b>(j,i) = frame1.at<cv::Vec3b>(j,i);
                    mas[i][j] = 0;
                    //                    snm[i][j] = j%mat.rows + i * mat.cols;
                }
            }
        }
    }

    void Mat(){
        std::vector<int> q;
        q.resize(mat.rows);
        //        std::cout << q.size() <<  std::endl;
        for (int i = 0; i < mat.cols; ++i) {
            for (int j = 0; j < mat.rows; j++) {
                q[j] = 0;
            }
            mas.push_back(q);
        }
        //        std::cout << mas.size() << " _ " << mat.cols << " _ " << frame.cols <<  std::endl;
    }

    std::vector<int> Get(){
        std::vector<int> a;
        //        std::cout << "Left click at x=" << lastClickX << ", y=" << lastClickY << std::endl;
        a.push_back(lastClickX);
        a.push_back(lastClickY);
        return a;
    }

    void Dpix(std::vector<int> a){
        pix.push_back(a);
    }

    void Dokaka(){
        if (kaka){
            kaka = false;
        }
        else{
            kaka = true;
        }
    }

    cv::Mat Vis(){
        bool a = false;
        for (int i = 0; i < mat.cols; ++i) {
            for (int j = 0; j < mat.rows; j++) {
                //                std::cout << mas[i][j] << "   ___PIZDA___   " << i << " " << j << " ";
                if (vasya[i][j] == 1){
                    a = true;
                    mat.at<cv::Vec3b>(j,i) = cv::Vec3b(255, 255, 255);
                }
                else{
                    mat.at<cv::Vec3b>(j,i) = cv::Vec3b(0, 0, 0);
                }
            }
            //            std::cout << std::endl;
        }
        if (a){
            std::cout << 1;
        }
        return mat;
    }

    //    void Setset() {
    //        set = DisjointSet(frame.rows * frame.cols);
    //    }
};

void onMouseClick(int event, int x, int y, int flags, void *pointerToMyVideoContent) {
    MyVideoContent &content = *((MyVideoContent*) pointerToMyVideoContent);
    // не обращайте внимание на предыдущую строку, главное что важно заметить:
    // content.frame - доступ к тому кадру что был только что отображен на экране
    // content.lastClickX - переменная которая вам тоже наверняка пригодится
    // вы можете добавить своих переменных в структурку выше (считайте что это описание объекта из ООП, т.к. почти полноценный класс)

    cv::Vec3b color;
    std::vector<int> b;
    b.push_back(0);
    b.push_back(0);
    b.push_back(0);
    if (event == cv::EVENT_LBUTTONDOWN) { // если нажата левая кнопка мыши
        //        std::cout << "Left click at x=" << x << ", y=" << y << std::endl;
        content.pack = true;
        std::vector<int> a = content.Get();
        color = content.frame.at<cv::Vec3b>(a[1],a[0]);
        b[0] = (int)color[0];
        b[1] = (int)color[1];
        b[2] = (int)color[2];
        //        std::cout << b[0] << " " << b[1] << " " << b[2] << std::endl;
        content.Dpix(b);
        content.Click(x,y);
    }

    if (event == cv::EVENT_RBUTTONDOWN){
        content.Dokaka();
    }
}

void Cam(){
    bool big = false, small = false, a = false, b = false;
    cv::VideoCapture video(0);
    rassert(video.isOpened(), 3423948392481);

    MyVideoContent content;
    std::vector<std::vector<int>> mas;
    cv::Mat Foto;
    DisjointSet set(1);
    int q = 0;
    bool isSuccess = false;
    while (true) {
        isSuccess = video.read(content.frame);
        cv::imshow("video", content.frame);
        int k = cv::waitKey(10);
        if (k == 32 || k == 27) {
            content.SetFon(content.frame.clone());
            break;
        }
    }
    while (true) {
        isSuccess = video.read(content.frame);

        rassert(isSuccess, 348792347819);
        rassert(!content.frame.empty(), 3452314124643);

        if (q == 0) {
            content.setF(baba(cv::imread("lesson03/data/castle_large.jpg"), content.frame.clone()));
            q++;
            content.mat = content.frame.clone();
            content.Mat();
            content.snm = content.mas;
            for (int i = 0; i < content.mat.cols; ++i) {
                for (int j = 0; j < content.mat.rows; j++) {
                    content.snm[i][j] = j%content.mat.rows + i * content.mat.cols;
                }
            }
            content.samdurak = content.frame.clone();
            content.nahnado = content.frame.clone();
            //            content.Setset();
            DisjointSet set1((content.frame.rows)*(content.frame.cols));
            set = set1;
        }
        std::cout << set.s << std::endl;
        rassert(set.get_set_size(50) == 1, 123125125);
        content.Dudu(set);

        if (big) {
            cv::imshow("video", content.frame1); // покаызваем очередной кадр в окошке
        } else if (small) {
            cv::imshow("video", content.Paint2()); // покаызваем очередной кадр в окошке
        } else if (a) {
            cv::imshow("video", content.Paint3()); // покаызваем очередной кадр в окошке
        } else if (b) {
            cv::imshow("video", content.Paint4()); // покаызваем очередной кадр в окошке
        }else {
            cv::imshow("video", content.Paint1()); // покаызваем очередной кадр в окошке
        }
        cv::setMouseCallback("video", onMouseClick, &content); // делаем так чтобы функция выше (onMouseClick) получала оповещение при каждом клике мышкой
        int key = cv::waitKey(10);
        std::cout << key << std::endl;
        std::string resultsDir = "lesson04/resultsData/";
        if (key == 0) {
            if (big) {
                big = false;
            } else {
                big = true;
            }
        }else if (key == 32){
            if (!std::filesystem::exists(resultsDir)) { // если папка еще не создана
                std::filesystem::create_directory(resultsDir); // то создаем ее
            }
            cv::imwrite(resultsDir + "01_Fon.jpg", content.fon);
            cv::imwrite(resultsDir + "02_Cum.jpg", content.frame);
            cv::imwrite(resultsDir + "03_Mask.jpg", content.Vis());
            content.vasya = Dilate(content.vasya, content.r);
            content.vasya = Erode(content.vasya, content.r);
            cv::imwrite(resultsDir + "04_Eelon.jpg", content.Vis());
            cv::imwrite(resultsDir + "05_Besit.jpg", content.nahnado);
        }else if (key == 49) {
            if (small) {
                small = false;
            } else {
                small = true;
            }
        } else if (key == 50) {
            if (a) {
                a = false;
            } else {
                a = true;
            }
        } else if (key > 50 && key < 56) {
            content.Set(key - 48);
        } else if (key == 61) {
            content.r++;
        } else if (key == 45) {
            content.r--;
        } else if (key == 48) {
            content.ret++;
        } else if (key == 57) {
            content.ret--;
        }else if (key == 122) {
            if (b) {
                b = false;
            } else {
                b = true;
            }
        }
        if (key == 27) {
            break;
        }
    }
}

int main() {
    try {
        testBGRToGray();

        for (int i = 1; i <= 4; ++i) {
            testSobel("line0" + std::to_string(i));
        }

        for (int i = 11; i <= 14; ++i) {
            // TODO сделайте вызов тестирования картинок line11.jpg - line14.jpg
            testSobel("line" + std::to_string(i));
        }

        testSobel("line21_water_horizont");
        testSobel("multiline1_paper_on_table");
        testSobel("multiline2_paper_on_table");

        testSobel("valve");
//        Cam();
        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}