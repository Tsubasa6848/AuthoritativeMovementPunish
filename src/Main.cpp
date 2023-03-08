#include <LoggerAPI.h>
#include "version.h"
#include <ServerAPI.h>

extern Logger logger;

void PluginInit()
{
    logger.setFile("logs/AuthoritativeMovementPunish.log"); 
    Logger(PLUGIN_NAME).info("Loaded Version 0.1.0");
    Logger(PLUGIN_NAME).info("Author: Tsubasa6848");
}
