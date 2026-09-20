#ifndef STA_SIGNAL_HANDLER_H
#define STA_SIGNAL_HANDLER_H

#include <signal.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*sta_sig_cb)(int signo, int *is_handled);

/**
 * Configures signal dispositions across all 29 catchable POSIX signals.
 *
 * @param optional_ignored_callback  CB for ignored group (SIGPIPE, SIGCHLD, etc.).
 * @param optional_shutdown_callback CB for shutdown/async group (SIGINT, SIGTERM, etc.).
 * @param optional_job_callback      CB for terminal job control group (SIGTSTP, SIGCONT, etc.).
 * @param handle_fatal               If non-zero, traps fatal errors and exits via _exit(128 + sig).
 *                                   If zero, leaves fatal errors bound to system default (SIG_DFL).
 * @return                           0 on success, -1 if any sigaction registration fails.
 */
int a_handle_signals(sta_sig_cb optional_ignored_callback,
                     sta_sig_cb optional_shutdown_callback,
                     sta_sig_cb optional_job_callback,
                     int handle_fatal);

#ifdef __cplusplus
}
#endif

#endif /* STA_SIGNAL_HANDLER_H */

#ifdef STA_SIGNAL_HANDLER_IMPLEMENTATION

static sta_sig_cb g_sta_ignored_cb  = NULL;
static sta_sig_cb g_sta_shutdown_cb = NULL;
static sta_sig_cb g_sta_job_cb      = NULL;

static void sta_ignored_dispatcher(int signo)
{
    int is_handled = 0;
    if (g_sta_ignored_cb) {
        g_sta_ignored_cb(signo, &is_handled);
    }
}

static void sta_shutdown_dispatcher(int signo)
{
    int is_handled = 0;
    if (g_sta_shutdown_cb) {
        g_sta_shutdown_cb(signo, &is_handled);
    }
    
    if (!is_handled) {
        _exit(128 + signo);
    }
}

static void sta_job_dispatcher(int signo)
{
    int is_handled = 0;
    if (g_sta_job_cb) {
        g_sta_job_cb(signo, &is_handled);
    }

    if (!is_handled) {
        /* Reset signal handler to default and re-raise to perform native kernel job control */
        struct sigaction sa_dfl;
        sa_dfl.sa_handler = SIG_DFL;
        sa_dfl.sa_flags   = 0;
        sigemptyset(&sa_dfl.sa_mask);
        sigaction(signo, &sa_dfl, NULL);
        raise(signo);
    }
}

static void sta_fatal_dispatcher(int signo)
{
    _exit(128 + signo);
}

