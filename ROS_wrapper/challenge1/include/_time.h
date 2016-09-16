#include <time.h>


inline time_t
get_time( void)
{
	return time( NULL);
}

inline double
diff_time( time_t start, time_t end)
{
	return difftime( end, start);
}
