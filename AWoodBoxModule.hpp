#ifndef AWOODBOXMODULE_HPP
# define AWOODBOXMODULE_HPP

# include "ABaseModule.hpp"

class AWoodBoxModule : public ABaseModule {
    public:
        AWoodBoxModule(const AWoodBoxModule &) = delete;
        AWoodBoxModule &operator=(const AWoodBoxModule &) = delete;
        virtual ~AWoodBoxModule();

    protected:
        AWoodBoxModule();
    private:
};

#endif /* AWOODBOXMODULE_HPP */
