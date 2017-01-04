
// The MIT License(MIT)
// 
// Copyright(c) 2015 Stefan Reinalter
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef TERMINUS_DELEGATES
#define TERMINUS_DELEGATES

#include <assert.h>
#include <functional>

namespace terminus
{
    template <typename T>
    class Delegate {};
    
    
    template <typename R, typename... Args>
    class Delegate<R(Args...)>
    {
        typedef R (*ProxyFunction)(void*, Args...);
        
        template <R (*Function)(Args...)>
        static inline R FunctionProxy(void*, Args... args)
        {
            return Function(std::forward<Args>(args)...);
        }
        
        template <class C, R (C::*Function)(Args...)>
        static inline R MethodProxy(void* instance, Args... args)
        {
            return (static_cast<C*>(instance)->*Function)(std::forward<Args>(args)...);
        }
        
        template <class C, R (C::*Function)(Args...) const>
        static inline R ConstMethodProxy(void* instance, Args... args)
        {
            return (static_cast<const C*>(instance)->*Function)(std::forward<Args>(args)...);
        }
        
    public:
        Delegate()
        : m_instance(nullptr)
        , m_proxy(nullptr)
        {
        }
        
        template <R (*Function)(Args...)>
        void Bind(void)
        {
            m_instance = nullptr;
            m_proxy = &FunctionProxy<Function>;
        }
        
        template <class C, R (C::*Function)(Args...)>
        void Bind(C* instance)
        {
            m_instance = instance;
            m_proxy = &MethodProxy<C, Function>;
        }
        
        template <class C, R (C::*Function)(Args...) const>
        void Bind(const C* instance)
        {
            m_instance = const_cast<C*>(instance);
            m_proxy = &ConstMethodProxy<C, Function>;
        }
        
        R Invoke(Args... args) const
        {
            assert((m_proxy != nullptr) && "Cannot invoke unbound Delegate. Call Bind() first.");
            return m_proxy(m_instance, std::forward<Args>(args)...);
        }
        
    private:
        void* m_instance;
        ProxyFunction m_proxy;
    };
}

#endif
