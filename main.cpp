#include "src/json.h"

using namespace JSON;

int main()
{
    std::string filename("..\\examples\\gltf.json");
    JsonObject jsonFile = loadFile(filename);

    JsonObject buffers = jsonFile["bufferViews"];

    std::cout << "Buffer 0" << std::endl;
    std::cout << buffers[0]["buffer"] << std::endl;
    std::cout << buffers[0]["byteOffset"] << std::endl;
    std::cout << buffers[0]["byteLength"] << std::endl;
    std::cout << buffers[0]["target"] << "\n" << std::endl;

    std::cout << "Buffer 1" << std::endl;
    std::cout << buffers[1]["buffer"] << std::endl;
    std::cout << buffers[1]["byteOffset"] << std::endl;
    std::cout << buffers[1]["byteLength"] << std::endl;
    std::cout << buffers[1]["target"] << std::endl;

    return 0;
}
