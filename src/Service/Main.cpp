// Crow
#include <crow.h>

// OpenCV
#include <opencv2/opencv.hpp>

// Stdlib
#include <string>
#include <vector>

// Detection parameters
const float INPUT_WIDTH = 640.0;
const float INPUT_HEIGHT = 640.0;
const float SCORE_THRESHOLD = 0.2;
const float NMS_THRESHOLD = 0.4;
const float CONFIDENCE_THRESHOLD = 0.4;

// File locations
const std::string RESOURCES_PATH = "resources";
const std::string CLASSES_FILE_PATH = "resources/classes.txt";
const std::string NETWORK_FILE_PATH = "resources/yolov5s.onnx";

struct Detection {
  int Class;
  float Confidence;
  cv::Rect BoundingBox;
};

auto generateRandomColors(int n) -> std::vector<cv::Scalar> {
  std::vector<cv::Scalar> ret(n);

  srand(time(0));

  for (auto &color : ret) {
    color = cv::Scalar{double(128 + rand() % 128), double(128 + rand() % 128),
                       double(128 + rand() % 128)};
  }

  return ret;
}

auto loadClasses() -> std::vector<std::string> {
  // Load classes text file into returned collection
  std::vector<std::string> ret;
  std::ifstream ifs(CLASSES_FILE_PATH);
  for (std::string line; getline(ifs, line); ret.push_back(line))
    ;
  return ret;
}

inline auto loadNetwork() -> cv::dnn::Net {
  // Load the network
  auto result = cv::dnn::readNet(NETWORK_FILE_PATH);

  result.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
  result.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);

  return result;
}

auto detect(const cv::Mat &in, cv::dnn::Net &net,
            const std::vector<std::string> &className) noexcept
    -> std::vector<Detection> {
  std::vector<Detection> ret;

  // Convert to blob
  cv::Mat blob;
  cv::dnn::blobFromImage(in, blob, 1.0 / 255.0,
                         cv::Size(INPUT_WIDTH, INPUT_HEIGHT), cv::Scalar(),
                         true, false);
  net.setInput(blob);

  // Do inference
  std::vector<cv::Mat> outputs;
  net.forward(outputs, net.getUnconnectedOutLayersNames());

  float x_factor = in.cols / INPUT_WIDTH;
  float y_factor = in.rows / INPUT_HEIGHT;

  // Output data
  float *data = (float *)outputs[0].data;

  const int dimensions = 85;
  const int rows = 25200;

  std::vector<int> classes;
  std::vector<float> confidences;
  std::vector<cv::Rect> boxes;

  // Decode output
  for (int i = 0; i < rows; ++i, data += 85) {
    float confidence = data[4];
    if (confidence >= CONFIDENCE_THRESHOLD) {

      float *classes_scores = data + 5;
      cv::Mat scores(1, className.size(), CV_32FC1, classes_scores);
      cv::Point classId;

      double maxClassScore;
      cv::minMaxLoc(scores, 0, &maxClassScore, 0, &classId);

      if (maxClassScore > SCORE_THRESHOLD) {
        confidences.push_back(confidence);
        classes.push_back(classId.x);

        float x = data[0];
        float y = data[1];
        float w = data[2];
        float h = data[3];

        boxes.push_back(cv::Rect(int((x - 0.5 * w) * x_factor), // Left
                                 int((y - 0.5 * h) * y_factor), // Top
                                 int(w * x_factor),             // Width
                                 int(h * y_factor))             // Height
        );
      }
    }
  }

  // Do non maximum supression
  std::vector<int> nmsResult;
  cv::dnn::NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD,
                    nmsResult);

  for (const auto &index : nmsResult) {
    ret.push_back(Detection{classes[index], confidences[index], boxes[index]});
  }

  return ret;
}

void drawDetections(cv::Mat &inOut, const std::vector<Detection> &detections,
                    const std::vector<std::string> &classes) {
  const static auto colors = generateRandomColors(classes.size());

  for (const auto &detection : detections) {
    const auto box = detection.BoundingBox;
    const auto classId = detection.Class;

    const auto color = colors[classId];

    cv::rectangle(inOut, box, color, 3);

    cv::rectangle(inOut, cv::Point(box.x, box.y - 20),
                  cv::Point(box.x + box.width, box.y), color, cv::FILLED);

    cv::putText(inOut, classes[classId].c_str(), cv::Point(box.x, box.y - 5),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
  }
}

int main(int, char *[]) {
  crow::SimpleApp app;

#ifndef NDEBUG
  app.loglevel(crow::LogLevel::Debug);
#endif // NDEBUG

  // Load classes
  std::vector<std::string> classes;
  try {
    classes = loadClasses();
  } catch (const std::runtime_error &e) {
    CROW_LOG_CRITICAL << "Failed to load classes: " << e.what();
    return -2;
  }
  CROW_LOG_INFO << "Loaded " << classes.size() << " classes";

  // Load network
  cv::dnn::Net net;
  try {
    net = loadNetwork();
  } catch (const std::runtime_error &e) {
    CROW_LOG_CRITICAL << "Failed to load network: " << e.what();
    return -3;
  }
  CROW_LOG_INFO << "Loaded network";

  // Process (POST) endpoint
  CROW_ROUTE(app, "/process").methods(
      "POST"_method)([&](const crow::request &req, crow::response &res) {
    if (!req.body.empty()) {
      // Attempt to load the image
      cv::Mat image;
      cv::Mat rawData(1, req.body.size(), CV_8U, (void *)req.body.data());

      image = cv::imdecode(rawData, cv::IMREAD_COLOR);

      if (!image.empty()) {
        // Perform detection
        const auto detections = detect(image, net, classes);

        // Either return the image with detections drawn on top
        if (req.url_params.get("image") != nullptr) {
          drawDetections(image, detections, classes);

          // Encode image
          std::vector<uchar> encodedResult;
          cv::imencode(".jpg", image, encodedResult);

          res.code = 200;
          res.set_header("Content-Type", "image/jpeg");
          res.body = std::string(encodedResult.begin(), encodedResult.end());
        }
        // Or return the detections
        else {
          crow::json::wvalue json;

          crow::json::wvalue::list detectionsJson;

          for (const auto &detection : detections) {
            detectionsJson.push_back({{"class", classes[detection.Class]},
                                      {"confidence", detection.Confidence}});
          }

          json["detections"] = crow::json::wvalue{detectionsJson};

          res.code = 200;
          res.set_header("Content-Type", "application/json");
          res.body = json.dump();
        }
      } else {
        res.code = 400;
        res.write("Failed to decode the image");
      }
    } else {
      res.code = 400;
      res.write("No image data in the request body");
    }
    res.end();
  });

  app.port(8001).multithreaded().run();

  return 0;
}
