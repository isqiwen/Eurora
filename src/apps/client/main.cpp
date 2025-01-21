#include "DataInputHandler.h"
#include "ISMRMRDProcessor.h"
#include "ServerConnector.h"
#include "Utilities.h"

int main(int argc, char** argv) {
    try {
        std::string host         = "localhost";
        std::string port         = "9002";
        std::string input_file   = "input.data";
        std::string output_file  = "output.h5";
        std::string input_format = "raw";  // 未来支持多种格式

        // 解析命令行参数（可扩展）
        if (argc > 1)
            host = argv[1];
        if (argc > 2)
            port = argv[2];
        if (argc > 3)
            input_file = argv[3];
        if (argc > 4)
            input_format = argv[4];

        // 初始化网络连接
        ServerConnector server(host, port);
        server.connect();

        // 加载数据并转换为 ISMRMRD 格式
        auto data = DataInputHandler::load(input_file, input_format);
        ISMRMRDProcessor processor(output_file);
        processor.process(data);

        // 发送数据到服务器
        server.send(processor.getProcessedData());
        server.close();

        log_info("Processing completed successfully.");
    } catch (const std::exception& e) {
        log_error("Error: " + std::string(e.what()));
        return -1;
    }

    return 0;
}
