#include "php.h"
#include <avcodec.h>

void ffmpeg_errorhandler(void *ptr, int level, const char *msg, va_list args)
{
	int php_level;
	TSRMLS_FETCH();

	switch (level) {
		case AV_LOG_ERROR:
			php_level = E_WARNING;
		break;

		case AV_LOG_INFO:
		case AV_LOG_DEBUG:
		default:
			php_level = E_NOTICE;
		break;
	}

	php_verror("", "", php_level, msg, args TSRMLS_CC);
}