
// Deprecated...
/*
#define DECLARE_DYNAMIC_SINGLETON( )\
private:\
	template<typename base_t>\
	inline static base_t owner_tag( void( base_t::* )( ));\
\
	void internal_tag( );\
	typedef decltype( owner_tag( &internal_tag )) this_t;\
\
public:\
	static const std::unique_ptr<this_t>& Get( )\
	{\
	}
*/
