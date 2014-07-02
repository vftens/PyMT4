/* (C) Copyright 2013 Rob Watson rmawatson [at] hotmail.com  and others.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * Contributors:
 *     Rob Watson ( rmawatson [at] hotmail )
 *
 */


#include <boost/mpl/for_each.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/type_traits.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/bind.hpp>
#include <boost/bind/placeholders.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/zip_view.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/negate.hpp>
#include <boost/mpl/remove.hpp>
#include <boost/mpl/remove_if.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/print.hpp>
#include <iostream>
#include <boost/type_traits.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <typeinfo>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <vector>

#include "pymt4_client.h"
#include "pymt4_common_serializer.h"
#include "pymt4_client_ioconnector.h"

namespace PyMT4
{

struct ignore{};

template <typename T> struct SerializeArgumentHelper {
	void operator()(Buffer& serialized_buffer,T& arg) 
	{	

		Serializer<T>::serializeItem(&arg,&serialized_buffer);
	}
};

template <> struct SerializeArgumentHelper<ignore>
{void operator()(Buffer&,ignore&){}};

struct SerializeArgument {
	
	void** _argptrs;
	Buffer& _serialized_buffer;

	SerializeArgument(Buffer& serialized_buffer,void** arg_ptrs) :_argptrs(arg_ptrs), _serialized_buffer(serialized_buffer) {}
	template <typename T> void operator()(T) { 
		SerializeArgumentHelper< boost::mpl::at_c<T,0>::type >()(_serialized_buffer,*(boost::mpl::at_c<T,0>::type*)(_argptrs[boost::mpl::at_c<T,1>::type::value])); 
}};

template<typename T> struct AssignHelper {
	template <int32_t I,typename T> void operator()(void** arg_ptrs,T* arg_ptr) {
		arg_ptrs[I] = (void*)arg_ptr; }};

template<> struct AssignHelper<boost::mpl::true_> {
	template <int32_t I,typename T> void operator()(void** arg_ptrs,T* arg_ptr){} };

using namespace boost;

#define ARG_MAX 12
#define ARG_TEMPLATE_DEC(z, n, unused) typename T ## n=ignore BOOST_PP_COMMA_IF( BOOST_PP_SUB(BOOST_PP_SUB(ARG_MAX,1),n) )
#define ARG_SIGNATURE_DEC(z, n, unused) T ## n arg ## n=T ## n()  BOOST_PP_COMMA_IF( BOOST_PP_SUB(BOOST_PP_SUB(ARG_MAX,1),n) )

struct value_printer
{
    template< typename U > void operator()(U x)
    {
        std::cout << typeid(U).name << std::endl;
    }
};

struct print_class_name {
    template <typename T>
    void operator()( T t ) const {
       std::cout << typeid(t).name() << " ";
    }
};


typedef std::map<boost::thread::id,int32_t> LastErrorMap;

extern LastErrorMap _lastErrorMap;
extern boost::mutex _lastErrorMapLock;
extern boost::mutex _dispatchLock;


template <typename R,BOOST_PP_REPEAT(ARG_MAX, ARG_TEMPLATE_DEC,~) > struct DispatchFunction {
	R inline operator()( const CommandIdentifier& commandIdentifier,BOOST_PP_REPEAT(ARG_MAX,ARG_SIGNATURE_DEC,~) ){

		//boost::mutex::scoped_lock dispatchLock(_dispatchLock);

		typedef BOOST_TYPEOF(&DispatchFunction::operator()) this_type;
		typedef mpl::filter_view<mpl::transform_view<
									mpl::pop_front< mpl::pop_front<boost::function_types::parameter_types<this_type>>::type >::type,
									boost::remove_reference<mpl::placeholders::_> 
											   >::type,
								 boost::mpl::not_<boost::is_same<mpl::placeholders::_,ignore> > 
								 >::type filtered_params;

		typedef boost::mpl::size<filtered_params>::type this_arity;
		typedef mpl::zip_view< mpl::vector< filtered_params, boost::mpl::range_c<int32_t,0,this_arity::value>::type >::type >::type parameter_items;

		void** arg_ptrs = NULL;

		if (this_arity::value) arg_ptrs = new void*[this_arity::value];


#		define ARG_ASSIGN(z,n,unused) AssignHelper< mpl::greater_equal<boost::integral_constant<unsigned,n>,this_arity >::type >().operator()<n>(arg_ptrs,&arg ## n);
#		define BOOST_PP_LOCAL_MACRO(n)   ARG_ASSIGN(~, n, ~)
#		define BOOST_PP_LOCAL_LIMITS     (0, ARG_MAX - 1)
#		include BOOST_PP_LOCAL_ITERATE()
		
		MessageCommandPtr messageCommand = MessageCommand::Create(commandIdentifier);
		Buffer& messageBuffer = messageCommand->messageBuffer();

		boost::mpl::for_each<parameter_items>(SerializeArgument(messageBuffer,arg_ptrs));

		if (this_arity::value) delete[] arg_ptrs;

		IOConnectorPtr connector = IOConnector::Instance();

		PendingResultPtr pendingResult = connector->dispatchMessage(messageCommand);
//		dispatchLock.unlock();

		R resultValue = pendingResult->waitForResult<R>();


		{
			boost::mutex::scoped_lock errorMapLock(_lastErrorMapLock);
			int32_t lastError = pendingResult->error();
			_lastErrorMap[boost::this_thread::get_id()] = lastError;
		}
		//dispatchLock.lock();
		return resultValue;

	} 
};

}

