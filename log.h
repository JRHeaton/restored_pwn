#ifndef RESTORED_PWN_LOG_H
#define RESTORED_PWN_LOG_H

#define log_function() restored_log("entering %s\n", __FUNC__);

void restored_log(const char *fmt, ...);

#endif /* RESTORED_PWN_LOG_H */