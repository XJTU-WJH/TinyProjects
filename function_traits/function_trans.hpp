#pragma once
#include<tuple>
#include<functional>
using std::function; 
using std::tuple;
using std::tuple_element;

template<typename T>
struct function_trans;

template<typename Ret,typename... Args>
struct function_trans<Ret(Args...)>
{
    enum{arity=sizeof...(Args)};
    typedef Ret function_type(Args...);
    typedef Ret returntype;
    using stl_function_type=function<function_type>;
    typedef Ret(*pointer)(Args...);
	/*
    template<size_t I>
    struct args
    {
        using type=typename tuple_element<I,tuple<Args...>>::type;
    };*/
};

template<typename Ret,typename... Args>
struct function_trans<Ret(&)(Args...)>:function_trans<Ret(Args...)>{};

template<typename Ret,typename... Args>
struct function_trans<Ret(*)(Args...)>:function_trans<Ret(Args...)>{};

template<typename Ret,typename... Args>
struct function_trans<function<Ret(Args...)>>:function_trans<Ret(Args...)>{};

#define FUNCTION_TRANS(...)\
        template<typename ReturnType,typename ClassType,typename... Args>\
        struct function_trans<ReturnType(ClassType::*)(Args...) __VA_ARGS__>:\
        function_trans<ReturnType(Args...)>{};

FUNCTION_TRANS();
FUNCTION_TRANS(const);
FUNCTION_TRANS(volatile);
FUNCTION_TRANS(const volatile);

template<typename Callable>
struct function_trans:function_trans<decltype(Callable::operator())>{};

template<typename Function>
typename function_trans<Function>::stl_function_type
to_function(const Function& lambda)
{
    return static_cast<typename function_trans<Function>::stl_function_type>(lambda);
}
/*
template<typename Function>
typename function_trans<Function>::stl_function_type
to_function(Function&& lambda)
{
    return static_cast<typename function_trans<Function>::stl_function_type>
    (forward<Function>(lambda));
}*/

template<typename Function>
typename function_trans<Function>::pointer 
to_function_pointer(const Function& lambda)
{
    return static_cast<typename function_trans<Function>::pointer>(lambda);
}