int a_handle_signals(sta_sig_cb optional_ignored_callback,
                     sta_sig_cb optional_shutdown_callback,
                     sta_sig_cb optional_job_callback,
                     int handle_fatal)
{
    struct sigaction sa_ign;
    struct sigaction sa_shut;
    struct sigaction sa_job;
    struct sigaction sa_fatal;

    g_sta_ignored_cb  = optional_ignored_callback;
    g_sta_shutdown_cb = optional_shutdown_callback;
    g_sta_job_cb      = optional_job_callback;

    /* 1. Setup Ignored Signal Group (5 signals) */
    if (optional_ignored_callback != NULL) {
        sa_ign.sa_handler = sta_ignored_dispatcher;
        sa_ign.sa_flags   = SA_RESTART;
    } else {
        sa_ign.sa_handler = SIG_IGN;
        sa_ign.sa_flags   = 0;
    }
    sigemptyset(&sa_ign.sa_mask);

    if (sigaction(SIGPIPE,  &sa_ign, NULL) < 0) return -1;
    if (sigaction(SIGWINCH, &sa_ign, NULL) < 0) return -1;
    if (sigaction(SIGURG,   &sa_ign, NULL) < 0) return -1;
#ifdef SIGIO
    if (sigaction(SIGIO,    &sa_ign, NULL) < 0) return -1;
#endif

    if (optional_ignored_callback != NULL) {
        if (sigaction(SIGCHLD, &sa_ign, NULL) < 0) return -1;
    } else {
        struct sigaction sa_chld;
        sa_chld.sa_handler = SIG_IGN;
        sa_chld.sa_flags   = SA_NOCLDWAIT | SA_NOCLDSTOP;
        sigemptyset(&sa_chld.sa_mask);
        if (sigaction(SIGCHLD, &sa_chld, NULL) < 0) return -1;
    }

    /* 2. Setup Shutdown / Async Signal Group (12 signals) */
    sa_shut.sa_handler = sta_shutdown_dispatcher;
    sa_shut.sa_flags   = 0;
    sigemptyset(&sa_shut.sa_mask);

    if (sigaction(SIGINT,    &sa_shut, NULL) < 0) return -1;
    if (sigaction(SIGTERM,   &sa_shut, NULL) < 0) return -1;
    if (sigaction(SIGHUP,    &sa_shut, NULL) < 0) return -1;
    if (sigaction(SIGQUIT,   &sa_shut, NULL) < 0) return -1;
    if (sigaction(SIGUSR1,   &sa_shut, NULL) < 0) return -1;
    if (sigaction(SIGUSR2,   &sa_shut, NULL) < 0) return -1;
    if (sigaction(SIGALRM,   &sa_shut, NULL) < 0) return -1;
    if (sigaction(SIGVTALRM, &sa_shut, NULL) < 0) return -1;
    if (sigaction(SIGPROF,   &sa_shut, NULL) < 0) return -1;
    if (sigaction(SIGXCPU,   &sa_shut, NULL) < 0) return -1;
    if (sigaction(SIGXFSZ,   &sa_shut, NULL) < 0) return -1;
#ifdef SIGPWR
    if (sigaction(SIGPWR,    &sa_shut, NULL) < 0) return -1;
#endif

    /* 3. Setup Job Control Signal Group (4 signals) */
    if (optional_job_callback != NULL) {
        sa_job.sa_handler = sta_job_dispatcher;
        sa_job.sa_flags   = SA_RESTART;
        sigemptyset(&sa_job.sa_mask);

        if (sigaction(SIGCONT, &sa_job, NULL) < 0) return -1;
        if (sigaction(SIGTSTP, &sa_job, NULL) < 0) return -1;
        if (sigaction(SIGTTIN, &sa_job, NULL) < 0) return -1;
        if (sigaction(SIGTTOU, &sa_job, NULL) < 0) return -1;
    } else {
        struct sigaction sa_dfl;
        sa_dfl.sa_handler = SIG_DFL;
        sa_dfl.sa_flags   = 0;
        sigemptyset(&sa_dfl.sa_mask);

        sigaction(SIGCONT, &sa_dfl, NULL);
        sigaction(SIGTSTP, &sa_dfl, NULL);
        sigaction(SIGTTIN, &sa_dfl, NULL);
        sigaction(SIGTTOU, &sa_dfl, NULL);
    }

    /* 4. Setup Fatal Signal Group (8 signals) */
    if (handle_fatal) {
        sa_fatal.sa_handler = sta_fatal_dispatcher;
        sa_fatal.sa_flags   = SA_NODEFER | SA_RESETHAND;
        sigemptyset(&sa_fatal.sa_mask);

        if (sigaction(SIGSEGV,   &sa_fatal, NULL) < 0) return -1;
        if (sigaction(SIGFPE,    &sa_fatal, NULL) < 0) return -1;
        if (sigaction(SIGBUS,    &sa_fatal, NULL) < 0) return -1;
        if (sigaction(SIGILL,    &sa_fatal, NULL) < 0) return -1;
        if (sigaction(SIGABRT,   &sa_fatal, NULL) < 0) return -1;
        if (sigaction(SIGSYS,    &sa_fatal, NULL) < 0) return -1;
        if (sigaction(SIGTRAP,   &sa_fatal, NULL) < 0) return -1;
#ifdef SIGSTKFLT
        if (sigaction(SIGSTKFLT, &sa_fatal, NULL) < 0) return -1;
#endif
    } else {
        sa_fatal.sa_handler = SIG_DFL;
        sa_fatal.sa_flags   = 0;
        sigemptyset(&sa_fatal.sa_mask);

        sigaction(SIGSEGV,   &sa_fatal, NULL);
        sigaction(SIGFPE,    &sa_fatal, NULL);
        sigaction(SIGBUS,    &sa_fatal, NULL);
        sigaction(SIGILL,    &sa_fatal, NULL);
        sigaction(SIGABRT,   &sa_fatal, NULL);
        sigaction(SIGSYS,    &sa_fatal, NULL);
        sigaction(SIGTRAP,   &sa_fatal, NULL);
#ifdef SIGSTKFLT
        sigaction(SIGSTKFLT, &sa_fatal, NULL);
#endif
    }

    return 0;
}

#endif /* STA_SIGNAL_HANDLER_IMPLEMENTATION */

// STA_SIGNAL_HANDLER Design by Arshad Latti with help of Gemini and implemented by Gemini