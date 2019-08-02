#ifndef LOCALCONNECTORGENERATOR_H
#define LOCALCONNECTORGENERATOR_H

#include "defaultlocalconnector.h"

// ==========================================================================================
//                                                                        CONNECTOR GENERATOR
// ==========================================================================================
class LocalConnectorGenerator
{
    LocalConnectorGenerator(); // Private!

public:

    LocalConnectorGenerator(LocalConnectorGenerator const&)            = delete;
    LocalConnectorGenerator& operator=(LocalConnectorGenerator const&) = delete;

    // Genrates default connector
    static LocalConnectorInterface * getDefaultConnector();
};

#endif // LOCALCONNECTORGENERATOR_H
