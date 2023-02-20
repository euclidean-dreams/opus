#include <zmq.hpp>
#include "percipient/CosmographerPercipient.h"
#include "framework/Constants.h"
#include "Opus.h"

namespace opus {

void bump_axiom(vec<float> &axioms, int index, Uint16 keymod) {
    if (keymod & KMOD_LSHIFT) {
        axioms[index] += 0.01;
    } else {
        axioms[index] += 0.1;
    }

    if (axioms[index] >= 1.0) {
        axioms[index] = 0.999;
    }
}

void blead_axiom(vec<float> &axioms, int index, Uint16 keymod) {
    if (keymod & KMOD_LSHIFT) {
        axioms[index] -= 0.01;
    } else {
        axioms[index] -= 0.1;
    }
    if (axioms[index] <= 0.0) {
        axioms[index] = 0.001;
    }
}

void send_phenomenon(NetworkSocket &socket, int identity) {
    auto builder = mkup<flatbuffers::FlatBufferBuilder>();
    auto phenomenon = CreatePhenomenon(*builder, identity, 1);
    builder->Finish(phenomenon);
    socket.sendParcel(Identifier::phenomenon, *builder);
}

int bootstrap(std::string configFilePath) {
    Bootstrapper bootstrapper(configFilePath, 1);
    Constants constants{};

    auto glimpsology = mksp<Arbiter<const Parcel>>();
    auto cosmographerSocket = mkup<NetworkSocket>(
            bootstrapper.getZmqContext(),
            constants.percipiaEndpoint,
            zmq::socket_type::sub,
            false
    );
    cosmographerSocket->setSubscriptionFilter(ImpresarioSerialization::Identifier::glimpse);
    auto cosmographerPercipient = mkup<CosmographerPercipient>(mv(cosmographerSocket), glimpsology);
    auto cosmographerPercipientThread = Circlet::begin(mv(cosmographerPercipient));

    auto opusInstance = mkup<Opus>(mv(glimpsology), constants);
    auto opusThread = Circlet::beginTicking(mv(opusInstance));

    auto socket = mkup<NetworkSocket>(
            bootstrapper.getZmqContext(),
            "tcp://0.0.0.0:44440",
            zmq::socket_type::pub,
            true
    );

    vec<float> axioms;
    axioms.push_back(0.9);
    axioms.push_back(0.7);
    axioms.push_back(0.3);
    axioms.push_back(0.9);
    axioms.push_back(0.5);
    axioms.push_back(0.1);
    axioms.push_back(0.25);
    axioms.push_back(0.25);
    axioms.push_back(0.3);
    axioms.push_back(0.2);

    SDL_Event event;
    bool running = true;
    while (running) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            // input
            auto symbol = event.key.keysym.sym;
            auto keymod = event.key.keysym.mod;
            if (symbol == SDLK_q) {
                bump_axiom(axioms, 0, keymod);
            } else if (symbol == SDLK_w) {
                bump_axiom(axioms, 1, keymod);
            } else if (symbol == SDLK_e) {
                bump_axiom(axioms, 2, keymod);
            } else if (symbol == SDLK_r) {
                bump_axiom(axioms, 3, keymod);
            } else if (symbol == SDLK_t) {
                bump_axiom(axioms, 4, keymod);
            } else if (symbol == SDLK_y) {
                bump_axiom(axioms, 5, keymod);
            } else if (symbol == SDLK_u) {
                bump_axiom(axioms, 6, keymod);
            } else if (symbol == SDLK_i) {
                bump_axiom(axioms, 7, keymod);
            } else if (symbol == SDLK_o) {
                bump_axiom(axioms, 8, keymod);
            } else if (symbol == SDLK_p) {
                bump_axiom(axioms, 9, keymod);

                // blead
            } else if (symbol == SDLK_a) {
                blead_axiom(axioms, 0, keymod);
            } else if (symbol == SDLK_s) {
                blead_axiom(axioms, 1, keymod);
            } else if (symbol == SDLK_d) {
                blead_axiom(axioms, 2, keymod);
            } else if (symbol == SDLK_f) {
                blead_axiom(axioms, 3, keymod);
            } else if (symbol == SDLK_g) {
                blead_axiom(axioms, 4, keymod);
            } else if (symbol == SDLK_h) {
                blead_axiom(axioms, 5, keymod);
            } else if (symbol == SDLK_j) {
                blead_axiom(axioms, 6, keymod);
            } else if (symbol == SDLK_k) {
                blead_axiom(axioms, 7, keymod);
            } else if (symbol == SDLK_l) {
                blead_axiom(axioms, 8, keymod);
            } else if (symbol == SDLK_SEMICOLON) {
                blead_axiom(axioms, 9, keymod);

                // micromode
            } else if (symbol == SDLK_1) {
                send_phenomenon(*socket, 0);
            } else if (symbol == SDLK_2) {
                send_phenomenon(*socket, 1);
            } else if (symbol == SDLK_3) {
                send_phenomenon(*socket, 2);
            } else if (symbol == SDLK_4) {
                send_phenomenon(*socket, 3);
            } else if (symbol == SDLK_5) {
                send_phenomenon(*socket, 4);

                // macromode
            } else if (symbol == SDLK_z) {
                send_phenomenon(*socket, 100);
            } else if (symbol == SDLK_x) {
                send_phenomenon(*socket, 101);
            } else if (symbol == SDLK_c) {
                send_phenomenon(*socket, 102);
            } else if (symbol == SDLK_v) {
                send_phenomenon(*socket, 103);
            } else if (symbol == SDLK_b) {
                send_phenomenon(*socket, 104);

                // extra
            } else if (symbol == SDLK_BACKSLASH) {
                for (float &axiom: axioms) {
                    axiom = 0.5;
                }
            } else if (symbol == SDLK_SLASH and SDLK_LSHIFT & event.key.keysym.mod) {
                send_phenomenon(*socket, 1000);
            }
            auto builder = mkup<flatbuffers::FlatBufferBuilder>();
            auto axiomsOffset = builder->CreateVector(axioms);
            auto axiomology = CreateAxiomology(*builder, axiomsOffset);
            builder->Finish(axiomology);
            socket->sendParcel(Identifier::axiomology, *builder);
        }
    }

    return 0;
}

}

int main(int argc, char *argv[]) {
    std::string configFilePath;
    if (argc == 1) {
        configFilePath = "./opus.yml";
    } else if (argc == 2) {
        configFilePath = argv[1];
    } else {
        return 1;
    }
    return opus::bootstrap(configFilePath);
}
