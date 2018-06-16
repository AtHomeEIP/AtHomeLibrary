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

            virtual void setCommandPlugin(typename AtHomeModule<T, n>::AtHomeCommandPlugin plugin) {
                _networkCommandCallback = plugin;
            }
#  ifndef DISABLE_PERSISTENT_STORAGE
            virtual void setOnBackupPlugin(typename AtHomeModule<T, n>::AtHomeStoragePlugin plugin) {
                _networkOnBackupCallback = plugin;
            }

            virtual void setOnRestorePlugin(typename AtHomeModule<T, n>::AtHomeStoragePlugin plugin) {
                _networkOnRestoreCallback = plugin;
            }
#  endif /* DISABLE_PERSISTENT_STORAGE */
            void setNetworkCommunicator(communication::ANetworkCommunicator &communicator) {
                if (_communicator != nullptr) {
                    _communicator->disconnectFromHost();
                }
                _communicator = &communicator;
#  ifndef DISABLE_PERSISTENT_STORAGE
                this->onRestoreFromStorage();
#  endif /* DISABLE_PERSISTENT_STORAGE */
                _communicator->connectToHost();
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
                    _networkCommandCallback(nullptr),
#  ifndef DISABLE_PERSISTENT_STORAGE
                    _networkOnBackupCallback(nullptr),
                    _networkOnRestoreCallback(nullptr),
#  endif /* DISABLE_PERSISTENT_STORAGE */
                    _communicator(nullptr),
                    AtHomeModule<T, n>() {
                AtHomeModule<T, n>::setCommandPlugin(&AtHomeNetworkModule::_onCommandReceivedNetwork);
#  ifndef DISABLE_PERSISTENT_STORAGE
                AtHomeModule<T, n>::setOnBackupPlugin(&AtHomeNetworkModule::_onNetworkBackup);
                AtHomeModule<T, n>::setOnRestorePlugin(&AtHomeNetworkModule::_onNetworkRestore);
#  endif /* DISABLE_PERSISTENT_STORAGE */
            }
#  ifndef DISABLE_PERSISTENT_STORAGE
            void onNetworkBackup(size_t offset, storage::IStorage &storage) {
                if (_communicator == nullptr) {
                    if (_networkOnBackupCallback != nullptr) {
                        _networkOnBackupCallback(offset, storage);
                    }
                    return;
                }
                communication::ip::tcp_host host = _communicator->getHost();
                storage.write(offset, reinterpret_cast<const void *>(host.ipv6), sizeof(host.ipv6));
                offset += sizeof(host.ipv6);
                storage.write(offset, reinterpret_cast<const void *>(&(host.hport)), sizeof(host.hport));
                offset += sizeof(host.hport);
                if (_networkOnBackupCallback != nullptr) {
                    _networkOnBackupCallback(offset, storage);
                }
            }

            void onNetworkRestore(size_t offset, storage::IStorage &storage) {
                if (_communicator == nullptr) {
                    if (_networkOnRestoreCallback != nullptr) {
                        _networkOnRestoreCallback(offset, storage);
                    }
                    return;
                }
                communication::ip::tcp_host host;
                storage.read(offset, reinterpret_cast<void *>(host.ipv6), sizeof(host.ipv6));
                offset += sizeof(host.ipv6);
                storage.read(offset, reinterpret_cast<void *>(&(host.hport)), sizeof(host.hport));
                offset += sizeof(host.hport);
                _communicator->setHost(host);
                if (_networkOnRestoreCallback != nullptr) {
                    _networkOnRestoreCallback(offset, storage);
                }
            }
#  endif /* DISABLE_PERSISTENT_STORAGE */

        private:
            void setEndPointCommand(Stream &communicator) {
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
                if (_communicator != nullptr) {
                    _communicator->setHost(host);
#  ifndef DISABLE_PERSISTENT_STORAGE
                    this->onBackupOnStorage();
#  endif /* DISABLE_PERSISTENT_STORAGE */
                    if (!_communicator->connectToHost()) {
                        _updateStreams();
                    }
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
            static void _onCommandReceivedNetwork(const char *command, Stream &stream) {
                AtHomeNetworkModule *instance = reinterpret_cast<AtHomeNetworkModule *>(AtHomeModule<T, n>::getInstance());
                if (instance == nullptr) {
                    return;
                }
                if (!STRCMP(command, communication::commands::setEndPoint)) {
                    instance->setEndPointCommand(stream);
                }
                else if (instance->_networkCommandCallback != nullptr) {
                    instance->_networkCommandCallback(command, stream);
                }
                else {
                    while (stream.read() != communication::commands::end_of_command);
                }
            }
#  ifndef DISABLE_PERSISTENT_STORAGE
            static void _onNetworkBackup(size_t offset, storage::IStorage &storage) {
                AtHomeNetworkModule *instance = reinterpret_cast<AtHomeNetworkModule *>(AtHomeModule<T, n>::getInstance());
                if (instance != nullptr) {
                    instance->onNetworkBackup(offset, storage);
                }
            }

            static void _onNetworkRestore(size_t offset, storage::IStorage &storage) {
                AtHomeNetworkModule *instance = reinterpret_cast<AtHomeNetworkModule *>(AtHomeModule<T, n>::getInstance());
                if (instance != nullptr) {
                    instance->onNetworkRestore(offset, storage);
                }
            }
#  endif /* DISABLE_PERSISTENT_STORAGE */
        private:
            typename AtHomeModule<T, n>::AtHomeCommandPlugin _networkCommandCallback;
#  ifndef DISABLE_PERSISTENT_STORAGE
            typename AtHomeModule<T, n>::AtHomeStoragePlugin _networkOnBackupCallback;
            typename AtHomeModule<T, n>::AtHomeStoragePlugin _networkOnRestoreCallback;
#  endif /* DISABLE_PERSISTENT_STORAGE */
        protected:
            communication::ANetworkCommunicator             *_communicator;
        };
    }
}

# endif /* !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_NETWORK) */
#endif /* ATHOMENETWORKMODULE_HPP */