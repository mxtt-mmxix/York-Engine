#include <cstdlib>
#include <vector>

#include <SDL.h>
#undef main

#include <xercesc/util/PlatformUtils.hpp>

#include "curlpp/cURLpp.hpp"

#include "york/Async.hpp"
#include "york/Config.hpp"
#include "york/Event.hpp"
#include "york/Log.hpp"
#include "york/StopWatch.hpp"
#include "york/XML/Document.hpp"

#include "LayerLoader.hpp"
#include "LayerStack.hpp"

int main()
{
    york::StopWatch stopWatch;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        YORK_CORE_ERROR(SDL_GetError());
        return EXIT_FAILURE;
    }

    try {
        xercesc::XMLPlatformUtils::Initialize();
    } catch (const xercesc::XMLException& e) {
        YORK_CORE_CRITICAL("Failed to load XML library!")
        return EXIT_FAILURE;
    }

    auto* clientXML = new york::xml::Document { "Client.xml" };
    york::xml::Tag& clientTag = *clientXML->getRootTag();
    york::xml::Tag* layersTag;

    std::vector<LayerLoader> layerLoaders;

    for (auto& tag : clientTag) {
        if (tag.getName() == "Layers") {
            layersTag = &tag;
        }
    }

    for (auto& tag: *layersTag) {
        if (tag.getName() == "Layer") {
            std::string layerName, createFunctionName;

            for (auto& layerChildTag: tag) {
                if (layerChildTag.getName() == "Name") {
                    layerName = layerChildTag.getValue();
                }
                if (layerChildTag.getName() == "CreateFunction") {
                    createFunctionName = layerChildTag.getValue();
                }
            }

            if (!createFunctionName.empty()) {
                layerLoaders.emplace_back(layerName, createFunctionName);
                continue;
            }

            layerLoaders.emplace_back(layerName);

        }
    }


    york::LayerStack layerStack;

    for (auto& layerLoader: layerLoaders) {
        layerStack.pushLayer(*layerLoader);
    }

    curlpp::initialize();

    YORK_CORE_INFO("Initialization took {} seconds on {}!", stopWatch.getTime(), YORK_PLATFORM);

    stopWatch.reset();

    try {
        while (!layerStack.empty()) {

            SDL_Event event;

            while (SDL_PollEvent(&event)) {
                york::broadcastEvent(york::Event { event });
            }

            york::flushEventQueue();

            for (york::Layer& layer : layerStack) {

                if (layer.getExit()) {
                    layerStack.popLayer(layer);
                    continue;
                }

                layer.onUpdate(stopWatch.getTime());
                layer.onRender();
            }
        }

        stopWatch.reset();
    } catch (std::exception& e) {
        YORK_CORE_CRITICAL(e.what());
    }

    layerLoaders.clear();

    york::async::getExecutor().wait_for_all();

    curlpp::terminate();

    delete clientXML;
    xercesc::XMLPlatformUtils::Terminate();

    SDL_Quit();

    YORK_CORE_INFO("Shutdown successfully!");
    return EXIT_SUCCESS;
}