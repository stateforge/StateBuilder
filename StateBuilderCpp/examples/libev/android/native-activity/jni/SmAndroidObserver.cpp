
#include <android/log.h>
#include "SmAndroidObserver.h"

using namespace std;

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

SmAndroidObserver *SmAndroidObserver::m_pObserver = NULL;

SmAndroidObserver::SmAndroidObserver()
: IObserver()
{
}

SmAndroidObserver::~SmAndroidObserver()
{
}

SmAndroidObserver& SmAndroidObserver::GetInstance()
{
    if(m_pObserver == NULL){
        m_pObserver = new SmAndroidObserver();
    }
    return *m_pObserver;
}

void SmAndroidObserver::OnTransitionBegin(const string &context,
                                        const string &statePrevious,
                                        const string &stateNext,
                                        const string &transition) 
{
	LOGI("OnTransitionBegin: %s, from %s to %s, event %s",
			context.c_str(), statePrevious.c_str(), stateNext.c_str(), transition.c_str());
};

void SmAndroidObserver::OnTransitionEnd(const string &context,
                                      const string &statePrevious,
                                      const string &stateNext,
                                      const string &transition) 
{
	LOGI("OnTransitionEnd: %s, from %s to %s, event %s",
				context.c_str(), statePrevious.c_str(), stateNext.c_str(), transition.c_str());
};

void SmAndroidObserver::OnTimerStart(const string &context, const string &timer, long duration)
{
	LOGI("OnTimerStart: %s, timer %s, duration %ld",
			context.c_str(), timer.c_str(), duration);
}

void SmAndroidObserver::OnTimerStop(const string &context, const string &timer)
{
	LOGI("OnTimerStop: %s, timer %s", context.c_str(), timer.c_str());
}

void SmAndroidObserver::OnEntry(const string &context, const string &state)
{
	LOGI("OnEntry: %s, state %s", context.c_str(), state.c_str());
}

void SmAndroidObserver::OnExit(const string &context, const string &state)
{
	LOGI("OnExit: %s, state %s", context.c_str(), state.c_str());
}

void SmAndroidObserver::OnProcessEventStart(const string &context, size_t remainingEvent)
{
	LOGI("OnProcessEventStart: %s, remaining event(s) %ld", context.c_str(), remainingEvent);
};

void SmAndroidObserver::OnProcessEventStop(const string &context, size_t remainingEvent)
{
	LOGI("OnProcessEventStop: %s, remaining event(s) %ld", context.c_str(), remainingEvent);
};
