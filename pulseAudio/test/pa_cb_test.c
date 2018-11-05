/*
 * pulseaudio callback test.
 * implement the log_Time && getDataTime
 *
 * build:
 * gcc -g -o test pa_cb_test.c -lpthread -lpulse
 */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/timeb.h>

#include <pulse/pulseaudio.h>
#include <pulse/ext-stream-restore.h>

pa_mainloop* mainloop;
pa_mainloop_api* mainloop_api;
pa_context* context;

static char* getDateTime()
{
	static char nowtime[20];
	time_t rawtime;
	struct tm* ltime;
	time(&rawtime);
	ltime = localtime(&rawtime);
	strftime(nowtime, 20, "%Y-%m-%d %H:%M:%S", ltime);
	return nowtime;
}

static char* log_Time(void)
{
    struct  tm      *ptm;
    struct  timeb   stTimeb;
    static  char    szTime[19];

    ftime(&stTimeb);
    ptm = localtime(&stTimeb.time);
    sprintf(szTime, "%02d-%02d %02d:%02d:%02d.%03d",
            ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, stTimeb.millitm);
    szTime[18] = 0;
    return szTime;
}

static int setup_context()
{
    printf("setup_context enter.\n");
	mainloop= pa_mainloop_new();
	if (!mainloop) {
    	printf("pa_mainloop_new() failed.\n");
		return -1;
	}
	mainloop_api = pa_mainloop_get_api(mainloop);

    pa_proplist     *proplist;
    proplist = pa_proplist_new();
    pa_proplist_sets(proplist, PA_PROP_APPLICATION_NAME, "Commandline sink switcher");
    pa_proplist_sets(proplist, PA_PROP_APPLICATION_ID, "org.tablix.paswitch");

	context = pa_context_new_with_proplist(mainloop_api, NULL, proplist);
	if (!context) {
    	printf("pa_context_new() failed.\n");
    	return -1;
	}
    pa_proplist_free(proplist);

	if(pa_context_connect(context, NULL, 0, NULL) < 0) {
    	printf("pa_context_connect() failed: %s\n", pa_strerror(pa_context_errno(context)));
    	return -1;
	}

	return 0;
}

void subscribe_cb(pa_context *c, pa_subscription_event_type_t t, uint32_t index, void *userdata) {

    switch (t & PA_SUBSCRIPTION_EVENT_FACILITY_MASK) {
        case PA_SUBSCRIPTION_EVENT_SINK:     //0x0000U
            if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE) {
                printf("subscribe_cb remove the sink time:%s\n", log_Time());
            } else {
                printf("subscribe_cb add the sink time:%s\n", log_Time());
            }
            break;
         default:
            break;
        }
}

static void context_state_callback(pa_context *c, void *userdata)
{
    printf("context_state_callback enter \n");
	switch(pa_context_get_state(c)) {
		case PA_CONTEXT_CONNECTING:
		case PA_CONTEXT_AUTHORIZING:
		case PA_CONTEXT_SETTING_NAME:
			break;

		case PA_CONTEXT_READY:
			printf("context_state_callback GOOOOOOO \n");

            pa_operation *o;

            pa_context_set_subscribe_callback(c, subscribe_cb, NULL);
            if (!(o = pa_context_subscribe(c, (pa_subscription_mask_t)
                                           (PA_SUBSCRIPTION_MASK_SINK|
                                            PA_SUBSCRIPTION_MASK_SOURCE), NULL, NULL))) {
                printf("pa_context_subscribe() failed");
                return;
            }
            pa_operation_unref(o);
			break;

		case PA_CONTEXT_TERMINATED:
    			mainloop_api->quit(mainloop_api, 0);
			break;

		default:
			printf("connection failure: %s\n",
					pa_strerror(pa_context_errno(c)));
			mainloop_api->quit(mainloop_api, 1);
	}
}

void* main_loop(void *arg) {
    pthread_detach(pthread_self());
    printf("main_loop enter.\n");
    if(setup_context()) {
        printf("can't get pulseaudio context.\n");
        return NULL;
    }
    if(!context) {
        printf("context null.\n");
        return NULL;
    }
    pa_context_set_state_callback(context, context_state_callback, NULL);
    printf("main_loop run.\n");
    int ret;
    if(pa_mainloop_run(mainloop, &ret) < 0) {
        printf("pa_mainloop_run() failed.\n");
        return NULL;
    }

    pa_context_disconnect(context);
    pa_context_unref(context);
    pa_signal_done();
    pa_mainloop_free(mainloop);

    return NULL;
}

int main(int argc, char** argv)
{
    int ret = 0;
    pthread_t pid_t0;

    printf("start pid_t0.\n");
    if (pthread_create(&pid_t0, NULL, main_loop, NULL) != 0) {
        return -1;
    }

    while(1) {
        sleep(1);
        printf("in alive.\n");
    }

	return ret;
}
