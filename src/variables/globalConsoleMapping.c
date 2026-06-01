#include "variables/globalConsoleMapping.h"

// Zero-initialised to MAP_INPUT_NONE; populated by input_mapping_set_defaults()
// (or input_mapping_load()) during init_all_system().
MapInput global_console_mapping[MAPPING_LABEL_COUNT] = {0};
