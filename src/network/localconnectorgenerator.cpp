#include "localconnectorgenerator.h"
// ==========================================================================================
// ==========================================================================================
//                                                                 GENERATE DEFAULT CONNECTOR
LocalConnectorInterface* LocalConnectorGenerator::getDefaultConnector()
{
    static LocalConnectorInterface *localConnector =
            new DefaultLocalConnector();
    return localConnector;
}
