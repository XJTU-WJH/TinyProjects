#pragma once

#include<boost/algorithm/minmax_element.hpp>
#include<boost/iterator/zip_iterator.hpp>
#include<boost/range.hpp>
#include<boost/range/join.hpp>
#include<boost/range/adaptors.hpp>
#include<boost/range/algorithm.hpp>
using namespace boost;
using namespace boost::adaptors;

#include<numeric>
#include "../function_traits/function_trans.hpp"


template<typename R>
class LinqCpp
{
public:
	typedef typename R::value_type value_type;
	
public:
	LinqCpp(R& range):m_linqrange(range){}
	LinqCpp(R&& range):m_linqrange(std::move(range)){}
	
	template<typename F>
	LinqCpp<filtered_range<F,R>>
	where(const F& f)
	{
		return LinqCpp<filtered_range<F,R>>(filter(m_linqrange,f));
	}
	
	template<typename F>
	LinqCpp<transformed_range<typename function_trans<F>::stl_function_type,R>>
	select(const F& f)
	{
		auto func=to_function(f);
		return LinqCpp<transformed_range<typename function_trans<F>::stl_function_type,R>>(boost::transform(m_linqrange,func));
	}
	
	auto begin() const->decltype(std::begin(std::declval<const R>()))
	{
		return std::begin(m_linqrange);
	}
	
	auto end() const->decltype(std::end(std::declval<const R>()))
	{
		return std::end(m_linqrange);
	}
	
	template<typename F>
	auto first(const F& f)->decltype(std::find_if(this->begin(),this->end(),f))
	{
		return std::find_if(this->begin(),this->end(),f);
	}
	/*
	template<typename F>
	auto last(const F& f)->decltype(reverse().first(f))
	{
		return reverse().first(f);
	}*/
	
	bool empty() const
	{
		return begin()==end();
	}
	
	template<typename F>
	bool any(const F& f) const
	{
		return std::any_of(begin(),end(),f);
	}
	
	template<typename F>
	bool all(const F& f) const
	{
		return std::all_of(begin(),end(),f);
	}
	
	template<typename F>
	void for_each(const F& f) const
	{
		return std::for_each(begin(),end(),f);
	}
	
	template<typename F>
	bool contains(const F& f)const
	{
		return std::find_if(begin(),end(),f);
	}
	/*
	template<typename F>
	auto distinct(const F& f) const->LinqCpp<decltype(unique(m_linqrange,f))>
	{
		return LinqCpp<decltype(unique(m_linqrange,f))>(unique(m_linqrange,f));	
	}*/
	
	template<typename F>
	value_type aggregate(const F& f) const
	{
		auto it=begin();
		auto val=*it++;
		return std::accumulate(it,end(),std::move(val),f);
	}
	
	value_type sum() const
	{
		return aggregate(std::plus<value_type>());
	}
	
	auto count() const->decltype(std::distance(begin(),end()))
	{
		return std::distance(begin(),end());
	}
	
	template<typename F>
	auto count(const F& f) const->decltype(std::count_if(begin(),end(),f))
	{
		return std::count_if(begin(),end(),f);
	}
	
	template<typename F>
	value_type Min(const F& f) const
	{
		return *std::min_element(begin(),end(),f);
	}
	
	value_type Min() const
	{
		return *std::min_element(begin(),end());
	}
	
	template<typename F>
	value_type Max(const F& f) const
	{
		return *std::max_element(begin(),end(),f);
	}
	
	value_type Max() const
	{
		return *std::max_element(begin(),end());
	}
	
	template<typename F>
	auto minmax(const F& f) const->decltype(boost::minmax_element(begin(),end(),f))
	{
		return boost::minmax_element(begin(),end(),f);
	}
	
	auto minmax() const->decltype(boost::minmax_element(begin(),end()))
	{
		return boost::minmax_element(begin(),end());
	}
	
private:
	R m_linqrange;
};

template<template<typename T> class IteratorRange,typename R>
using Range=IteratorRange<decltype(std::begin(std::declval<R>()))>;

template<typename R>
using Iterator_range=Range<boost::iterator_range,R>;

template<typename R>
LinqCpp<Iterator_range<R>> from(const R& range)
{
	return LinqCpp<Iterator_range<R>>(Iterator_range<R>(range));
}

template<typename... Args>
auto zip(const Args&... args)->
boost::iterator_range<boost::zip_iterator<decltype(boost::make_tuple(std::begin(args)...))>>
{
	auto zip_begin=boost::make_zip_iterator(boost::make_tuple(std::begin(args)...));
	auto zip_end=boost::make_zip_iterator(boost::make_tuple(std::end(args)...));
	return boost::make_iterator_range(zip_begin,zip_end);
}


