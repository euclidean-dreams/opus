#include "Tchotchke.h"

namespace opus {

void Tchotchke::assertNotNull(void *entity) {
    if (entity == nullptr) {
        throw SDLFailure{};
    }
}

}
