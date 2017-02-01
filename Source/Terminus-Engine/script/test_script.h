#pragma once

#include "cpp_script.h"

class TERMINUS_API TestScript : public terminus::CppScript
{
private:
    bool is_first;
    
public:
    TestScript();
    ~TestScript();
    virtual void initialize() override;
    virtual void update(double dt) override;
    virtual void shutdown() override;
    virtual void serialize(void* mem) override;
    virtual void deserialize(void* data) override;
    virtual size_t get_size() override;
};

DECLARE_FACTORY_FUNC(TestScript, terminus::CppScript);
