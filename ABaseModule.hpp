#ifndef ABASEMODULE_HPP
# define ABASEMODULE_HPP

# include "ADisplayModule.hpp"
# include "ACommunicativeModule.hpp"
# include "APoweredModule.hpp"
# include "ASensorModule.hpp"
# include "AStorageModule.hpp"

namespace woodBox {
    namespace module {
        class ABaseModule : public ADisplayModule, public ACommunicativeModule, public APoweredModule, public ASensorModule, public AStorageModule {
            public:
                virtual ABaseModule &operator=(ABaseModule &) = delete;
                virtual ~ABaseModule();
                virtual void run();
                virtual void stop();
				template <typename T>
				static ABaseModule *getInstance() {
					if (_instance == nullptr) {
						_instance = new T();
						return _instance;
					}
					return dynamic_cast<T*>(_instance);
				}
            protected:
				ABaseModule(display::IDisplay * = nullptr, communication::ICommunicator * = nullptr,
					power::IPower * = nullptr, sensor::ISensor * = nullptr, storage::IStorage * = nullptr);
                ABaseModule(ABaseModule &);
                virtual void onReset() = 0;
                virtual void onStart() = 0;
                virtual void onStop() = 0;
                virtual void onPause() = 0;
                virtual void onResume() = 0;
				virtual void onSleep() = 0;
                virtual void onWakeUp() = 0;
                virtual void onBackupOnStorage() = 0;
                virtual void onRestoreFromStorage() = 0;
				virtual void onSampleSensor() = 0;
				virtual void onUpdateDisplay() = 0;
			private:
				static ABaseModule *_instance;
        };
    }
}

#endif /* ABASEMODULE_HPP */
