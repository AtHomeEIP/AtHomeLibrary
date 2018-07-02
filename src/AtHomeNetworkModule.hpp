#ifndef ATHOMENETWORKMODULE_HPP
# define ATHOMENETWORKMODULE_HPP
# include "AtHomeConfig.h"
# if !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK)
#  include <stdio.h>
#  include "AtHomeModule.hpp"
#  include "AtHomeFlashCommon.h"
#  include "ANetworkCommunicator.hpp"

namespace athome {
    namespace module {
        template <typename T, size_t n>
        class AtHomeNetworkModule : public AtHomeModule<T, n> {
        friend class AtHomeModule<T, n>;
        public:
            AtHomeNetworkModule(const AtHomeNetworkModule &) = delete;
            AtHomeNetworkModule &operator=(const AtHomeNetworkModule &) = delete;
            ~AtHomeNetworkModule() {}

            void setNetworkCommunicator(communication::ANetworkCommunicator &communicator) {
                if (_communicator != nullptr) {
                    _communicator->disconnectFromHost();
                }
                _communicator = &communicator;
#  ifndef DISABLE_PERSISTENT_STORAGE
                this->onRestoreFromStorage();
#  endif /* DISABLE_PERSISTENT_STORAGE */
                if (AtHomeModule<T, n>::getSerial()) {
                    _communicator->connectToHost();
                }
            }

            void setHost(const communication::ip::tcp_host &host) {
                if (_communicator != nullptr) {
                    _communicator->disconnectFromHost();
                }
                _communicator->setHost(host);
#  ifndef DISABLE_PERSISTENT_STORAGE
                this->onBackupOnStorage();
#  endif /* DISABLE_PERSISTENT_STORAGE */
                if (!_communicator->connectToHost()) {
                    _updateStreams();
                }
            }

        protected:
            AtHomeNetworkModule():
                    _communicator(nullptr),
                    AtHomeModule<T, n>() {
                AtHomeModule<T, n>::setCommandPlugin(_networkCommands);
                //AtHomeModule<T, n>::setCommandPlugin(&AtHomeNetworkModule::_onCommandReceivedNetwork);
#  ifndef DISABLE_PERSISTENT_STORAGE
                AtHomeModule<T, n>::setOnBackupPlugin(_onNetworkBackup);
                AtHomeModule<T, n>::setOnRestorePlugin(_onNetworkRestore);
#  endif /* DISABLE_PERSISTENT_STORAGE */
            }
#  ifndef DISABLE_PERSISTENT_STORAGE
            size_t onNetworkBackup(size_t offset, storage::IStorage &storage) {
                if (_communicator == nullptr) {
                    return;
                }
                communication::ip::tcp_host host = _communicator->getHost();
                size_t delta = 0;
                storage.write(offset, reinterpret_cast<const void *>(host.ipv6), sizeof(host.ipv6));
                delta += sizeof(host.ipv6);
                storage.write(offset + delta, reinterpret_cast<const void *>(&(host.hport)), sizeof(host.hport));
                delta += sizeof(host.hport);
                return delta;
            }

            size_t onNetworkRestore(size_t offset, storage::IStorage &storage) {
                if (_communicator == nullptr) {
                    return;
                }
                communication::ip::tcp_host host;
                size_t delta = 0;
                storage.read(offset, reinterpret_cast<void *>(host.ipv6), sizeof(host.ipv6));
                delta += sizeof(host.ipv6);
                storage.read(offset + delta, reinterpret_cast<void *>(&(host.hport)), sizeof(host.hport));
                delta += sizeof(host.hport);
                _communicator->setHost(host);
                return delta;
            }
#  endif /* DISABLE_PERSISTENT_STORAGE */

        private:
            void setEndPoint(Stream &communicator) {
                communication::ip::tcp_host host;
                char version;
                if (communicator.readBytes(&version, 1) < 1) {
                    return;
                }
                if (version == 4) {
                    communicator.readBytes(reinterpret_cast<char *>(host.ipv4), sizeof(host.ipv4));
                }
                else if (version == 6) {
                    communicator.readBytes(reinterpret_cast<char *>(host.ipv6), sizeof(host.ipv6));
                }
                else {
                    return;
                }
                if (communicator.readBytes(reinterpret_cast<char *>(&host.hport), sizeof(host.hport)) < 1) {
                    return;
                }
                setHost(host);
            }

            static void _setEndPoint(const char *command, Stream &stream) {
                AtHomeNetworkModule *instance = reinterpret_cast<AtHomeNetworkModule *>(AtHomeModule<T, n>::getInstance());
                if (instance != nullptr) {
                    return instance->setEndPoint(stream);
                }
            }

            void _updateStreams() {
                if (ABaseModule::_streams == nullptr) {
                    ABaseModule::_streams = new Stream*[2];
                    ABaseModule::_streams[0] = _communicator;
                    ABaseModule::_streams[1] = nullptr;
                }
                else {
                    size_t i;
                    for (i = 0; ABaseModule::_streams[i] != nullptr; i++) {
                        if (ABaseModule::_streams[i] == _communicator) {
                            return;
                        }
                    }
                    ABaseModule::_streams[i] = _communicator; // Will expect to have at least 2 trailing nullptr to have allocated space
                }
            }

        private:
#  ifndef DISABLE_PERSISTENT_STORAGE
            static size_t _onNetworkBackup(size_t offset, storage::IStorage &storage) {
                AtHomeNetworkModule *instance = reinterpret_cast<AtHomeNetworkModule *>(AtHomeModule<T, n>::getInstance());
                if (instance != nullptr) {
                    return instance->onNetworkBackup(offset, storage);
                }
                return 0;
            }

            static size_t _onNetworkRestore(size_t offset, storage::IStorage &storage) {
                AtHomeNetworkModule *instance = reinterpret_cast<AtHomeNetworkModule *>(AtHomeModule<T, n>::getInstance());
                if (instance != nullptr) {
                    return instance->onNetworkRestore(offset, storage);
                }
                return 0;
            }
#  endif /* DISABLE_PERSISTENT_STORAGE */
        protected:
            communication::ANetworkCommunicator             *_communicator;

        private:
            static const Command                            _setEndPointCommand;
            static const CommandTable                       _networkCommands;
        };

        template <typename T, size_t n>
        const Command AtHomeNetworkModule<T, n>::_setEndPointCommand = { communication::commands::setEndPoint,
                                                                         AtHomeNetworkModule<T, n>::_setEndPoint };

        template <typename T, size_t n>
        const CommandTable AtHomeNetworkModule<T, n>::_networkCommands = { &AtHomeNetworkModule<T, n>::_setEndPointCommand,
                                                                           nullptr };
    }
}

# endif /* !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) */
#endif /* ATHOMENETWORKMODULE_HPP */