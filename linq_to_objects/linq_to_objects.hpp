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
#include<vector>
#include<set>
#include<functional>
#include "../function_traits/function_trans.hpp"
using std::multimap;

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
	
	LinqCpp<boost::range_detail::uniqued_range<R>>
	distinct()
	{
		return LinqCpp<boost::range_detail::uniqued_range<R>>(boost::range_detail::uniqued_range<R>(m_linqrange));
	}
	
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
	
	auto count() const->decltype(std::distance(this->begin(),this->end()))
	{
		return std::distance(this->begin(),this->end());
	}
	
	template<typename F>
	auto count(const F& f) const->decltype(std::count_if(this->begin(),this->end(),f))
	{
		return std::count_if(this->begin(),this->end(),f);
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
	auto minmax(const F& f) const->decltype(boost::minmax_element(this->begin(),this->end(),f))
	{
		return boost::minmax_element(this->begin(),this->end(),f);
	}
	
	auto minmax() const->decltype(boost::minmax_element(this->begin(),this->end()))
	{
		return boost::minmax_element(this->begin(),this->end());
	}
	
	auto element(size_t index) const->decltype(std::next(this->begin(),index))
	{
		return std::next(this->begin(),index);
	}
	
	LinqCpp<boost::select_first_range<R>>
	keys() const
	{
		return LinqCpp<boost::select_first_range<R>>(boost::adaptors::keys(m_linqrange));
	}
	
	LinqCpp<boost::select_second_const_range<R>>
	values() const
	{
		return LinqCpp<boost::select_second_const_range<R>>(boost::adaptors::values(m_linqrange));
	}
	
	LinqCpp<boost::reversed_range<R>>
	reverse() const
	{
		return LinqCpp<boost::reversed_range<R>>(boost::adaptors::reverse(m_linqrange));
	}
	/*
	auto take(size_t n) const->LinqCpp<decltype(slice(m_linqrange,0,n))>
	{
		return LinqCpp<decltype(slice(m_linqrange,0,n))>(slice(m_linqrange,0,n));
	}
	
	auto take(size_t st,size_t ed) const->LinqCpp<decltype(slice(m_linqrange,st,ed))>
	{
		return LinqCpp<decltype(slice(m_linqrange,st,ed))>(slice(m_linqrange,st,ed));
	} 
	*/
	vector<value_type> to_vector() const
	{
		return vector<value_type>(begin(),end());
	}
	
	template<typename F>
	auto takewhile(const F& f) const
	->LinqCpp<decltype(boost::make_iterator_range(this->begin(),std::find_if(this->begin(),this->end(),f)))>
	{
		return LinqCpp<decltype(boost::make_iterator_range(begin(),std::find_if(begin(),end(),f)))>
						(boost::make_iterator_range(begin(),std::find_if(begin(),end(),f)));
	}
	/*
	auto skip(size_t n) const->decltype(boost::make_iterator_range(this->begin()+n,this->end()))
	{
		return LinqCpp<decltype(boost::make_iterator_range(begin()+n,end()))>(boost::make_iterator_range(begin()+n,end()));
	}*/
	
	template<typename F>
	auto skipwhile(const F& f) const
	->LinqCpp<decltype(boost::make_iterator_range(std::find_if_not(this->begin(),this->end(),f),this->end()))>
	{
		return LinqCpp<decltype(boost::make_iterator_range(std::find_if_not(begin(),end(),f),end()))>
						(boost::make_iterator_range(std::find_if_not(begin(),end(),f),end()));
	}
	/*
	auto step(size_t n) const->decltype(stride(m_linqrange,n))
	{
		return stride(m_linqrange,n);
	}
	*/
	LinqCpp<boost::indirected_range<R>>
	indirect()
	{
		return LinqCpp<boost::indirected_range<R>>(boost::adaptors::indirect(m_linqrange));
	}
	
	template<typename R2>
	LinqCpp<joined_range<R,const R2>>
	concat(const R2& other)
	{
		return LinqCpp<joined_range<R,const R2>>(boost::join(m_linqrange,other));
	}
	
	template<typename R2>
	void except(const R2& other,std::vector<value_type>& result_vector)
	{
		std::set_difference(begin(),end(),std::begin(other),std::end(other),back_inserter(result_vector));
	}
	
	template<typename R2>
	bool includes(const R2& other) const
	{
		return std::includes(begin(),end(),std::begin(other),std::end(other));
	}
	
	template<typename F>
	multimap<typename std::result_of<F(value_type)>::type,value_type>
	group_by(const F& f) const
	{
		typedef typename std::result_of<F(value_type)>::type keytype;
		multimap<keytype,value_type> mymap;
		std::for_each(begin(),end(),[&mymap,&f](value_type item)
		{
			mymap.insert(std::make_pair(f(item),item));
		});
		return mymap;	
	}
	
	template<typename KeyFn,typename ValueFn>
	multimap<typename std::result_of<KeyFn(value_type)>::type,typename std::result_of<ValueFn(value_type)>::type>
	group_by(const KeyFn& fnk,const ValueFn& fnv)
	{
		typedef typename std::result_of<KeyFn(value_type)>::type keytype;
		typedef typename std::result_of<ValueFn(value_type)>::type valuetype;
		multimap<keytype,valuetype> mymap;
		std::for_each(begin(),end(),[&mymap,&fnk,&fnv](value_type item)
		{
			keytype key=fnk(item);
			valuetype val=fnv(item);
			mymap.insert(std::make_pair(key,val));
		});
		return mymap;	
	}
	
	template<typename T>
	LinqCpp<boost::transformed_range<std::function<T(value_type)>,R>>
	cast() const
	{
		std::function<T(value_type)> f=[](value_type item){return static_cast<T>(item);};
		return LinqCpp<boost::transformed_range<std::function<T(value_type)>,R>>(select(f));
	}
	
	template<typename R2>
	bool equals(const LinqCpp<R2>& other) const
	{
		return count==other.count()&&std::equal(begin(),end(),other.begin());
	}
	
	template<typename R2,typename F>
	bool equals(const LinqCpp<R2>& other,const F& f) const
	{
		return count==other.count()&&std::equal(begin(),end(),other.begin(),f);
	}
	
	template<typename R2>
	bool operator==(const LinqCpp<R2>& other) const
	{
		return equals(other);
	}
	template<typename R2>
	bool operator!=(const LinqCpp<R2>& other) const
	{
		return !(*this==other);
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


