#ifndef LOCALCONNECTORGENERATOR_H
#define LOCALCONNECTORGENERATOR_H

#include "localconnector.h"

class LocalConnectorGenerator
{
    LocalConnectorGenerator(); // Private!

public:

    LocalConnectorGenerator(LocalConnectorGenerator const&)            = delete;
    LocalConnectorGenerator& operator=(LocalConnectorGenerator const&) = delete;

    static LocalConnectorInterface * getDefaultConnector();
};

#endif // LOCALCONNECTORGENERATOR_H
