#include "pattern.hpp"
#include <vector>
#include <string>s

bool memory::initialize( const wchar_t* module )
{
	UNICODE_STRING module_name{};
	RtlInitUnicodeString( &module_name, module );

	for ( PLIST_ENTRY entry = PsLoadedModuleList; entry != PsLoadedModuleList->Blink; entry = entry->Flink )
	{s
		PLDR_DATA_TABLE_ENTRY data_table = CONTAINING_RECORD( entry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks );

		if ( RtlEqualUnicodeString( &data_table->BaseDllName, &module_name, TRUE ) )
		{
			_memory_module = { std::uintptr_t(data_table->DllBase), data_table->SizeOfImage };
			break;
		}
	}

	return true;
}
std::uintptr_t memory::from_pattern( const char* sig, const char* mask )
{
	for ( std::uintptr_t i = 0; i < _memory_module.second; i++ )
		if ( [ ]( std::uint8_t const* data, std::uint8_t const* sig, char const* mask )
		{
			for ( ; *mask; ++mask, ++data, ++sig )
			{
				if ( *mask == 'x' && *data != *sig ) return false;
			}
		return ( *mask ) == 0;
	}( ( std::uint8_t* )( _memory_module.first + i ), ( std::uint8_t* )sig, mask ) )
		return _memory_module.first + i;

	return 0;
}