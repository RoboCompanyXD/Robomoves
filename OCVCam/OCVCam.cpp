/**
 * File:   cameradet2.cpp
 * Author: ehu
 *
 * Created on 2 de marzo de 2020, 11:03
 */

#include "OCVCam.h"

/**
 * Class constructor
 */
OCVCam::OCVCam() {

    runCamThread = true;

}

/**
 * Class destructor
 */
OCVCam::~OCVCam() {
    runCamThread = false;
}

//int x, xp, y, yp, area, frame_width, frame_height, foto = 0; // TODO explicar qué es cada variable por separado

/*
 * Esta clase configura la cámara con el patrón y el espacio de color.
 * Una vez iniciada esta clase, el búcle principal empieza a ciclar y
 * con cada ciclo procesa el último fotograma disponible (no se procesan todos los fotogramas).
 *
 * TODO: documentar esta clase con un esquema/diagrama de secuencia
 *       - Explicar de manera general cuál es el proceso para obtener la posición del torso.
 *            1) Query last available fotogram
 *            2) Resize/scale the fotogram to reduce its pixel resolution, and therefore reduce computational cost
 *            3) ...
 *       - Explicar lo de la medición del tiempo
 *       - Explicar cómo utilizar los resultados de esta clase fuera de ella
 */
int OCVCam::AnalyzeCam() {

    //cout << "OpenCV version: "<< CV_VERSION << endl;
    //xp = 30;
    //yp = 45;

    clock_t start_t, end_t;
    start_t = clock();

    std::vector<int> compression_params;
    std::vector<cv::Rect> bodies;

    compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
    compression_params.push_back(90);

    cv::VideoCapture cap(0);
    cv::Mat frame, hsv_frame, mask, frame_gray, mask_grey, final_mask;
    int frames_nodetect = 0;

    cv::Moments mu;
    cv::Point2f mc;
    frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    //Mat mask_grey(frame_height,frame_width);
    mask_grey = cv::Mat::zeros(cv::Size(frame_width, frame_height), CV_8UC1);

    /*
     * Directorio en el que se guarda la captura
     */
    struct stat sb;
    if (!(stat("/tmp/robonitor", &sb) == 0 && S_ISDIR(sb.st_mode))) {
        const int dir_err = system("mkdir -p /tmp/robonitor");
        if (dir_err == -1) {
            printf("Error creating directory!n");
            exit(-1);
        }
    }

    /*
     * Rango del color a detectar
     */
    cv::Scalar lower_yellow = cv::Scalar(24, 135, 93, 0);
    cv::Scalar upper_yellow = cv::Scalar(42, 252, 255, 0);
    cv::Scalar lower_green = cv::Scalar(54, 145, 16, 0);
    cv::Scalar upper_green = cv::Scalar(100, 255, 128, 0);



    /*
     * Haar-cascade Detection
     */
    //bool fileAvailable = cv::CascadeClassifier.load("/home/pi/pedestrian-detection/haarcascade_upperbody.xml");
    //if (!fileAvailable) {
    //    perror("Could not load file: /home/pi/pedestrian-detection/haarcascade_upperbody.xml")
    //}
    cv::CascadeClassifier body_classifier;
    body_classifier = cv::CascadeClassifier("/home/pi/pedestrian-detection/haarcascade_upperbody.xml");

    while (runCamThread) {
        cap >> frame;

        /** Detección del color */

        cv::cvtColor(frame, hsv_frame, cv::COLOR_BGR2HSV); //Cambio del espacio de color
        cv::inRange(hsv_frame, lower_green, upper_green, mask); //Filtrado

        cv::erode(mask, mask, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2))); //Erosionar

        //        mu = moments(mask); // Calculo momentos y posicion
        //        if (mu.m00 > 30000) {
        //            mc = Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00);
        //            x = int(mc.x);
        //            y = int(mc.y);
        //            area = int(mu.m00);
        //        } else {
        //            mc = Point2f(0, 0);
        //            x = 0;
        //            y = 0;
        //            area = 0;
        //        }

        cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY); // cambio a grises
        cv::resize(frame_gray, frame_gray, cv::Size(), 0.25, 0.25, cv::INTER_LINEAR);

        /** Deteccion de upperbodies */

        body_classifier.detectMultiScale(frame_gray, bodies, 1.05, 3);

        /** Prueba para utilizar las 9 ultimos frames en el que se ha detectado persona */

        if (frames_nodetect > 9) {
            mask_grey = cv::Mat::zeros(cv::Size(frame_width, frame_height), CV_8UC1);
            isUserInView = false;
        }
        if (bodies.size() > 0) {
            frames_nodetect = 0;
            isUserInView = true;
            std::cout << "Vector size: " << bodies.size() << std::endl; // TODO: remove this
            for (int i = 0; i < bodies.size(); i++) {
                //cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 255), 2)
                cv::rectangle(mask_grey, cv::Point(bodies[i].x, bodies[i].y), cv::Point(bodies[i].x + bodies[i].width, bodies[i].y + bodies[i].height), cv::Scalar(255, 255, 255), cv::FILLED);
                //rectangle(frame,Point(bodies[i].x,bodies[i].y),Point(bodies[i].x+bodies[i].width,bodies[i].y+bodies[i].height),Scalar(0, 255, 255), 2);
                //rectangle(mask,Point(xp,yp),Point(xp+50,yp+100),Scalar(255, 255, 255), 2);
            }
        } else {
            frames_nodetect++;
        }

        /** Interseccion de deteccion por color y reconocimiento */

        cv::bitwise_and(mask, mask_grey, final_mask); //interseccion de deteccion por color y reconocimiento


        mu = cv::moments(final_mask); // Calculo momentos y posicion
        if (mu.m00 > 30000) {
            mc = cv::Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00);
            x = int(mc.x);
            y = int(mc.y);
            area = int(mu.m00);
        } else {
            mc = cv::Point2f(0, 0);
            x = 0;
            y = 0;
            area = 0;
        }

        //        imshow( "Mask", mask_grey );
        //        if( waitKey(10) == 27 )
        //        {
        //            break; // escape
        //        }
        //        imshow( "Detect", frame );
        //        if( waitKey(10) == 27 )
        //        {
        //            break; // escape
        //        }
        //        imshow( "Origi", final_mask );
        //        if( waitKey(10) == 27 )
        //        {
        //            break; // escape
        //        }

        end_t = clock();

        /*
         * Guardar captura cada segundo
         */
        //        if ((double)(end_t - start_t) / CLOCKS_PER_SEC > 1.0){
        //            start_t = end_t;
        //            system("touch /tmp/robonitor/.lock");
        //            imwrite("/tmp/robonitor/cap.jpg", frame, compression_params);
        //            system("unlink /tmp/robonitor/.lock");
        //        }

    } //end loop(1)
    return 0;
} //end int OCVCam(void)

void OCVCam::exitCamThread() {

    runCamThread = false;
}