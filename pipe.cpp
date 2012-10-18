#pragma warning(disable:4996)
//#include <tricky.h>
#include "typedef.h"
#include "pipe.h"
//#include "../base/util.h"

#ifdef KERNEL_MODE
# include <undoc.h>
# include <cpprt.h>
#else
# include <windows.h>
#endif

void* open_named_pipe( const char* name ) {
	return CreateFile( name, GENERIC_ALL, 0, 0, OPEN_EXISTING, 0, 0 );
}
void* crt_named_pipe( const char* name ) {
	return CreateNamedPipe( name, PIPE_ACCESS_DUPLEX, 
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 
		PIPE_UNLIMITED_INSTANCES, 4096, 4096, -1, 0 );
}
bool connect_pipe( void* handle ) { 
	return ConnectNamedPipe( handle, 0 ) != false; 
}
bool disconnect_pipe( void* handle ) { 
	return DisconnectNamedPipe( handle ) != false; 
}
void close_handle( void* handle ) { 
	CloseHandle( handle ); 
}
bool read_msg( void* handle, void* buffer, uint32_t length, int* nr ) {
	return ReadFile( handle, buffer, length, (LPDWORD)nr, 0 ) != false;
}
bool write_msg( void* handle, void* buffer, uint32_t length, int* nr ) {
	return WriteFile( handle, buffer, length, (LPDWORD)nr, 0 ) != false;
}

void nmp_server_t::close()
{
	//m_handle_mutex.p();
	m_handle && ( disconnect_pipe( m_handle ), close_handle( m_handle ), m_handle = 0 );
	//m_handle_mutex.v();
}

void nmp_client_t::close()
{
	//m_handle_mutex.p();
	m_handle && ( close_handle( m_handle ), m_handle = 0 );
	//m_handle_mutex.v();
}

int named_pipe_t::read( void* buffer, int nb )
{
	//auto_sync_t as( m_handle_mutex );
	if (buffer == NULL || m_handle == NULL || nb <= 0)
		return 0;
	
	int nb_read;
	if (!read_msg(m_handle, buffer, nb, (int*)&nb_read))
		return 0;

	return nb_read;
}

int named_pipe_t::write( void* buffer, int nb )
{
	//auto_sync_t as( m_handle_mutex );
	if (buffer == NULL || m_handle == NULL || nb <= 0)
		return 0;

	int nb_written;
	if (!write_msg(m_handle, buffer, nb, (int*)&nb_written))
		return 0;

	return nb_written;
}

int nmp_server_t::create( const char* name )
{
	//auto_sync_t as( m_handle_mutex );
	if (m_handle != NULL || name == NULL)
		return false;

	char pipename[ 1024 ] = "\\\\.\\pipe\\";
	strcat( pipename, name );

	m_handle = crt_named_pipe( pipename );
	if (m_handle == ( void* )-1) {
		m_handle = 0;
		return false;
	}

	return true;
}

int nmp_server_t::wait()
{
	//auto_sync_t as( m_handle_mutex );
	return connect_pipe( m_handle ) != false;
}

int nmp_client_t::connect( const char* name )
{
	if( m_handle != NULL || name == NULL)
		return false;

	char pipename[ 1024 ] = "\\\\.\\pipe\\";
	strcat( pipename, name );
	
	do {
		//m_handle_mutex.p();
		m_handle = open_named_pipe( pipename );
		( m_handle == ( void* )-1 ) && ( m_handle = 0 );
		//m_handle_mutex.v();
		Sleep( 10 );
	} while( !m_handle );

	return true;
}
