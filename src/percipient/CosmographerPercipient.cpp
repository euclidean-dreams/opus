#include "CosmographerPercipient.h"

namespace opus {

CosmographerPercipient::CosmographerPercipient(std::unique_ptr<impresarioUtils::NetworkSocket> socket,
                                               std::shared_ptr<impresarioUtils::Arbiter<const impresarioUtils::Parcel>> glimpsology)
        : socket{move(socket)},
          glimpsology{move(glimpsology)} {

}

void CosmographerPercipient::activate() {
    auto parcel = socket->receiveParcel();
    if(!receivedFirstGlimpse) {
        LOGGER->info("received first glimpse!");
        receivedFirstGlimpse = true;
    }
    if (parcel->getIdentifier() == ImpresarioSerialization::Identifier::glimpse) {
        glimpsology->give(move(parcel));
    }
}

bool CosmographerPercipient::finished() {
    return false;
}

}
