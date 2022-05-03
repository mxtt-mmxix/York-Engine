/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2022 Matthew McCall
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//
// Created by Matthew McCall on 2/20/22.
//

#ifndef YORK_REGISTRY_HPP
#define YORK_REGISTRY_HPP

#include <vector>
#include <functional>

namespace york {

class EventHandler;
struct Event;

/**
 * A singleton class for sharing data between multiple instances of York-Engine.
 *
 * Currently, the Runtime links with York-Engine. On most modern operating systems, this results in the Engine library being loaded twice, with the Runtime and your Client have their own copies loaded. As a result these two copies of the Engine have their own memory allocated. Thus, internal data structures created in one copy will not be available in the other copy. As such, the `Registry` will contain the necessary data initialized by the Runtime for use in your client. You need to accept this `Registry` and pass it to the base constructor of the `Layer`.
 */
class Registry {
    friend EventHandler;
    friend void broadcastEvent(Event e);

public:
    Registry(const Registry&) = delete;

    /**
     * Please do not call this method in your client.
     *
     * The Registry will be passed in the createFunction.
     *
     * @return The Registry
     */
    static Registry& getInstance();

private:
    Registry() = default;
    std::vector<std::reference_wrapper<EventHandler>> m_eventHandlers;
};

extern "C" void registerRegistry(Registry& registry);

}

#endif // YORK_REGISTRY_HPP