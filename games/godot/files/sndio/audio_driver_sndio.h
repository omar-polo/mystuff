/* $OpenBSD$ */

#include "servers/audio_server.h"

#ifdef SNDIO_ENABLED

#include "core/os/mutex.h"
#include "core/os/thread.h"

#include <sndio.h>

class AudioDriverSndio : public AudioDriver {
	Thread *thread;
	Mutex *mutex;

	Vector<int32_t> samples_in;
	Vector<int16_t> samples_out;

	struct sio_hdl *handle;

	static void thread_func(void*);
	size_t period_size;

	unsigned int mix_rate;
	int channels;
	bool active;
	bool thread_exited;
	mutable bool exit_thread;
	SpeakerMode speaker_mode;

public:
	const char *get_name() const {
		return "sndio";
	}

	virtual Error init();
	virtual void start();
	virtual int get_mix_rate() const;
	virtual SpeakerMode get_speaker_mode() const;
	virtual void lock();
	virtual void unlock();
	virtual void finish();

	AudioDriverSndio();
	~AudioDriverSndio();
};

#endif
