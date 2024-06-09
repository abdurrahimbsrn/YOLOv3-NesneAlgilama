#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>
#include <fstream>

// Fonksiyon tanımları
std::vector<std::string> getOutputsNames(const cv::dnn::Net& net);
void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame);

std::vector<std::string> classes;

int main() {
    // Mutlak dosya yollarını belirtin
    std::string modelConfiguration = "C:/Program Files/OpenCV/yolov3.cfg";
    std::string modelWeights = "C:/Program Files/OpenCV/yolov3.weights";
    std::string classesFile = "C:/Program Files/OpenCV/coco.names";

    // Sınıf isimlerini yükle
    std::ifstream ifs(classesFile.c_str());
    std::string line;
    while (std::getline(ifs, line)) {
        classes.push_back(line);
    }

    // YOLO ağını yükle
    cv::dnn::Net net = cv::dnn::readNetFromDarknet(modelConfiguration, modelWeights);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    // Giriş görüntüsünü yükle
    cv::Mat frame = cv::imread("C:/Users/Abdurrahim/Desktop/Fotoğraflar/images.png");

    // Giriş blob'u oluştur
    cv::Mat blob;
    cv::dnn::blobFromImage(frame, blob, 0.00392, cv::Size(416, 416), cv::Scalar(0, 0, 0), true, false);
    net.setInput(blob);

    // Algılamaları yap
    std::vector<cv::Mat> outs;
    net.forward(outs, getOutputsNames(net));

    // Algılamaları işleme
    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;
    for (size_t i = 0; i < outs.size(); ++i) {
        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols) {
            cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            cv::Point classIdPoint;
            double confidence;
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            if (confidence > 0.5) {
                int centerX = (int)(data[0] * frame.cols);
                int centerY = (int)(data[1] * frame.rows);
                int width = (int)(data[2] * frame.cols);
                int height = (int)(data[3] * frame.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;

                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)confidence);
                boxes.push_back(cv::Rect(left, top, width, height));
            }
        }
    }
    // Algılamaları yazdır
    for (size_t idx = 0; idx < boxes.size(); ++idx) {
        cv::Rect box = boxes[idx];
        std::cout << "Detected: " << classes[classIds[idx]] << " at ("
            << box.x << ", " << box.y << ", " << box.width << ", " << box.height << ")"
            << " with confidence: " << confidences[idx] << std::endl;
    }

    // Non-maximum suppression (NMS) uygulama
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, 0.5, 0.4, indices);

    // Sonuçları çizme
    for (size_t i = 0; i < indices.size(); ++i) {
        int idx = indices[i];
        cv::Rect box = boxes[idx];
        drawPred(classIds[idx], confidences[idx], box.x, box.y, box.x + box.width, box.y + box.height, frame);
    }

    // Görüntüyü göster
    cv::imshow("Image", frame);
    cv::waitKey(0);
    return 0;
}

// Çıktı katmanlarının isimlerini almak için yardımcı fonksiyon
std::vector<std::string> getOutputsNames(const cv::dnn::Net& net) {
    static std::vector<std::string> names;
    if (names.empty()) {
        std::vector<int> outLayers = net.getUnconnectedOutLayers();
        std::vector<std::string> layersNames = net.getLayerNames();
        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i)
            names[i] = layersNames[outLayers[i] - 1];
    }
    return names;
}

// Tahminleri çizmek için yardımcı fonksiyon
void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame) {
    // Kutu çizme
    cv::rectangle(frame, cv::Point(left, top), cv::Point(right, bottom), cv::Scalar(0, 255, 0));

    // Sınıf etiketi ve güven skoru
    std::string label = cv::format("%.2f", conf);
    if (!classes.empty()) {
        CV_Assert(classId < (int)classes.size());
        label = classes[classId] + ":" + label;
    }

    // Etiketin gösterimi
    int baseLine;
    cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    top = std::max(top, labelSize.height);
    cv::rectangle(frame, cv::Point(left, top - labelSize.height),
        cv::Point(left + labelSize.width, top + baseLine), cv::Scalar::all(255), cv::FILLED);
    cv::putText(frame, label, cv::Point(left, top), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(), 1);
}
