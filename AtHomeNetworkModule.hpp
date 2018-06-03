#ifndef ATHOMENETWORKMODULE_HPP
# define ATHOMENETWORKMODULE_HPP
# include "AtHomeConfig.h"
# if !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK)
#  include <stdio.h>
#  include "AtHomeModule.hpp"
#  include "AtHomeFlashCommon.h"

namespace athome {
    namespace module {
        template <typename T, size_t n>
        class AtHomeNetworkModule : public AtHomeModule<T, n> {
        friend class AtHomeModule<T, n>;
        public:
            AtHomeNetworkModule(const AtHomeNetworkModule &) = delete;
            AtHomeNetworkModule &operator=(const AtHomeNetworkModule &) = delete;
            ~AtHomeNetworkModule() {}

            virtual void setCommandPlugin(AtHomeCommandPlugin plugin) {
                _networkCommandCallback = plugin;
            }

            virtual void setOnBackupPlugin(AtHomeStoragePlugin plugin) {
                _networkOnBackupCallback = plugin;
            }

            virtual void setOnRestorePlugin(AtHomeStoragePlugin plugin) {
                _networkOnRestoreCallback = plugin;
            }

        protected:
            AtHomeNetworkModule():
                    _networkCommandCallback(nullptr),
                    _networkOnBackupCallback(nullptr),
                    _networkOnRestoreCallback(nullptr),
                    AtHomeModule<T, n>() {
                memset(&_host, 0, sizeof(_host));
            }

        private:
            void setEndPointCommand(Stream &communicator) {
                int version = communicator.read();
                if (version == 4) {
                    for (uint8_t i = 0; i < 4; i++) {
                        _host.ipv4[i] = communicator.read();
                    }
                }
                else if (version == 6) {
                    for (uint8_t i = 0; i < 16; i++) {
                        _host.ipv6[i] = communicator.read();
                    }
                }
                else {
                    while (communicator.read() != communication::commands::end_of_command);
                    return;
                }
                _host.hport = communicator.read();
                _host.hport |= (communicator.read() << 8);
                while (communicator.read() != communication::commands::end_of_command);
            }

        private:
            AtHomeCommandPlugin         _networkCommandCallback;
            AtHomeStoragePlugin         _networkOnBackupCallback;
            AtHomeStoragePlugin         _networkOnRestoreCallback;

        protected:
            communication::ip::tcp_host _host;
        };
    }
}

# endif /* !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) */
#endif /* ATHOMENETWORKMODULE_HPP */