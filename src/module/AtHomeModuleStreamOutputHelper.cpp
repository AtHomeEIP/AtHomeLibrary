#include "AtHomeModuleStreamOutputHelper.hpp"

#ifndef DISABLE_ON_WRITE_CALLBACKS
using athome::module::AtHomeModuleStreamOutputHelper;

AtHomeModuleStreamOutputHelper *AtHomeModuleStreamOutputHelper::_streamOutputHelperInstance = nullptr;
#endif /* DISABLE_ON_WRITE_CALLBACKS */
